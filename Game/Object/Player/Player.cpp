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
	model_.worldTF.translation.z += 0.01f;
}