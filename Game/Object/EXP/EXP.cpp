#include "EXP.h"

void EXP::Init(lwp::Vector3 position){
	model_.worldTF.translation = position;

	// モデル読み込み
	model_.LoadShortPath("Exp/Exp.obj");
	// 場所を設定
	model_.worldTF.scale = lwp::Vector3(kSize_, kSize_, kSize_);
}

void EXP::Update(){
	model_.worldTF.translation = { 5.0f,0.0f,0.0f };
}