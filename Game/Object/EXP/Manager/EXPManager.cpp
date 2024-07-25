#include "EXPManager.h"

void EXPManager::Init()
{
}

void EXPManager::Update()
{
	for (EXP* exp :exp_) {
		exp->Update();
	}
}

void EXPManager::AddEXP(lwp::Vector3 position)
{
	EXP* exp = new EXP();
	exp->Init(position);
	exp_.push_back(exp);
}
