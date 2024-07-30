#pragma once
#include <vector>
#include "Game/Object/Upgrade/IUpgrade.h"
#include "Game/Object/Upgrade/Skill/AttackUp.h"
#include "Game/Object/Upgrade/Skill/HPUp.h"

class UpgradeManager
{
public:

	void Init(PlayerParameter& Input);

	void Update();

	void SelectUpgrade();

	void LevelUp();

	int Choose(bool select);

	bool GetUpgradeFlag() {return UpgradeFlag;};


private:

	void RandomUpgrade();

private:
	//ImGui用
	std::vector<std::string> SelectUpgradeName;

	// アップグレードのリスト
	std::vector<IUpgrade*> attackUpgrade_;
	std::vector<IUpgrade*> escapeUpgrade_;
	//trueにするとアップグレード選択へ
	bool UpgradeFlag = false;
	// ランダムで選択したアップグレード
	std::vector<int> select_;
	PlayerParameter* parameter_;
};