#include "ArrowBoss.h"
#include "Object/Player/Player.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Object::Collider;

ArrowBoss::~ArrowBoss() {
	for (HomingArrow* arrow : homingArrows_) {
		delete arrow;
	}
	homingArrows_.clear();
}

void ArrowBoss::Init()
{
	// 当たり判定のインスタンス生成
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	// 大きさ
	models_[0].worldTF.scale = { 2,3,2 };
	models_[0].worldTF.translation = { 0,0,60 };

	shotCount_ = 0;

	hp_ = 200;
}

void ArrowBoss::Update()
{
	// 死亡時アニメーション
	// 死んだかどうかはすぐに判別
	if (isDead_) {
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
			B_RootInit();
			break;
			// 自機を狙う
		case Behavior::kAiming:
			B_AimingInit();
			break;
			// 撃つ
		case Behavior::kNormalShot:
			B_NormalShotInit();
			break;
			// ホーミング弾
		case Behavior::kHomingShot:
			B_HomingShotInit();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// 更新処理
	switch (behavior_) {
	case Behavior::kRoot:
		B_RootUpdate();
		break;
		// 自機を狙う
	case Behavior::kAiming:
		B_AimingUpdate();
		break;
		// 通常射撃
	case Behavior::kNormalShot:
		B_NormalShotUpdate();
		break;
		// ホーミング弾
	case Behavior::kHomingShot:
		B_HomingShotUpdate();
		break;
	}

	// 全ての弾の更新処理
	ArrowsUpdate();
}

void ArrowBoss::Attack()
{
#pragma region 通常弾
	if (behavior_ == Behavior::kNormalShot) {
		// 右に飛んでいく弾
		for (int i = 0; i < 2; i++) {
			// 弾を生成
			HomingArrow* homingArrow = new HomingArrow();
			// 初期化
			homingArrow->Init(models_[0].worldTF);
			// 自機のアドレスを設定
			homingArrow->SetPlayer(player_);
			// 上に弾を撃つ(x,y軸をランダムで少し回転)
			LWP::Math::Vector3 rotate = { -(float)M_PI / 10.0f * (i + 1), 2.0f * (float)M_PI / 3.0f, 0 };
			homingArrow->SetShootingAngle(rotate);
			// ホーミング開始時間
			homingArrow->SetHomingStartFrame(30);

			homingArrows_.push_back(homingArrow);
		}

		// 左に飛んでいく弾
		for (int i = 0; i < 2; i++) {
			// 弾を生成
			HomingArrow* homingArrow = new HomingArrow();
			// 初期化
			homingArrow->Init(models_[0].worldTF);
			// 自機のアドレスを設定
			homingArrow->SetPlayer(player_);
			// 上に弾を撃つ(x,y軸をランダムで少し回転)
			LWP::Math::Vector3 rotate = { -(float)M_PI / 10.0f * (i + 1), -2.0f * (float)M_PI / 3.0f, 0 };
			homingArrow->SetShootingAngle(rotate);
			// ホーミング開始時間
			homingArrow->SetHomingStartFrame(30);

			homingArrows_.push_back(homingArrow);
		}
	}
#pragma endregion

#pragma region ホーミング弾
	else if (behavior_ == Behavior::kHomingShot) {
		// 弾を生成
		HomingArrow* homingArrow = new HomingArrow();
		// 初期化
		homingArrow->Init(models_[0].worldTF);
		// 自機のアドレスを設定
		homingArrow->SetPlayer(player_);
		// 上に弾を撃つ(x,y軸をランダムで少し回転)
		LWP::Math::Vector3 rotate = RandomShootingAngle();
		homingArrow->SetShootingAngle(rotate);

		homingArrows_.push_back(homingArrow);
	}
#pragma endregion 
	isAttack_ = false;
}

void ArrowBoss::ArrowsUpdate() {
	// ホーミング弾の更新処理
	for (HomingArrow* arrow : homingArrows_) {
		arrow->Update();
	}
	homingArrows_.remove_if([](HomingArrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		return false;
		});
}

LWP::Math::Vector3 ArrowBoss::RandomShootingAngle() {
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	// x,y軸をランダムに回転
	std::uniform_real_distribution<float> distributionX((float)M_PI / 2.5f, 1.5f * (float)M_PI / 2.5f);// 45~135度
	std::uniform_real_distribution<float> distributionY(0, 2 * (float)M_PI);				   // 0~360度
	LWP::Math::Vector3 rotate = { -distributionX(randomEngine), distributionY(randomEngine), 0 };

	return rotate;
}

/// Behavior関数ここから↓
void ArrowBoss::B_RootInit() {
	// 攻撃状態解除
	isAttack_ = false;
	// 自機狙い状態のクールタイム
	stunFrame_ = kStunAllFrame;

	// カメラのfovを戻す
	followCamera_->EndFovDirection();
}
void ArrowBoss::B_RootUpdate() {
	// 攻撃の待ち時間
	if (stunFrame_ <= 0) {
		isAiming_ = true;
	}

	// 攻撃範囲内にいるか
	if (CheckAttackRange()) {
		// 攻撃可能状態か
		if (isAiming_) {
			behaviorRequest_ = Behavior::kAiming;
		}
	}
	else {
		// 移動処理
		Move();
		// 自機の方向に身体を向ける
		LockPlayer();
	}

	stunFrame_--;
}

void ArrowBoss::B_AimingInit() {
	// 自機狙い状態開始
	isAiming_ = true;
	attackWaitTime_ = kAimAllFrame;
}
void ArrowBoss::B_AimingUpdate() {
	// 体を自機に向ける
	LockPlayer();

	// 既定の時間を過ぎたら行動開始
	if (attackWaitTime_ <= 0) {
		int randomBehavior = GenerateRandamNum(2, 3);
		behaviorRequest_ = static_cast<Behavior>(randomBehavior);
	}

	attackWaitTime_--;
}

void ArrowBoss::B_NormalShotInit() {
	// 自機狙い状態解除
	isAiming_ = false;
	// 攻撃開始
	isAttack_ = true;
	// 射撃の全体フレーム
	shotFrame_ = kNormalShotAllFrame;
	// 射撃のディレイ
	shotDelay_ = kNormalShotDelayFrame;
	// 射撃回数
	shotCount_ = kMaxNormalShotCount;
}
void ArrowBoss::B_NormalShotUpdate() {
	// 射撃間隔
	if (shotDelay_ <= 0 && shotCount_ >= 1) {
		Attack();
		shotDelay_ = kNormalShotDelayFrame;
		shotCount_--;
	}
	// 既定の時間を過ぎたら攻撃終了
	if (shotFrame_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
	}

	shotDelay_--;
	shotFrame_--;
}

void ArrowBoss::B_HomingShotInit() {
	// 自機狙い状態解除
	isAiming_ = false;
	// 攻撃開始
	isAttack_ = true;
	// 射撃の全体フレーム
	shotFrame_ = kHomingShotAllFrame;
	// 射撃のディレイ
	shotDelay_ = kNormalShotDelayFrame;
	// 射撃回数
	shotCount_ = kMaxHomingShotCount;

	// 視野角を高くする
	followCamera_->StartFovDirection(kEffectFov);
}
void ArrowBoss::B_HomingShotUpdate() {
	// 射撃間隔
	if (shotDelay_ <= 0 && shotCount_ >= 1) {
		Attack();
		shotDelay_ = kHomingShotDelayFrame;
		shotCount_--;
	}
	// 既定の時間を過ぎたら攻撃終了
	if (shotFrame_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
	}

	shotDelay_--;
	shotFrame_--;
}