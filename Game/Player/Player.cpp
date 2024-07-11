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
	ImGui::Begin("Player");
	ImGui::DragFloat3("Translate",&model_.worldTF.translation.x);
	ImGui::End();
}