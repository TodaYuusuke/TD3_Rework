#pragma once
#include "Game/Object/Upgrage/IUpgrade.h"
namespace Choose {
	int Attack = 0;
	int Escape = 1;
};
class UpgradeManager
{
public:

	void Init();

	void Update();

	void LevelUp();

	int Choose(int select);

	bool GetUpgradeFlag() {return UpgradeFlag;};

private:

	void RandomUpgrade();

private:
	// アップグレードのリスト
	static std::vector<IUpgrade*> attackUpgrade_;
	static std::vector<IUpgrade*> escapeUpgrade_;
	//trueにするとアップグレード選択へ
	bool UpgradeFlag = false;
	// ランダムで選択したアップグレード
	static std::vector<int> selectUpgrade_;

};