#include "NormalEnemy.h"

void NormalEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;
}

void NormalEnemy::Update()
{
	if (CheckAttackRange())
	{
		IsAttack_ = false;
	}
	if (IsAttack_)
	{
		// 攻撃処理
		Attack();
		// 攻撃アニメーション

	}
	else
	{
		LockPlayer();
		Move();
	}
}

void NormalEnemy::Attack()
{
	//後ろに移動
	atackWork = AtackWork::Stert;
	if (atackWork == AtackWork::Stert) {
		atackWork = AtackWork::Interval;
	}
	else if (atackWork == AtackWork::Interval) {
		atackWork = AtackWork::End;
	}
	else if (atackWork == AtackWork::End) {
		IsAttack_ = false;
	}
	//貯める

	//突進
}

void NormalEnemy::DyingAnimation()
{
}
