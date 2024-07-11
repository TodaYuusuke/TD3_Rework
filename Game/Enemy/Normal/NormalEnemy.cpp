#include "NormalEnemy.h"

void NormalEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadShortPath("NormalEnemy/NormalEnemy.obj");
	models_[0].materials[1].enableLighting = false;

	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;
}

void NormalEnemy::Update()
{
	if (CheckAttackRange())
	{
		IsAttack_ = true;
	}
	if (IsAttack_)
	{
		// 攻撃処理
		Attack();
		// 攻撃アニメーション
		//AttackAnimation();
	}
	else
	{
		LockPlayer();
		Move();
	}
}

void NormalEnemy::Attack()
{
}

void NormalEnemy::DyingAnimation()
{
}
