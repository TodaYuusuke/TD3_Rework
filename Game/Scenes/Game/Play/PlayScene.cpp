#include "PlayScene.h"

#include "../Clear/GameClearScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void PlayScene::Initialize()
{
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	player_->Init();
}

void PlayScene::Update()
{
	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new GameClearScene; };
	}
	// プレイヤーを更新
	player_->Update();
}
