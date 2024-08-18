#pragma once
#include "Game/Object/Player/Skill/ISkill.h"

#pragma region

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

// 距離を格納
struct LengthRadius {
	float playerRadius;	// プレイヤーの判定
	float attackRadius;	// 攻撃の範囲
	float EXPpullRadius;// 経験値吸収
};

// 補正や割合を格納
struct CollectionRatio {
	float attackReduceStart;	// 攻撃の開始判定を縮める割合(0 に近いほど縮まない)
	float attackExtendEnd;		// 攻撃の終了判定を伸ばす割合(0 に近いほど伸びない)
	float momentStuckRatio;		// 攻撃後に動けない時間の割合(0 に近いほど早く動く)
};

// 回数やゲージ的なものを格納
struct CountGage {
	int MaxAttackCount;		// 攻撃ができる最大数
	int attackCount;		// 現在攻撃ができる回数
	int MaxHpCount;			// 体力の最大数
	int hpCount;			// 現在の体力数
};

//　レベル関連
struct Level {
	int level_ = 1;
	float exp_;//貯まっている経験値

};
class ISkill;

struct Upgrade {
	// 移動速度のパラメータ
	MoveSpeed moveSpeed;
	// 時間のパラメータ
	ProgressTime progressTime;
	// 距離のパラメータ
	LengthRadius lengthRadius;
	// 補正や割合のパラメータ
	CollectionRatio collectionRatio;
	// 回数やゲージのパラメータを
	CountGage countGage;
	// スキル
	std::list<ISkill*>skill;
	//レベル関連
	Level level;
};

// フラグをまとめた構造体
struct Flags {
	bool isInputMove = false;	// 移動入力されたか
	bool isInputAttack = false;	// 攻撃入力されたか
	bool isDamage = false;      // 攻撃を受けたか
	bool isDead = false;		// 死んでいるか
};
#pragma endregion パラメーター構造体

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