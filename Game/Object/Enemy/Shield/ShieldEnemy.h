#pragma once
#include "../IEnemy.h"

class ShieldEnemy : public IEnemy {
public:
	ShieldEnemy() : IEnemy(60.0f, 1.0f, 1.0f) {};

	void Init()override;
	void Update()override;
	void Attack();
	void DyingAnimation()override;

private:
	MotionWork attackWork_ = {
		.speed = 0.05f,
	};
	MotionWork attackStanbyWork_ = {
		.speed = 0.05f,
	};
	MotionWork attackEndWork_ = {
		.speed = 0.1f,
	};
};