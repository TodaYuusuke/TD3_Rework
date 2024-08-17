#pragma once
#include <Adapter.h>
#include "Object/Enemy/IEnemy.h"
#include "Object/Player/Player.h"

using namespace LWP::Object::Collider;

class Arrow
{
public:
	~Arrow() = default;
	void Init(lwp::TransformQuat transform);
	void Update();
	void Attack();

	//*** Getter ***//
	bool GetIsAlive() { return isAlive_; }

	//*** Setter ***//
	void SetIsCollision(bool isActive) { aabb_.isActive = isActive; }
	// 向きを設定
	void SetRotate(LWP::Math::Vector3 rotate) { model_.worldTF.rotation = rotate; }
	// 速度を設定
	void SetVelocity(LWP::Math::Vector3 velocity) { velocity_ = velocity; }
	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }

public:
	void Death();

private:// 定数
	// 弾の寿命
	const float kLifeTime = 600;

	// 通常弾の速度
	const float kNormalSpeed = 10.0f;

private:
	//モデルインスタンス
	LWP::Resource::RigidModel model_;
	// 自機のアドレス
	Player* player_;

	// AABB判定
	AABB aabb_;

	// 弾の寿命
	float deadTimer_;

	// 方向ベクトル
	LWP::Math::Vector3 velocity_;

	bool isAlive_;
};