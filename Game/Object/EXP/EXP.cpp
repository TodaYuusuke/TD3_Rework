#include "EXP.h"
#include "Game/Object/Player/Player.h"

void EXP::Init(lwp::Vector3 position){
	model_.worldTF.translation = position;

	// モデル読み込み
	model_.LoadShortPath("Exp/Exp.obj");
	model_.materials[1].color = 0x7CFC00FF;
	// 場所を設定
	model_.worldTF.scale = lwp::Vector3(kSize_, kSize_, kSize_);
	// 死んでない
	isDead_ = false;
	isDisable_ = true;
	InitCollider();
}

void EXP::Update(){
	// モデルの回転
	Rotate_.y += rotateSpeed_;
	if (Rotate_.y < -3.14f)
	{
		Rotate_.y += 3.14f;
	}
	else if (3.14f < Rotate_.y)
	{
		Rotate_.y -= 3.14f;
	}

	model_.worldTF.rotation = lwp::Quaternion::ConvertEuler(Rotate_);

	// 発生してから数秒は判定を付与しない
	if (isDisable_)
	{
		if (kToEnableTime_ < time_)
		{
			time_ = 0.0f;
			collider.collider.isActive = true;
			isDisable_ = false;

			return;
		}
		// 時間を加算
		time_ += lwp::GetDefaultDeltaTimeF();
	}
	// アニメーションが開始していたら
	else if (isStert)
	{
		if (kAnimationTime_ <= time_)
		{
			isDead_ = true;
			model_.isActive = false;
			collider.collider.isActive = false;
			return;
		}
		// 位置を参照
		lwp::Vector3 move = player_->GetWorldPosition() - model_.worldTF.translation;
		float t = std::clamp(time_ / kAnimationTime_, 0.0f, 1.0f);
		// タメがあるような移動
		model_.worldTF.translation = model_.worldTF.translation + LWP::Utility::Easing::InExpo(t) * move;
		// 1.0f ~ 0.0f に再計算
		//t = (1.0f - LWP::Utility::Easing::InExpo(t));
		// 一定以下なら 0.0f にする(描画しない)
		//t = 0.2 < t ? t : 0.0f;
		// このイージングでないとよく見えなかった
		model_.worldTF.scale = lwp::Vector3(kSize_, kSize_, kSize_) * t;
		// 時間を加算
		time_ += lwp::GetDefaultDeltaTimeF();
	}
}

void EXP::InitCollider()
{
	collider.collider.SetBroadShape(lwp::Collider::Sphere());
	collider.collider.SetFollowTarget(&model_.worldTF);
	collider.collider.name = "EXP";
	collider.sphere.radius = 1.0f;
	// ヒットしたときの処理を設定
	//collider.collider.enterLambda = [this](lwp::Collider::Collider* hitTarget) { EnterEXP(hitTarget); };
	collider.collider.stayLambda = [this](lwp::Collider::Collider* hitTarget) { EnterEXP(hitTarget); };
}

void EXP::EnterEXP(LWP::Object::Collider::Collider* hitTarget)
{
	//取得可能時間になっていないならスキップ
	if (isDisable_) {
		return;
	}
	if (hitTarget->name == "EXPPull") {

		collider.collider.isActive = false;

		isStert = true;
		// 取得時に回転スピードを上げる
		rotateSpeed_ = kRotateGetSpeed_;
	}
}
