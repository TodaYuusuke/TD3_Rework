#include "TestScene.h"

#include "../Game/Play/PlayScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void TestScene::Initialize() {
	damageParticle_.model.LoadCube();
	deadParticle_.model.LoadCube();
	spawnParticle_.model.LoadCube();
	accumulateParticle_.model.LoadCube();
}

// 更新
void TestScene::Update()
{
	if (Keyboard::GetTrigger(DIK_1)){
		damageParticle_.Add(16);
	}
	if (Keyboard::GetTrigger(DIK_2)) {
		deadParticle_.Add(16);
	}
	if (Keyboard::GetTrigger(DIK_3)) {
		spawnParticle_.Add(16);
	}
	if (Keyboard::GetTrigger(DIK_4)) {
		accumulateParticle_.Add(16);
	}
}