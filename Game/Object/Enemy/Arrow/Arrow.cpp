#include "Arrow.h"

void Arrow::Init(lwp::TransformQuat transform)
{
	// モデルの作成
	//model_.LoadFile("Arrow/Arrow.obj");
	model_.LoadCube();
	model_.worldTF.translation = transform.translation;
	model_.worldTF.rotation = transform.rotation;
	model_.materials[1].enableLighting = true;

	// 当たり判定を設定
	//aabb_ = new LWP::Object::Collider::AABB();
	//aabb_.CreateFromPrimitive(&model_);
	//aabb_.mask.SetBelongFrag(GameMask::EnemyAttack());
	//aabb_.mask.SetHitFrag(GameMask::Player() | GameMask::Weapon());
	//aabb_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {
	//	data;
	//	if (!(data.state == OnCollisionState::None) &&
	//		data.hit &&
	//		(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
	//	{
	//		Death();
	//	}
	//	});

	//aabb_.isActive = true;
}

void Arrow::Update()
{
	// 移動処理
	Attack();

	// 弾が生存時間を越えていたら死ぬ
	if (deadTimer_ >= kLifeTime)
	{
		Death();
	}
	// 寿命を進める
	deadTimer_++;
}

void Arrow::Attack()
{
	//// 弾が向いている方向に動く処理
	//LWP::Math::Vector3 velocity = { 0,0,1 };
	//LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
	//velocity = velocity * rotateMatrix;
	//velocity_ = velocity.Normalize();
	//velocity_ *= kNormalSpeed;

	// 自機に向かうベクトル
	LWP::Math::Vector3 targetVel = (player_->GetWorldPosition() - model_.worldTF.translation).Normalize();
	// 狙う対象に身体を向ける
	lwp::Vector3 sub = targetVel - model_.worldTF.translation;

	//プレイヤーの現在の向き
	sub = sub.Normalize();

	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,0.0f,1.0f }, sub).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

	//行きたい方向のQuaternionの作成
	model_.worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));



	// 加算
	//model_.worldTF.translation += velocity_ * LWP::Info::GetDeltaTime();
}

void Arrow::Death() {
	
}