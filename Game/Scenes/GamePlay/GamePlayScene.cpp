#include "GamePlayScene.h"

#include "../GameClear/GameClearScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

GamePlayScene::GamePlayScene() : enemyManager_(&player_, &expManager_) {}

void GamePlayScene::Initialize() {
	levelData.LoadShortPath("Field.json");

	// 一時的にカメラの位置を調整
	mainCamera.transform.translation.z = -25.0f;
	mainCamera.transform.translation.y = 4.0f;

	// 追従カメラの初期化
	followCamera_.Init(&mainCamera);
	// タイマー
	gameTimer_ = GameTimer::GetInstance();
	gameTimer_->Initialize();

	// 秒数のセット
	gameTimer_->Reset(180);
	// ゲームが始まってから計測開始
	gameTimer_->Start();

	// 太陽初期化
	sun_.transform.translation.y = 30.0f;
	sun_.intensity = 2.0f;
	sun_.radius = 105.0f;
	sun_.decay = 0.58f;

	// 自機の初期化
	player_.Init(&followCamera_);
	followCamera_.SetTarget(player_.GetWorldTransform());

	// enemyManagerの初期化
	enemyManager_.Init(&followCamera_);
	
	upgradeManager_.Init(player_.GetPlayerParameter());
	expManager_.Init();
}

void GamePlayScene::Update()
{
	upgradeManager_.Update();

	if (upgradeManager_.GetUpgradeFlag()) {
		return;
	}

	gameTimer_->Update();

#pragma region
	// タイマーのカウントが終了したとき
	if (gameTimer_->isEnd_) {
		//だんだん音が下がる
		/*BGMt = (std::min)(BGMt + 0.002f, 1.0f);
		BGMvolume = Lerp(BGMvolume, 0.0f, BGMt);
		BGM->SetVolume(BGMvolume);*/
		// プレイヤーが生きているとき
		if (player_.GetPlayerParameter().flags_.isDead == false) {
			// クリアしたときの処理
			// 何か演出を出す

			if (player_.ClearAnime()) {
				// タイマーを消す
				gameTimer_->isActive_ = false;
				// 描画を消す
				gameTimer_->Update();


				// シーン遷移演出開始
				/*sceneTransition_->Start();
				if (sceneTransition_->GetIsSceneChange()) {
					BGM->Stop();
					nextSceneFunction = []() { return new ClearScene; };
				}*/
			}
			return;
		}
		// プレイヤーが死んでいた時
		else
		{

			// ゲームオーバーしたときの処理
			if (player_.GameOverAnime()) {
				// タイマーを消す
				gameTimer_->isActive_ = false;

				// 描画を消す
				gameTimer_->Update();

				// シーン遷移演出開始
				//sceneTransition_->Start();

				/*if (sceneTransition_->GetIsSceneChange()) {
					BGM->Stop();
					nextSceneFunction = []() { return new GameOverScene; };
				}*/
			}
			return;
		}
	}
#pragma endregion ゲームタイマーが止まった時

	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new GameClearScene; };
	}
	player_.Update();
	enemyManager_.Update();
	expManager_.Update();

	// カメラの位置を更新
	followCamera_.Update();
}
