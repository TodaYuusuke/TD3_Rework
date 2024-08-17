#pragma once
#include "Game/Object/Upgrade/IUpgrade.h"

class HPUp : public IUpgrade {
public:

	void Upgrade(PlayerParameter* Input)override {
		Input->status.countGage.MaxHpCount += 1;
		Input->status.countGage.hpCount += 1;
	};

	std::string GetTexturePass()override {
		//テクスチャのファイルパス
		return "Skill/PowerUp.png";
	}

	std::string GetUpgradeName()override {
		return "HPUp";
	}

};