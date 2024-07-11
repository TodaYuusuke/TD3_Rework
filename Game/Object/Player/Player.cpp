#include "Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void Player::Init() {
	model_.LoadCube();
	// 関数ポインタ配列の初期化
	InitStateFunctions();
}

void Player::Update() {

	//*** 操作入力を受け取る ***//

	// 移動、向き操作
	CheckInputMove();

	// 新しい状態になっていた時
	if (reqBehavior_) {
		behavior_ = reqBehavior_.value();
		// 状態の初期化
		(this->*stateInit_[(int)behavior_])();
		reqBehavior_ = std::nullopt;
	}

	// 状態の更新
	(this->*stateUpdate_[(int)behavior_])();

}

void Player::CheckInputMove() {
	// 方向を取得
	lwp::Vector2 direct{ 0.0f,0.0f };
	if (lwp::Keyboard::GetPress(DIK_W)) {
		direct.y += 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_S)) {
		direct.y -= 1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_A)) {
		direct.x -= 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_D)) {
		direct.x += 1.0f;
	}
	direct += LWP::Input::Controller::GetLStick();
	direct = direct.Normalize();

	// そもそも移動入力が無かったらフラグを立てない
	flags_.isInputMove = !(direct.x == 0 && direct.y == 0);

	// 移動入力があった時に方向を更新する
	// 方向をゼロにしない
	if (flags_.isInputMove) {
		// ここで三次元空間に変換
		destinate_.x = direct.x;
		destinate_.z = direct.y;
	}
}

void Player::InitStateFunctions() {
	// 初期関数を格納
	stateInit_[(int)Behavior::Idle] = &Player::InitIdle;
	stateInit_[(int)Behavior::Move] = &Player::InitMove;
	stateInit_[(int)Behavior::Attack] = &Player::InitAttack;


	// 更新関数を格納
	stateUpdate_[(int)Behavior::Idle] = &Player::UpdateIdle;
	stateUpdate_[(int)Behavior::Move] = &Player::UpdateMove;
	stateUpdate_[(int)Behavior::Attack] = &Player::UpdateAttack;
}

#pragma region 状態の初期化

void Player::InitIdle() {
}

void Player::InitMove() {
}

void Player::InitAttack() {
}

#pragma endregion


#pragma region 状態の更新

void Player::UpdateIdle() {
	// 移動入力がされている時
	if (flags_.isInputMove) {
		reqBehavior_ = Behavior::Move;
	}
}

void Player::UpdateMove() {
	// 移動入力されている時
	if (flags_.isInputMove) {
		// 速度を代入する
		velocity_ = destinate_ * Info::GetDeltaTimeF();
	}
	// 入力をやめた時
	else {
		velocity_ = { 0.0f,0.0f,0.0f };
		reqBehavior_ = Behavior::Idle;
	}

	model_.worldTF.translation += velocity_;
}

void Player::UpdateAttack() {
}

#pragma endregion