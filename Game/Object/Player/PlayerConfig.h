#pragma once


/// <summary>
/// プレイヤーの基本的な設定を格納
/// </summary>
class PlayerConfig final {
public: //*** メンバ関数 ***//

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// Tree 形式で内部の情報を表示する
	/// <para>内部でも #ifdef DEMO をしている</para>
	/// </summary>
	void DebugTree();

private: //*** サブクラス ***//

	// 移動時の値を格納
	// 秒速で計算している
	struct MoveSpeed {
		float moveSpeed;	// 通常移動の速度
		float attackSpeed;	// 攻撃時の速度
		float momentSpeed;	// 攻撃後の後隙の速度
	};

	// 経過させる時間(秒)
	struct ProgressTime {
		float attackTime;	// 攻撃を行っている時間
		float momentTime;	// 攻撃後の後隙の時間1
	};

public: //*** 変数群 ***//

	// 移動速度のパラメータをまとめている
	MoveSpeed moveSpeed;

	// 経過させる時間のパラメータ
	ProgressTime progressTime;

private: //*** 関数群 ***//

	// 移動値を初期化
	void InitMoveSpeed();
	// 時間を初期化
	void InitProgressTime();


	// 移動値のデバッグ表示
	void DebugTreeMoveSpeed();
	// 時間のデバッグ表示
	void DebugTreeProgressTime();

};