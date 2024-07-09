#pragma once
#include "../IEnemy.h"

class NormalEnemy : public IEnemy {
public:
	void Init()override;
	void Update()override;
	void Attack();
};