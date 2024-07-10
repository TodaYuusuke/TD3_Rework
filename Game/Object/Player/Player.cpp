#include "Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void Player::Init() {
	model_.LoadCube();
}

void Player::Update() {
	// すぐに消す
	ImGui::Begin("player");

	model_.worldTF.DebugGUI();

	ImGui::End();
}