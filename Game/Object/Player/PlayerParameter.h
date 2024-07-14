#pragma once

#include "PlayerConfig.h"


/// <summary>
/// プレイヤーの基本的な設定を持っている
/// 設定から参照し、計算した結果を格納
/// 設計的に、初期設定と同じ項目が多い
/// </summary>
class PlayerParameter final {
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

	//*** 初期設定から計算した結果を格納 ***//

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

	// 距離を格納
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


public: //*** メンバ変数 ***//

	// 基本的な設定
	PlayerConfig config;
	
	// 移動速度のパラメータ
	MoveSpeed moveSpeed;
	// 時間のパラメータ
	ProgressTime progressTime;
	// 距離のパラメータ
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