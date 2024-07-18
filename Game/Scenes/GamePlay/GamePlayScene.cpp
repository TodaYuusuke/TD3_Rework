#include "GamePlayScene.h"

#include "../GameClear/GameClearScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void GamePlayScene::Initialize()
{
	player_ = new Player();
	player_->Init();
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();
}

void GamePlayScene::Update()
{
	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new GameClearScene; };
	}
	player_->Update();
	enemyManager_->Update();
}
