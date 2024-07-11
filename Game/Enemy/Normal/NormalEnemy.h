#pragma once
#include "../IEnemy.h"

class NormalEnemy : public IEnemy {
public:
	NormalEnemy() : IEnemy(180.0f, 1.0f, 1.0f) {};

	void Init()override;
	void Update()override;
	void Attack();
	void DyingAnimation()override;

private:
};