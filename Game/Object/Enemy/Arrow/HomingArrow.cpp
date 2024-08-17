#include "HomingArrow.h"
#include "../../Player/Player.h"

HomingArrow::HomingArrow(/*std::function<void(LWP::Math::Vector3)> func*/) {
	//summonContrail_ = func;
}


HomingArrow::~HomingArrow() {

}

void HomingArrow::Init(lwp::TransformQuat transform) {
	// モデルの作成
	model_.LoadCube();
	model_.worldTF.translation = transform.translation;
	model_.worldTF.rotation = transform.rotation;
	model_.materials[1].enableLighting = true;
	model_.worldTF.scale = { 1.0f,1.0f,1.0f };

	// 当たり判定を設定
	//aabb_ = new LWP::Object::Collider::AABB();
	/*aabb_.CreateFromPrimitive(&model_);
	aabb_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	aabb_.mask.SetHitFrag(MaskLayer::Player | MaskLayer::Layer3);
	aabb_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		if (!(data.state == OnCollisionState::None) &&
			data.hit &&
			((data.hit->mask.GetBelongFrag() & MaskLayer::Player) ||
				(data.hit->mask.GetBelongFrag() & MaskLayer::Layer3)))
		{
			Death();
		}
		});
	aabb_.isActive = true;*/

	// ホーミング開始時間(何も設定されていない場合はデフォルトの値を入れる)
	if (homingStartFrame_ == 0) {
		homingStartFrame_ = kHomingStartFrame;
	}
	// ホーミングする時間
	homingFrame_ = kHomingEndFrame;
	// ホーミング機能をoff
	isHoming_ = false;
	// ホーミング精度
	homingAccuracy_ = kLongDistHomingAccuracy;
	// 生存フラグ
	isAlive_ = true;

	// イージング開始時のベクトル
	LWP::Math::Vector3 velocity = { 0,0,1 };
	startEaseVel_ = (velocity * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation));
	startEaseVel_ = startEaseVel_.Normalize() * kSpeed;

	summonParticle_.model.LoadCube();
}

void HomingArrow::Update() {
	// 移動処理
	Attack();

	// 弾の消える条件
	if (deadTimer_ >= kLifeTime || model_.worldTF.translation.y <= 0)
	{
		Death();
	}

	if (static_cast<int>(deadTimer_) % 1 == 0) {
		summonParticle_.model.worldTF.translation = model_.worldTF.translation;
		summonParticle_.Add(1);
	}

	// 寿命を進める
	deadTimer_++;
}

void HomingArrow::Attack() {
	// ホーミング開始時間の確認
	if (homingFrame_ <= kHomingEndFrame - homingStartFrame_) {
		isHoming_ = true;
	}
	// ホーミング終了時間の確認
	if (homingFrame_ <= 0) {
		isHoming_ = false;
	}

	// ホーミングの処理
	if (isHoming_) {
		// 距離によってホーミング精度を変える
		ChangeHomingAccuracy();
		// ホーミングの処理
		HomingUpdate();

		// 弾の向きを自機に向ける時は速度を0にする
		if (homingFrame_ >= kHomingEndFrame - homingStartFrame_ - kNormal2HomingFrame) {
			velocity_ = { 0,0,0 };
		}
	}
	// ホーミングしないときの処理
	else {
		// 方向ベクトル(モデルから見て正面に向かって移動)
		LWP::Math::Vector3 velocity = { 0,0,1 };
		// 回転行列の生成(発射角を含み計算)
		LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
		velocity = velocity * rotateMatrix;
		velocity_ = velocity.Normalize() * kSpeed;

		if (deadTimer_ <= 0) {
			startEaseVel_ = velocity_;
		}

		// 発射された弾はだんだん遅くなり止まる
		if (homingFrame_ >= kHomingEndFrame - homingStartFrame_) {
			LWP::Math::Vector3 endVel = { 0,0,0 };
			velocity_ = LWP::Math::Vector3::Slerp(startEaseVel_, endVel, deadTimer_ / homingStartFrame_);
		}
	}

	// 加算
	model_.worldTF.translation += velocity_ * LWP::Info::GetDeltaTimeF();

	// ホーミング開始時間を進める
	homingFrame_--;
	// 0よりも低くならないようにする
	homingFrame_ = std::clamp<float>(homingFrame_, 0, kHomingEndFrame);
}

void HomingArrow::Death() {
	isAlive_ = false;
}

void HomingArrow::HomingUpdate() {
	// 自機座標より少し上をホーミング
	LWP::Math::Vector3 playerPos = player_->GetWorldPosition();
	playerPos.y += 0.5f;
	// 弾から自機への方向ベクトルを算出
	LWP::Math::Vector3 toPlayer = playerPos - model_.worldTF.translation;
	// 正規化
	toPlayer = toPlayer.Normalize();
	velocity_ = velocity_.Normalize();

	// 球面線形保管により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = LWP::Math::Vector3::Slerp(velocity_, toPlayer, homingAccuracy_);
	velocity_ *= kSpeed;

#pragma region 弾の角度
	// 速度自体は変わってほしくないので計算時のみvelocityを正規化する
	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,0.0f,1.0f }, velocity_.Normalize()).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,0.0f,1.0f }, velocity_.Normalize());

	//行きたい方向のQuaternionの作成
	model_.worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));
#pragma endregion
}

void HomingArrow::ChangeHomingAccuracy() {
	// 自機との距離
	float distance = (model_.worldTF.translation - player_->GetWorldPosition()).Length();
	if (distance <= kHomingStrengthRange) {
		homingAccuracy_ = kShortDistHomingAccuracy;
	}
}