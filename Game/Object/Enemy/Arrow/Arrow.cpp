#include "Arrow.h"

void Arrow::Init(lwp::TransformQuat transform)
{
	// モデルの作成
	model_.LoadCube();
	model_.worldTF.translation = transform.translation;
	model_.worldTF.rotation = transform.rotation;
	model_.materials[1].enableLighting = true;
	isAlive_ = true;
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

void Arrow::Attack() {
	const LWP::Math::Vector3 vel = { 0,0,1 };
	// 座標を決定
	model_.worldTF.translation += vel * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
}

void Arrow::Death() {
	isAlive_ = false;
}