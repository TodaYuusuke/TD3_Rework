#include "GamePlayScene.h"

#include "../GameClear/GameClearScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

GamePlayScene::GamePlayScene() : enemyManager_(&player_) {}

void GamePlayScene::Initialize() {
	levelData.LoadShortPath("Field.json");

	// 一時的にカメラの位置を調整
	mainCamera.transform.translation.z = -25.0f;
	mainCamera.transform.translation.y = 4.0f;

	// 太陽初期化
	sun_.transform.translation.y = 30.0f;
	sun_.intensity = 2.0f;
	sun_.radius = 105.0f;
	sun_.decay = 0.58f;

	player_.Init(&mainCamera);
	enemyManager_.Init();
}

void GamePlayScene::Update()
{
	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new GameClearScene; };
	}
	player_.Update();
	enemyManager_.Update();
}
