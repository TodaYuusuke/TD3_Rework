#pragma once
#include <Adapter.h>

class Player;

class IEnemy
{
public:
	IEnemy() = delete;
	/// <param name="attackWaitTime">攻撃ができるようになるまでの時間</param>
	/// <param name="attackRange">攻撃範囲の広さ</param>
	/// <param name="deadFlame">死亡演出の長さ</param>
	IEnemy(float attackWaitTime, float attackRange, float deadFlame)
		: kAttackWaitTime_(attackWaitTime),
		kAttackRange_(attackRange),
		kDeadFlame_(deadFlame) {};
	virtual ~IEnemy() {};

	virtual void Init() = 0;
	virtual void Update() = 0;

	// 位置を指定して移動する
	void SetPosition(lwp::Vector3 Pos) {
		models_[0].worldTF.translation.x = Pos.x;
		models_[0].worldTF.translation.z = Pos.z;
	};

	//移動する
	void Move();
	//　プレイヤーを向く
	void LockPlayer();
	// 狙う対象をセット(今回は自機をセットする)
	void SetTarget(Player* player) { player_ = player; }

#pragma region
	//倒された後の処理をまとめた関数
	void Dying();
	//倒された時の演出
	virtual void DyingAnimation() = 0;
	//死亡時の処理
	void Dead();
	//
	bool GetIsDead() {
		return isDead_;
	}
#pragma endregion 死亡処理関連

#pragma region
	virtual void Attack() = 0;
	// プレイヤーが攻撃範囲内にいるかどうか
	//  いる場合trueを返す
	bool CheckAttackRange();
#pragma endregion 攻撃関連

public:

#pragma region 
	//攻撃ができるようになるまでの時間
	const float kAttackWaitTime_;
	//攻撃範囲の広さ
	const float kAttackRange_;
	//死亡演出の長さ
	const float kDeadFlame_;
#pragma endregion 定数

private://関数


	lwp::Vector3 GetDirectVel();

protected://変数
	//モデルインスタンス
	std::vector<LWP::Resource::RigidModel> models_;
	// プレイヤーのポインタ
	Player* player_;

	//移動の速さ
	//いじると移動速度に掛ける倍率が変わります
	float moveSpeed_ = 1.0f;
	// HP
	int hp_ = 0;
	// 攻撃のクールタイム
	float attackWaitTime_;

	//hpが0になってからの経過フレーム
	int deadFrame_ = 0;
	//生きているかどうか
	//HPが0になったタイミングでtrueになる
	bool isDying_ = false;
	//HPが0になった後、deadFlame_の値がkDeadFlame()より小さくなったらtrue
	bool isDead_ = false;
	//攻撃できるかどうか
	//trueなら攻撃可能
	bool isAttack_ = false;
};