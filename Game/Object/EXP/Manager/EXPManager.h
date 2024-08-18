#pragma once

#include "Game/Object/EXP/EXP.h"

class EXPManager
{
public:
	void Init();
	void Update();
	void AddEXP(lwp::Vector3 position, Player* player);

private:
	std::list<EXP*>exp_;
};
