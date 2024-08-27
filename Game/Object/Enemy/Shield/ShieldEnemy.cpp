#include "ShieldEnemy.h"

void ShieldEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;
}

void ShieldEnemy::Update()
{
	if (LWP::Input::Keyboard::GetTrigger(DIK_1)) {
		isDead_ = true;
	}

	if (CheckAttackRange())
	{
		isAttack_ = false;
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

void ShieldEnemy::Attack()
{
	//後ろに移動
	atackWork = AtackWork::Start;
	if (atackWork == AtackWork::Start) {
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

void ShieldEnemy::DyingAnimation()
{
}
