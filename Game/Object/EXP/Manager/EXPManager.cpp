#include "EXPManager.h"

void EXPManager::Init()
{
}

void EXPManager::Update()
{
	for (EXP* exp :exp_) {
		exp->Update();
	}

	exp_.remove_if([this](EXP* exp)
		{
			if (exp->GetIsDead()) {
				// 死亡パーティクル発生
				delete exp;
				return true;
			}
			return false;
		});
}

void EXPManager::AddEXP(lwp::Vector3 position,Player* player)
{
	EXP* exp = new EXP();
	exp->Init(position);
	exp->SetPlayer(player);
	exp_.push_back(exp);
}
