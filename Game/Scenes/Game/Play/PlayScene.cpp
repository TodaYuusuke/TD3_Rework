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
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();

}

void PlayScene::Update()
{
	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new GameClearScene; };
	}
	player_->Update();
	enemyManager_->Update();
}
