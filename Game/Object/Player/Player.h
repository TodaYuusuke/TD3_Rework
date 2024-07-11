#pragma once
#include <Adapter.h>

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetWorldPosition() { model_.worldTF.GetWorldPosition(); }

private: //*** サブクラス ***//

	// 状態
	enum class Behavior : int {
		Idle = 0,
		Move,
		Attack,

		_Count,	// 状態最大数
	};

	// フラグをまとめた構造体
	struct Flags {
		bool isInputMove = false;
	};

private: //*** メンバ変数 ***//

	// モデル
	LWP::Resource::RigidModel model_;

	// 今の状態を格納
	Behavior behavior_ = Behavior::Idle;
	// 状態が変わった時に値が入る
	std::optional<Behavior> reqBehavior_ = std::nullopt;

	// 初期化関数ポインタの配列
	void (Player::* stateInit_[(int)Behavior::_Count])();
	// 更新関数ポインタの配列
	void (Player::* stateUpdate_[(int)Behavior::_Count])();
	
private: //*** 細かく設定される変数 ***//

	// まとめられたフラグ
	Flags flags_;

	// 次に移動する方向 | 向いている方向
	// モデルの向きから判断すればいいかもしれない
	// でも違う方向に向かう時に使うかも
	// ↑別の変数に役割を分ければいい
	LWP::Math::Vector3 destinate_ = { 0.0f,0.0f,1.0f };

	// プレイヤーの速度を格納
	LWP::Math::Vector3 velocity_ = { 0.0f,0.0f,0.0f };


private: //*** メンバ関数 ***//

	// 移動方向、向いている方向の入力を受け取る
	void CheckInputMove();

	// 関数ポインタ配列を初期化
	void InitStateFunctions();

private: //*** 各状態の関数群 ***//

	// 状態の初期化

	// 待機状態
	void InitIdle();
	// 移動状態
	void InitMove();
	// 攻撃状態
	void InitAttack();


	// 状態の更新

	// 待機状態
	void UpdateIdle();
	// 移動状態
	void UpdateMove();
	// 攻撃状態
	void UpdateAttack();


};