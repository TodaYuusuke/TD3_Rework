#pragma once
#include "../IEnemy.h"

class NormalEnemy : public IEnemy {
public:
	NormalEnemy() : IEnemy(60.0f, 1.0f, 1.0f) {};

	void Init()override;
	void Update()override;
	void Attack();
	void DyingAnimation()override;

private:
	AtackWork atackWork_;

	lwp::Quaternion Rot_;
	lwp::Quaternion EndRot_;
	MotionWork attackRotWork_ = {
	.targetpoint = 1.0f,
	.speed = 0.05f,
	};
	MotionWork attackMoveWork_ = {
	.targetpoint = 1.0f,
	.speed = 0.05f,
	};
	MotionWork attackMoveEndWork_ = {
		.speed = 0.1f,
	};
	MotionWork attackStanbyWork_ = {
	.speed = 0.02f,
	};
	MotionWork attackEndWork_ = {
	.targetpoint = 0.0f,
	.speed = 0.1f,
	};
};