#include "Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;

void Player::Init(LWP::Object::Camera* pCamera) {
	// モデルを読み込む
	// 一時的にキューブ
	model_.LoadCube();
	// 関数ポインタ配列を初期化
	InitStateFunctions();

	// 設定を初期化
	parameter_.Init();
	//当たり判定を初期化
	InitColliders();

	// フォローカメラを初期化
	followCamera_.Init(&model_.worldTF, pCamera);

}

void Player::Update() {

	// 死んでいる時
	if (flags_.isDead) {
		// デバッグ情報を入力、処理前に確認する
		DebugWindow();
		// デバッグ表示用
		model_.isActive = !model_.isActive;
		return;
	}

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

	// カメラの位置を更新
	followCamera_.Update();

	// フラグを元に戻す処理
	// 無敵時間中の時
	if (0.0f < times_.invincibleTime) {
		times_.invincibleTime -= Info::GetDeltaTimeF();
		// 無敵時間が切れた時
		if (times_.invincibleTime <= 0.0f) {
			// 判定を取るようにする
			playerCollider_.collider.isActive = true;
		}
	}
}

void Player::DecreaseHP() {
	// 体力を減らす
	parameter_.countGage.hpCount--;
	// 体力が 0 以下になった時
	if (parameter_.countGage.hpCount <= 0) {
		// 死ぬ
		flags_.isDead = true;
	}
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
	stateInit_[(int)Behavior::Damage] = &Player::InitDamage;


	// 更新関数を格納
	stateUpdate_[(int)Behavior::Idle] = &Player::UpdateIdle;
	stateUpdate_[(int)Behavior::Move] = &Player::UpdateMove;
	stateUpdate_[(int)Behavior::Attack] = &Player::UpdateAttack;
	stateUpdate_[(int)Behavior::Moment] = &Player::UpdateMoment;
	stateUpdate_[(int)Behavior::Damage] = &Player::UpdateDamage;
}

#pragma region 当たり判定関数

void Player::InitColliders() {
	// プレイヤーの当たり判定を初期化
	InitColliderPlayer();
	// 攻撃の当たり判定を初期化
	InitColliderAttack();
}

void Player::InitColliderPlayer() {
	playerCollider_.collider.name = "Player";
	playerCollider_.capsule.radius = parameter_.lengthRadius.playerRadius;

	// ヒットしたときの処理を設定
	playerCollider_.collider.enterLambda = [this](Collider::Collider* hitTarget) { EnterPlayer(hitTarget); };
	playerCollider_.collider.stayLambda = [this](Collider::Collider* hitTarget) { StayPlayer(hitTarget); };
	playerCollider_.collider.exitLambda = [this](Collider::Collider* hitTarget) { ExitPlayer(hitTarget); };
}

void Player::InitColliderAttack() {
	attackCollider_.collider.name = "PlayerAttack";
	attackCollider_.collider.isActive = false;
	attackCollider_.capsule.radius = parameter_.lengthRadius.attackRadius;

	// ヒットしたときの処理を設定
	attackCollider_.collider.enterLambda = [this](Collider::Collider* hitTarget) { EnterAttack(hitTarget); };
	attackCollider_.collider.stayLambda = [this](Collider::Collider* hitTarget) { StayAttack(hitTarget); };
	attackCollider_.collider.exitLambda = [this](Collider::Collider* hitTarget) { ExitAttack(hitTarget); };
}

#pragma region ヒット時処理

void Player::EnterPlayer(LWP::Object::Collider::Collider* hitTarget) {
	hitTarget;
}

void Player::StayPlayer(LWP::Object::Collider::Collider* hitTarget) {
	hitTarget;
}

void Player::ExitPlayer(LWP::Object::Collider::Collider* hitTarget) {
	hitTarget;
}

void Player::EnterAttack(LWP::Object::Collider::Collider* hitTarget) {
	hitTarget;
}

void Player::StayAttack(LWP::Object::Collider::Collider* hitTarget) {
	hitTarget;
}

void Player::ExitAttack(LWP::Object::Collider::Collider* hitTarget) {
	hitTarget;
}
// ヒット時処理
#pragma endregion

// 当たり判定関数
#pragma endregion

#pragma region 状態の初期化

void Player::InitIdle() {
	// 攻撃可能回数を元に戻す
	parameter_.countGage.attackCount = parameter_.countGage.MaxAttackCount;
}

void Player::InitMove() {
}

void Player::InitAttack() {
	// 経過時間を初期化
	times_.behaviorTime = 0.0f;
	// 速度を設定
	// 移動量は固定し、デルタタイムは後で計算する
	movement_ = parameter_.moveSpeed.attackSpeed;
	// カメラの正面ベクトルへ修正
	velocity_ = destinate_ * followCamera_.GetTransformQuat().rotation;
	// y 情報を消す
	// 正規化した情報に移動量を掛ける
	velocity_ = Vector3(velocity_.x, 0.0f, velocity_.z).Normalize() * movement_;

	// 攻撃を有効化
	attackCollider_.collider.isActive = true;
	// 攻撃の判定を設定する
	// 徐々に短くしたいから変数に入れるかも
	attackCollider_.capsule.start = model_.worldTF.translation;
	// 攻撃の範囲を設定する
	// コンフィグではなくパラメータを使うことになる
	attackCollider_.capsule.radius = parameter_.lengthRadius.attackRadius;
	// 攻撃回数を 1 減らす
	parameter_.countGage.attackCount--;
}

void Player::InitMoment() {
	// 経過時間を初期化
	times_.behaviorTime = 0.0f;
	// 攻撃を無効化
	attackCollider_.collider.isActive = false;
}

void Player::InitDamage() {
	// 体力を減らす
	DecreaseHP();
	// 経過時間を初期化
	times_.behaviorTime = 0.0f;
	// 無敵時間を設定する
	times_.invincibleTime = parameter_.progressTime.damageInvincibleTime;
	// 当たり判定を消す
	playerCollider_.collider.isActive = false;
}

// 状態の初期化
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
		// 速度量を代入する
		movement_ = parameter_.moveSpeed.moveSpeed * Info::GetDeltaTimeF();
		// カメラの正面ベクトルへ修正
		velocity_ = destinate_ * followCamera_.GetTransformQuat().rotation;
		// y 情報を消す
		// 正規化した情報に移動量を掛ける
		velocity_ = Vector3(velocity_.x, 0.0f, velocity_.z).Normalize() * movement_;
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
	times_.behaviorTime += Info::GetDeltaTimeF();


	model_.worldTF.translation += velocity_ * Info::GetDeltaTimeF();

	// 攻撃を少しづつ短くする
	attackCollider_.capsule.start += velocity_ * parameter_.collectionRatio.attackReduceStart * Info::GetDeltaTimeF();
	// 攻撃は最初は進行方向に長くしたい
	// 徐々に短くなっていくのがいい
	// 進む速度分伸びる(速度を上げると前に伸びる)
	attackCollider_.capsule.end = model_.worldTF.translation + velocity_ * parameter_.collectionRatio.attackExtendEnd * Info::GetDeltaTimeF();

	// 一定時間経過した時
	if (parameter_.progressTime.attackTime <= times_.behaviorTime) {
		reqBehavior_ = Behavior::Moment;
	}
}

void Player::UpdateMoment() {
	// 経過時間加算
	times_.behaviorTime += Info::GetDeltaTimeF();

	// 一定時間経たないと移動入力は反映されない
	if (parameter_.progressTime.momentTime * parameter_.collectionRatio.momentStuckRatio <= times_.behaviorTime) {
		// 移動入力されている時
		if (flags_.isInputMove) {
			// 速度を代入する
			velocity_ = destinate_ * parameter_.moveSpeed.momentSpeed * Info::GetDeltaTimeF();
		}
		// 入力をやめた時
		else {
			velocity_ = { 0.0f,0.0f,0.0f };
		}

		model_.worldTF.translation += velocity_;
	}
	// 一定時間経過した時
	if (parameter_.progressTime.momentTime <= times_.behaviorTime) {
		reqBehavior_ = Behavior::Idle;
	}

	// 攻撃入力されていて、まだ攻撃できるとき
	if (flags_.isInputAttack && 0 < parameter_.countGage.attackCount) {
		reqBehavior_ = Behavior::Attack;
	}
}

void Player::UpdateDamage() {
	// 経過時間加算
	times_.behaviorTime += Info::GetDeltaTimeF();

	// 一定時間経過した時
	if (parameter_.progressTime.damageTime <= times_.behaviorTime) {
		reqBehavior_ = Behavior::Idle;
	}
}

// 状態の更新
#pragma endregion

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
	case Player::Behavior::Damage:
		ImGui::Text("Damage");
		break;
	default:
		ImGui::Text("%d", (int)behavior_);
		break;
	}
	ImGui::Separator();

	// 今の体力を表示
	ImGui::Text("HpCount(Now/Max)");
	ImGui::Text("%d / %d", parameter_.countGage.hpCount, parameter_.countGage.MaxHpCount);
	// 体力やフラグをリセットする
	if (ImGui::Button("ResetHP")) {
		parameter_.countGage.hpCount = parameter_.countGage.MaxHpCount;
		flags_.isDead = false;
	}

	ImGui::Separator();

	if (ImGui::Button("Damage")) {
		reqBehavior_ = Behavior::Damage;
	}
	// 無敵時間か表示
	ImGui::Text("Invincible : %s", playerCollider_.collider.isActive ? "FALSE" : "TRUE");
	ImGui::Separator();

	// 攻撃回数を表示
	ImGui::Text("AttackCount(Now/Max)");
	ImGui::Text("%d / %d", parameter_.countGage.attackCount, parameter_.countGage.MaxAttackCount);
	ImGui::Separator();

	// 設定を表示
	parameter_.DebugTree();

	ImGui::End();
#endif // DEMO
}