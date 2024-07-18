#include "GameClearScene.h"

#include "../Title/TitleScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void GameClearScene::Initialize()
{
}

void GameClearScene::Update()
{
	if (Keyboard::GetTrigger(DIK_N)){
		nextSceneFunction = []() { return new TitleScene; };
	}
}
