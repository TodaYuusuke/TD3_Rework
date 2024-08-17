#pragma once
#include "Game/Object/Upgrade/IUpgrade.h"

class AttackUp : public IUpgrade{
public:

	void Upgrade(PlayerParameter* Input)override{
		//TODO:攻撃力がないので仮
		Input->status.moveSpeed.moveSpeed += 10.0f;
	};

	std::string GetTexturePass()override{
		//テクスチャのファイルパス
		return "Skill/PowerUp.png";
	}

	std::string GetUpgradeName()override{
		return "AttackUp";
	}

};