#include "Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;

void Player::Init() {
	// モデルを読み込む
	// 一時的にキューブ
	model_.LoadCube();
	// 関数ポインタ配列を初期化
	InitStateFunctions();
	// 設定を初期化
	configs_.Init();
	//当たり判定を初期化
	InitColliders();
}

void Player::Update() {

	// 移動する前の情報を入力
	playerCollider_.capsule.start = model_.worldTF.translation + Vector3(0.0f, 0.1f, 0.0f);

	//*** 操作入力を受け取る ***//

	// 移動、向き操作
	CheckInputMove();

	// 攻撃入力
	CheckInputAttack();


	// デバッグ情報を入力、処理前に確認する
	DebugWindow();

	// 新しい状態になっていた時
	if (reqBehavior_) {
		behavior_ = reqBehavior_.value();
		// 状態の初期化
		(this->*stateInit_[(int)behavior_])();
		reqBehavior_ = std::nullopt;
	}

	// 状態の更新
	(this->*stateUpdate_[(int)behavior_])();

	// 移動した後の情報を入力
	// 参照なので当たり判定に反映される
	playerCollider_.capsule.end = model_.worldTF.translation;
}

void Player::DebugWindow() {
#ifdef DEMO
	// カプセルの描画も当たり判定に追従させる
	playerCollider_.capsule.isActive = playerCollider_.collider.isActive;
	attackCollider_.capsule.isActive = attackCollider_.collider.isActive;

	ImGui::Begin("PlayerWindow");

	// 今の行動を表示
	ImGui::Text("Behavior : "); ImGui::SameLine();
	switch (behavior_) {
	case Player::Behavior::Idle:
		ImGui::Text("Idle");
		break;
	case Player::Behavior::Move:
		ImGui::Text("Move");
		break;
	case Player::Behavior::Attack:
		ImGui::Text("Attack");
		break;
	case Player::Behavior::Moment:
		ImGui::Text("Moment");
		break;
	default:
		break;
	}

	// 設定を表示
	configs_.DebugTree();

	ImGui::End();
#endif // DEMO
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
	// スティックでも入力を取る
	direct += LWP::Input::Controller::GetLStick();
	direct = direct.Normalize();

	// TODO : カメラからの方向に変換する


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

void Player::CheckInputAttack() {
	// 攻撃ボタンが押された時
	if (Input::Keyboard::GetTrigger(DIK_SPACE) ||
		Input::Pad::GetTrigger(0, XINPUT_GAMEPAD_A)) {
		flags_.isInputAttack = true;
	}
	else {
		flags_.isInputAttack = false;
	}
}

void Player::InitStateFunctions() {
	// 初期化関数を格納
	stateInit_[(int)Behavior::Idle] = &Player::InitIdle;
	stateInit_[(int)Behavior::Move] = &Player::InitMove;
	stateInit_[(int)Behavior::Attack] = &Player::InitAttack;
	stateInit_[(int)Behavior::Moment] = &Player::InitMoment;


	// 更新関数を格納
	stateUpdate_[(int)Behavior::Idle] = &Player::UpdateIdle;
	stateUpdate_[(int)Behavior::Move] = &Player::UpdateMove;
	stateUpdate_[(int)Behavior::Attack] = &Player::UpdateAttack;
	stateUpdate_[(int)Behavior::Moment] = &Player::UpdateMoment;
}

void Player::InitColliders() {
	// プレイヤーの当たり判定を初期化
	InitColliderPlayer();
	// 攻撃の当たり判定を初期化
	InitColliderAttack();
}

void Player::InitColliderPlayer() {
	playerCollider_.collider.name = "Player";
	playerCollider_.capsule.radius = configs_.lengthRadius.playerRadius;
}

void Player::InitColliderAttack() {
	attackCollider_.collider.name = "PlayerAttack";
	attackCollider_.collider.isActive = false;
	attackCollider_.capsule.radius = configs_.lengthRadius.attackRadius;
}

#pragma region 状態の初期化

void Player::InitIdle() {
}

void Player::InitMove() {
}

void Player::InitAttack() {
	// 経過時間を初期化
	behaviorTime_ = 0.0f;
	// 速度を設定
	// 移動速度は固定し、デルタタイムは後で計算する
	velocity_ = destinate_ * configs_.moveSpeed.attackSpeed;
	// 攻撃を有効化
	attackCollider_.collider.isActive = true;
	// 攻撃の判定を設定する
	// 徐々に短くしたいから変数に入れるかも
	attackCollider_.capsule.start = model_.worldTF.translation;
	// 攻撃の範囲を設定する
	// コンフィグではなくパラメータを使うことになる
	attackCollider_.capsule.radius = configs_.lengthRadius.attackRadius;
}

void Player::InitMoment() {
	// 攻撃を無効化
	attackCollider_.collider.isActive = false;
	// 経過時間を初期化
	behaviorTime_ = 0.0f;
}

#pragma endregion


#pragma region 状態の更新

void Player::UpdateIdle() {
	// 移動入力がされている時
	if (flags_.isInputMove) {
		reqBehavior_ = Behavior::Move;
	}
	// 攻撃入力がされている時
	if (flags_.isInputAttack) {
		reqBehavior_ = Behavior::Attack;
	}
}

void Player::UpdateMove() {
	// 移動入力されている時
	if (flags_.isInputMove) {
		// 速度を代入する
		velocity_ = destinate_ * configs_.moveSpeed.moveSpeed * Info::GetDeltaTimeF();
	}
	// 入力をやめた時
	else {
		velocity_ = { 0.0f,0.0f,0.0f };
		reqBehavior_ = Behavior::Idle;
	}

	model_.worldTF.translation += velocity_;

	// 攻撃入力がされている時
	// 移動後に攻撃へ移行
	if (flags_.isInputAttack) {
		reqBehavior_ = Behavior::Attack;
	}
}

void Player::UpdateAttack() {
	// 経過時間加算
	behaviorTime_ += Info::GetDeltaTimeF();

	
	model_.worldTF.translation += velocity_ * Info::GetDeltaTimeF();

	// 攻撃を少しづつ短くする
	attackCollider_.capsule.start += velocity_ * 0.75f * Info::GetDeltaTimeF();
	// 攻撃は最初は進行方向に長くしたい
	// 徐々に短くなっていくのがいい
	// 進む速度分伸びる(速度を上げると前に伸びる)
	attackCollider_.capsule.end = model_.worldTF.translation + velocity_ * Info::GetDeltaTimeF();

	// 一定時間経過した時
	if (configs_.progressTime.attackTime <= behaviorTime_) {
		reqBehavior_ = Behavior::Moment;
	}
}

void Player::UpdateMoment() {
	// 経過時間加算
	behaviorTime_ += Info::GetDeltaTimeF();

	// 一定時間経たないと移動入力は反映されない
	if (configs_.progressTime.momentTime * 0.55f <= behaviorTime_) {
		// 移動入力されている時
		if (flags_.isInputMove) {
			// 速度を代入する
			velocity_ = destinate_ * configs_.moveSpeed.momentSpeed * Info::GetDeltaTimeF();
		}
		// 入力をやめた時
		else {
			velocity_ = { 0.0f,0.0f,0.0f };
		}

		model_.worldTF.translation += velocity_;
	}
	// 一定時間経過した時
	if (configs_.progressTime.momentTime <= behaviorTime_) {
		reqBehavior_ = Behavior::Idle;
	}
}

#pragma endregion