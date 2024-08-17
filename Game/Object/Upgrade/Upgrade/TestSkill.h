#pragma once
#include "Game/Object/Upgrade/IUpgrade.h"
#include "Game/Object/Player/Skill/test/test.h"

class TestSkill : public IUpgrade {
public:

	void Upgrade(PlayerParameter* Input)override {
		Input->status.skill.push_back(new Test);
	};

	std::string GetTexturePass()override {
		//テクスチャのファイルパス
		return "Skill/PowerUp.png";
	}

	std::string GetUpgradeName()override {
		return "TestSkill";
	}

};