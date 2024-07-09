#pragma once
#include <Adapter.h>

class Player;

class IEnemy
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	void Move();
	void LockPlayer();

	virtual void Attack() = 0;

private://関数

	// 狙う対象をセット(今回は自機をセットする)
	virtual void SetTarget(Player* player) { player_ = player; }

	lwp::Vector3 GetDirectVel();

private://変数
	//モデルインスタンス
	std::vector<LWP::Resource::RigidModel> models_;
	// プレイヤーのポインタ
	Player* player_;

protected:
	//移動の速さ
	//いじると移動速度に掛ける倍率が変わります
	float MoveSpeed = 1.0f;
};