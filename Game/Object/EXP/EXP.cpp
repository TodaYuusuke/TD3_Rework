#include "EXP.h"

void EXP::Init(lwp::Vector3 position){
	model_.worldTF.translation = position;

	// モデル読み込み
	model_.LoadShortPath("Exp/Exp.obj");
	// 場所を設定
	model_.worldTF.scale = lwp::Vector3(kSize_, kSize_, kSize_);
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
}