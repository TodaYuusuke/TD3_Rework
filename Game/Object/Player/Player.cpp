#include "Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;

void Player::Init(FollowCamera* followCamera) {
	// モデルを読み込む
	// 一時的にキューブ
	model_.LoadShortPath("player/player.obj");
	// 関数ポインタ配列を初期化
	InitStateFunctions();

	// 設定を初期化
	parameter_.Init();
	//当たり判定を初期化
	InitColliders();

	// 追従カメラのアドレスを設定
	followCamera_ = followCamera;
	// 武器を作成
	weapon_.reset(new Weapon);
	weapon_->Initialize();
	weapon_->SetParent(&model_.worldTF);
}

void Player::Update() {

	// 死んでいる時
	if (parameter_.flags_.isDead) {
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

	weapon_->Update();

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

	// フラグを元に戻す処理
	// 無敵時間中の時
	if (0.0f < times_.invincibleTime) {
		times_.invincibleTime -= Info::GetDeltaTimeF();
		model_.isActive = !model_.isActive;
		// 無敵時間が切れた時
		if (times_.invincibleTime <= 0.0f) {
			// 判定を取るようにする
			model_.isActive = true;
			playerCollider_.collider.isActive = true;
		}
	}

	parameter_.Update();
}

bool Player::ClearAnime()
{
	return true;
}

bool Player::GameOverAnime()
{
	return true;
}

void Player::DecreaseHP() {
	// 体力を減らす
	parameter_.status.countGage.hpCount--;

	// 体力が 0 以下になった時
	if (parameter_.status.countGage.hpCount <= 0) {
		// 死ぬ
		parameter_.flags_.isDead = true;
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

	// ここで三次元空間に変換
	destinate_.x = direct.x;
	destinate_.z = direct.y;

	//回転行列を作る
	lwp::Matrix4x4 rotateMatrix = lwp::Matrix4x4::CreateRotateXYZMatrix(followCamera_->pCamera_->transform.rotation);
	//移動ベクトルをカメラの角度だけ回転
	destinate_ = TransformNormal(destinate_, rotateMatrix);
	destinate_.y = 0;
#pragma region
	//移動ベクトルをカメラの角度だけ回転
	//ロックオン座標
	lookPoint = destinate_;

	//プレイヤーの現在の向き
	lookPoint = lookPoint.Normalize();

	Vector3 cross = Vector3::Cross({ 0.0f,0.0f,1.0f }, lookPoint).Normalize();
	float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, lookPoint);

	//行きたい方向のQuaternionの作成
	model_.worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));
#pragma endregion プレイヤーの移動方向を向く

	// そもそも移動入力が無かったらフラグを立てない
	parameter_.flags_.isInputMove = !(direct.x == 0 && direct.y == 0);
}

void Player::CheckInputAttack() {
	// 攻撃ボタンが押された時
	if (Input::Keyboard::GetTrigger(DIK_SPACE) ||
		Input::Pad::GetTrigger(XINPUT_GAMEPAD_A, 0)) {
		parameter_.flags_.isInputAttack = true;
	}
	else {
		parameter_.flags_.isInputAttack = false;
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
	playerCollider_.capsule.radius = parameter_.status.lengthRadius.playerRadius;

	// ヒットしたときの処理を設定
	playerCollider_.collider.enterLambda = [this](Collider::Collider* hitTarget) { EnterPlayer(hitTarget); };
	playerCollider_.collider.stayLambda = [this](Collider::Collider* hitTarget) { StayPlayer(hitTarget); };
	playerCollider_.collider.exitLambda = [this](Collider::Collider* hitTarget) { ExitPlayer(hitTarget); };
}

void Player::InitColliderAttack() {
	attackCollider_.collider.name = "PlayerAttack";
	attackCollider_.collider.isActive = false;
	attackCollider_.capsule.radius = parameter_.status.lengthRadius.attackRadius;

	// ヒットしたときの処理を設定
	attackCollider_.collider.enterLambda = [this](Collider::Collider* hitTarget) { EnterAttack(hitTarget); };
	attackCollider_.collider.stayLambda = [this](Collider::Collider* hitTarget) { StayAttack(hitTarget); };
	attackCollider_.collider.exitLambda = [this](Collider::Collider* hitTarget) { ExitAttack(hitTarget); };
}

#pragma region ヒット時処理

void Player::EnterPlayer(LWP::Object::Collider::Collider* hitTarget) {
	if (hitTarget->name == "Enemy") {
		parameter_.flags_.isDamage = true;
	}
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
	parameter_.status.countGage.attackCount = parameter_.status.countGage.MaxAttackCount;
}

void Player::InitMove() {
}

void Player::InitAttack() {
	// 経過時間を初期化
	times_.behaviorTime = 0.0f;
	// 速度を設定
	// 移動速度は固定し、デルタタイムは後で計算する
	velocity_ = destinate_ * parameter_.status.moveSpeed.attackSpeed;
	// 攻撃を有効化
	attackCollider_.collider.isActive = true;
	// 攻撃の判定を設定する
	// 徐々に短くしたいから変数に入れるかも
	attackCollider_.capsule.start = model_.worldTF.translation;
	// 攻撃の範囲を設定する
	// コンフィグではなくパラメータを使うことになる
	attackCollider_.capsule.radius = parameter_.status.lengthRadius.attackRadius;
	// 攻撃回数を 1 減らす
	parameter_.status.countGage.attackCount--;
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
	times_.invincibleTime = parameter_.status.progressTime.damageInvincibleTime;
	// 当たり判定を消す
	playerCollider_.collider.isActive = false;
}

// 状態の初期化
#pragma endregion


#pragma region 状態の更新

void Player::UpdateIdle() {
	// 移動入力がされている時
	if (parameter_.flags_.isInputMove) {
		reqBehavior_ = Behavior::Move;
	}
	// 攻撃入力がされている時
	if (parameter_.flags_.isInputAttack) {
		reqBehavior_ = Behavior::Attack;
	}
}

void Player::UpdateMove() {
	// 移動入力されている時
	if (parameter_.flags_.isInputMove) {
		// 速度を代入する
		velocity_ = destinate_ * parameter_.status.moveSpeed.moveSpeed * Info::GetDeltaTimeF();
	}
	// 入力をやめた時
	else {
		velocity_ = { 0.0f,0.0f,0.0f };
		reqBehavior_ = Behavior::Idle;
	}

	model_.worldTF.translation += velocity_;

	// 攻撃入力がされている時
	// 移動後に攻撃へ移行
	if (parameter_.flags_.isInputAttack) {
		reqBehavior_ = Behavior::Attack;
	}
	if (parameter_.flags_.isDamage) {
		reqBehavior_ = Behavior::Damage;
	}
}

void Player::UpdateAttack() {
	// 経過時間加算
	times_.behaviorTime += Info::GetDeltaTimeF();


	model_.worldTF.translation += velocity_ * Info::GetDeltaTimeF();

	// 攻撃を少しづつ短くする
	attackCollider_.capsule.start += velocity_ * parameter_.status.collectionRatio.attackReduceStart * Info::GetDeltaTimeF();
	// 攻撃は最初は進行方向に長くしたい
	// 徐々に短くなっていくのがいい
	// 進む速度分伸びる(速度を上げると前に伸びる)
	attackCollider_.capsule.end = model_.worldTF.translation + velocity_ * parameter_.status.collectionRatio.attackExtendEnd * Info::GetDeltaTimeF();

	// 一定時間経過した時
	if (parameter_.status.progressTime.attackTime <= times_.behaviorTime) {
		reqBehavior_ = Behavior::Moment;
	}
}

void Player::UpdateMoment() {
	// 経過時間加算
	times_.behaviorTime += Info::GetDeltaTimeF();

	// 一定時間経たないと移動入力は反映されない
	if (parameter_.status.progressTime.momentTime * parameter_.status.collectionRatio.momentStuckRatio <= times_.behaviorTime) {
		// 移動入力されている時
		if (parameter_.flags_.isInputMove) {
			// 速度を代入する
			velocity_ = destinate_ * parameter_.status.moveSpeed.momentSpeed * Info::GetDeltaTimeF();
		}
		// 入力をやめた時
		else {
			velocity_ = { 0.0f,0.0f,0.0f };
		}

		model_.worldTF.translation += velocity_;
	}
	// 一定時間経過した時
	if (parameter_.status.progressTime.momentTime <= times_.behaviorTime) {
		reqBehavior_ = Behavior::Idle;
	}

	// 攻撃入力されていて、まだ攻撃できるとき
	if (parameter_.flags_.isInputAttack && 0 < parameter_.status.countGage.attackCount) {
		reqBehavior_ = Behavior::Attack;
	}
}

void Player::UpdateDamage() {
	// 経過時間加算
	times_.behaviorTime += Info::GetDeltaTimeF();

	// 一定時間経過した時
	if (parameter_.status.progressTime.damageTime <= times_.behaviorTime) {
		reqBehavior_ = Behavior::Idle;
		parameter_.flags_.isDamage = false;
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
	ImGui::Text("%d / %d", parameter_.status.countGage.hpCount, parameter_.status.countGage.MaxHpCount);
	// 体力やフラグをリセットする
	if (ImGui::Button("ResetHP")) {
		parameter_.status.countGage.hpCount = parameter_.status.countGage.MaxHpCount;
		parameter_.flags_.isDead = false;
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
	ImGui::Text("%d / %d", parameter_.status.countGage.attackCount, parameter_.status.countGage.MaxAttackCount);
	ImGui::Separator();

	// 設定を表示
	parameter_.DebugTree();

	ImGui::End();
#endif // DEMO
}

lwp::Vector3 TransformNormal(const lwp::Vector3& v, const lwp::Matrix4x4& m) {
	return lwp::Vector3(
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	);
}
