#include "DashBoss.h"
#include "../../Player/Player.h"

using namespace LWP;

DashBoss::~DashBoss() {

}

void DashBoss::Init()
{
	// 当たり判定のインスタンス生成
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	// 大きさ
	models_[0].worldTF.scale = { 2,3,2 };
	// 当たり判定を有効化
	isActive_ = true;

	// 攻撃のクールタイム
	attackWaitTime_ = kAttackWaitTime;
	// 現在の時間
	currentFrame_ = 0;

	// 攻撃前のモーション
	preAttackMotion_.Add(&models_[0].worldTF.scale, lwp::Vector3{ -1,-1,-1 }, 0, 1.4f, LWP::Utility::Easing::Type::OutQuart)
		.Add(&models_[0].worldTF.scale, lwp::Vector3{ 1,1,1 }, 1.45f, 0.2f, LWP::Utility::Easing::Type::OutQuart);

	moveSpeed_ = 2.0f;

	// HPを設定
	hp_ = 60;

	accumulateParticle_.model.LoadCube();
	accumulateParticle_.model.materials[1].enableLighting = false;

	deadParticle_.model.LoadCube();
}

void DashBoss::Update()
{
	// 死亡時アニメーション
	// 死んだかどうかはすぐに判別
	if (IsDead_) {
		Dying();
		DyingAnimation();
		return;
	}

	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			B_RootInit();
			break;
			// ダッシュ攻撃前の硬直
		case Behavior::kPreDash:
			B_PreDashInit();
			break;
			// ダッシュ
		case Behavior::kDash:
			B_DashInit();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// 更新処理
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		B_RootUpdate();
		// 体を自機に向ける
		LockPlayer();
		break;
		// ダッシュ攻撃前の硬直
	case Behavior::kPreDash:
		B_PreDashUpdate();
		// 体を自機に向ける
		LockPlayer();
		break;
		// ダッシュ
	case Behavior::kDash:
		B_DashUpdate();
		break;
	}
}

void DashBoss::Attack()
{

}

void DashBoss::DyingAnimation() {

}

bool DashBoss::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0].worldTF.translation - player_->GetWorldPosition()).Length();
	if (distance <= kAttackRange_) {
		return true;
	}
	return false;
}

void DashBoss::B_RootInit() {
	attackWaitTime_ = kAttackWaitTime;
	models_[0].worldTF.scale = { 2,3,2 };
	currentFrame_ = 0;
}

void DashBoss::B_RootUpdate() {
	Move();

	// 攻撃の待ち時間
	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
	else {
		// 攻撃範囲にいるなら攻撃可能状態に移行する
		if (CheckAttackRange()) {
			IsAttack_ = true;
		}
	}

	if (IsAttack_) {
		behaviorRequest_ = Behavior::kPreDash;
	}
}

void DashBoss::B_PreDashInit() {
	currentFrame_ = 0;
	preAttackMotion_.Start();
}

void DashBoss::B_PreDashUpdate() {
	// パーティクルを出す時間
	if (currentFrame_ <= 60 && isActive_) {
		if (currentFrame_ % 2 == 0) {
			accumulateParticle_.model.worldTF.translation = models_[0].worldTF.translation;
			accumulateParticle_.Add(16);
		}
	}

	if (currentFrame_ >= 110) {
		// 自機との方向ベクトルを取得(ただしy方向のベクトルは取得しない)
		dashVel_ = GetDirectVel();
		dashVel_.y = 0;
	}

	if (currentFrame_ >= 130) {
		behaviorRequest_ = Behavior::kDash;
	}
	currentFrame_++;
}

void DashBoss::B_DashInit() {
	currentFrame_ = 0;
	IsAttack_ = false;
	models_[0].worldTF.scale = { 2,3,2 };
}

void DashBoss::B_DashUpdate() {
	// 突進攻撃
	models_[0].worldTF.translation += dashVel_ * kDashSpeedCoefficient;

	// 既定の時間を過ぎたら攻撃終了
	if (currentFrame_ >= kDashAttackAllFrame) {
		behaviorRequest_ = Behavior::kRoot;
	}

	currentFrame_++;
}