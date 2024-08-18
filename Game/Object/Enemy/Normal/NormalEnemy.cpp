#include "NormalEnemy.h"

void NormalEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadShortPath("NormalEnemy/NormalEnemy.obj");
	models_[0].materials[1].enableLighting = false;
	models_[0].worldTF.translation.y = 0.5f;
	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;

	InitCollider();
}

void NormalEnemy::Update()
{
	if (hp_ == 0) {
		Dying();
		return;
	}

	if (CheckAttackRange())
	{
		isAttack_ = true;
	}
	if (isAttack_)
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
		isAttack_ = false;
	}
	//貯める

	//突進
}