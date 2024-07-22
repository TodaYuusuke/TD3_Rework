#include "EXP.h"

void EXP::Init(lwp::Vector3 position){
	model_.worldTF.translation = position;

	// モデル読み込み
	model_.LoadCube();
	//モデル色変え
	//model_.materials[1].color
	// 場所を設定
	model_.worldTF.scale = lwp::Vector3(kSize_, kSize_, kSize_);
}

void EXP::Update(){

}