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
		float attackTime;			// 攻撃を行っている時間
		float momentTime;			// 攻撃後の後隙の時間
		float damageTime;			// ダメージを喰らった後の時間
		float damageInvincibleTime;	// ダメージを喰らった後の無敵時間
	};
	// TODO : 攻撃中の無敵時間

	// 距離や範囲を格納
	struct LengthRadius {
		float playerRadius;	// プレイヤーの判定
		float attackRadius;	// 攻撃の範囲
	};

	// 補正や割合を格納
	struct CollectionRatio {
		float attackReduceStart;	// 攻撃の開始判定を縮める割合(0 に近いほど縮まない)
		float attackExtendEnd;		// 攻撃の終了判定を伸ばす割合(0 に近いほど伸びない)
		float momentStuckRatio;		// 攻撃後に動けない時間の割合(0 に近いほど早く動く)
	};

public: //*** 変数群 ***//

	// 移動速度のパラメータをまとめている
	MoveSpeed moveSpeed;

	// 経過させる時間のパラメータ
	ProgressTime progressTime;

	// あらゆる距離のパラメータ
	LengthRadius lengthRadius;

	// 補正や割合のパラメータ
	CollectionRatio collectionRatio;

private: //*** 関数群 ***//

	// 移動値を初期化
	void InitMoveSpeed();
	// 時間を初期化
	void InitProgressTime();
	// 距離や範囲を初期化
	void InitLengthRadius();
	// 補正や割合を初期化
	void InitCollectionRatio();

	// 移動値のデバッグ表示
	void DebugTreeMoveSpeed();
	// 時間のデバッグ表示
	void DebugTreeProgressTime();
	// 距離や範囲のデバッグ表示
	void DebugTreeLengthRadius();
	// 補正や割合のデバッグ表示
	void DebugTreeCollectionRatio();

};