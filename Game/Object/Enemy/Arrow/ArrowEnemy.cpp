#include "ArrowEnemy.h"

void ArrowEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;
}

void ArrowEnemy::Update() {
	// 矢の更新処理
	arrows_.remove_if([](Arrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		arrow->Update();
		return false;
		});

	// 攻撃範囲内にいるか
	if (CheckAttackRange()) {
		isAttack_ = true;
	}

	// 攻撃処理
	if (isAttack_) {
		// 攻撃処理(アニメーション処理もアリ)
		Attack();
	}
	// 自機に向かって歩く
	else {
		LockPlayer();
		Move();
	}

	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
}

void ArrowEnemy::Attack()
{
	// 矢の発射
	if (attackWaitTime_ <= 0)
	{
		LockPlayer();
		Arrow* arrow = new Arrow();
		arrow->Init(models_[0].worldTF);
		arrows_.push_back(arrow);
		attackWaitTime_ = kAttackWaitTime_;
		isAttack_ = false;
	}
}

void ArrowEnemy::DyingAnimation()
{
}
