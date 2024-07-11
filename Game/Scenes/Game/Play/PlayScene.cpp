#include "PlayScene.h"

#include "../Clear/GameClearScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void PlayScene::Initialize()
{
	player_ = new Player();
	player_->Init();
	normalEnemy_ = new NormalEnemy();
	normalEnemy_->Init();
	normalEnemy_->SetTarget(player_);
}

void PlayScene::Update()
{
	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new GameClearScene; };
	}
	player_->Update();
	normalEnemy_->Update();
}
