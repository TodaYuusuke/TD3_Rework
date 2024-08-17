#include "TestScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;


void TestScene::Initialize() {
	// 自機の生成
	player_.Init();

	// 敵
	enemy_.Init();

	// 追従カメラの生成
	followCamera_.Init(player_.GetWorldTransform(), &mainCamera);
}

void TestScene::Update() {
	// 自機
	player_.Update();
	// 追従カメラ
	followCamera_.Update();
}