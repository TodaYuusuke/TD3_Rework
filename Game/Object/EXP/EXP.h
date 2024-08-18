#pragma once
#include <Adapter.h>
class Player;
class EXP
{
public:
	EXP(){};
	~EXP(){};

	void Init(lwp::Vector3 position);

	void Update();

	void InitCollider();

	bool GetIsDead() { return isDead_; };

	void SetPlayer(Player* player) {
		player_ = player;
	};
	// コライダーと形状をまとめた構造体
	// 形状はカプセル
	struct SphereCollider {
		LWP::Object::Collider::Collider collider;	// 当たり判定
		LWP::Object::Collider::Sphere& sphere;	// 形状

		/// <summary>
		/// コンストラクタ
		/// <para>参照変数のために用意</para>
		/// </summary>
		SphereCollider() : sphere(collider.SetBroadShape(lwp::Collider::Sphere())) {
		}
	};
private:	
	void EnterEXP(LWP::Object::Collider::Collider* hitTarget);
	//当たり判定
	SphereCollider collider;

	Player* player_;

	LWP::Resource::RigidModel model_;

	lwp::Vector3 Rotate_;

	// この実態を消去してもいいか
	bool isDead_ = false;
	// サイズ
	const float kSize_ = 0.25f;

	bool isStert = false;
	lwp::Vector3* lvPosition_;

	// いま判定がない時間かを判断
	bool isDisable_ = true;

	// アニメーションする時間
	const float kAnimationTime_ = 0.2f;

	// 判定を付与するまでの時間
	const float kToEnableTime_ = 1.0f;

	// 加算する変数
	float time_ = 0.0f;

	// モデルの回転力
	float rotateSpeed_ = 0.3f;
	float kRotateGetSpeed_ = 0.7f;
};


