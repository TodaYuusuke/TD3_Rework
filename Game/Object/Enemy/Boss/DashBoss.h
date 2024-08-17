#pragma once
#include "../IEnemy.h"
#include "../../Particle/AccumulateParticle.h"
#include "../../Particle/DeadParticle.h"

/// <summary>
/// 突進タイプのボスキャラ
/// </summary>
class DashBoss : public IEnemy
{
private:// 構造体
	// 振るまい
	enum class Behavior {
		kRoot,	 // 通常状態
		kPreDash,// ダッシュ攻撃前の硬直
		kDash	 // ダッシュ中
	};

public:
	///
	/// 純粋仮想関数
	/// 

	DashBoss() : IEnemy(60.0f, 20.0f, 1.0f) {};
	~DashBoss()override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

private:
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack()override;

	void DyingAnimation()override;

	///
	/// User Method
	/// 

	/// <summary>
	/// 攻撃条件
	/// </summary>
	/// <returns></returns>
	bool CheckAttackRange();

#pragma region 振るまい

	// 通常状態の初期化
	void B_RootInit();
	// 通常状態の更新処理
	void B_RootUpdate();

	// ダッシュ攻撃前の硬直の初期化
	void B_PreDashInit();
	// ダッシュ攻撃前の硬直の更新処理
	void B_PreDashUpdate();

	// ダッシュ状態の初期化
	void B_DashInit();
	// ダッシュ状態の更新処理
	void B_DashUpdate();

#pragma endregion

private:// 定数
	// 突進速度の係数
	const float kDashSpeedCoefficient = 2.0f;

	// 攻撃のクールタイム
	const float kAttackWaitTime = 240;
	// 突進攻撃の全体フレーム
	const float kDashAttackAllFrame = 30;
	// 攻撃前の硬直の全体フレーム
	const float kPreDashAllFrame = 60;

private:// プライベートな変数
	// ダッシュの方向ベクトル
	LWP::Math::Vector3 dashVel_;
	lwp::Vector3 dirVel_;

	// 現在のフレーム
	int currentFrame_;

	// 現在の振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 攻撃前アニメーション
	LWP::Resource::Motion preAttackMotion_;

	bool isActive_;

	AccumulateParticle accumulateParticle_;
	DeadParticle deadParticle_;
};