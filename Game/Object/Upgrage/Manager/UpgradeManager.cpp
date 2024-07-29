#include "UpgradeManager.h"

// 実体宣言
std::vector<IUpgrade*> UpgradeManager::attackUpgrade_;
std::vector<IUpgrade*> UpgradeManager::escapeUpgrade_;
std::vector<int> selectUpgrade_;

void UpgradeManager::Init()
{

}

void UpgradeManager::Update()
{
	
}

void UpgradeManager::LevelUp()
{
	//抽選を開始
	RandomUpgrade();
	UpgradeFlag = true;
}

int UpgradeManager::Choose(int select)
{
	// 攻撃
	if (Choose::Attack)
	{
		// 抽選候補に入れる
		while (1)
		{
			// 取得する範囲の添え字を受け取る
			// 同じ数だけ作れば問題なし
			int rand = LWP::Utility::GenerateRandamNum(0, (int)attackUpgrade_.size() - 1);

			// 既に選択していたら再抽選
			if (attackUpgrade_[rand]->GetIsAppliedFlag())
			{
				continue;
			}
			return rand;
		}
	}
	else if(Choose::Escape)
	{
		// 抽選候補に入れる
		while (1)
		{
			// 取得する範囲の添え字を受け取る
			// 同じ数だけ作れば問題なし
			int rand = LWP::Utility::GenerateRandamNum(0, (int)escapeUpgrade_.size() - 1);

			// 既に選択していたら再抽選
			if (attackUpgrade_[rand]->GetIsAppliedFlag())
			{
				continue;
			}
			return rand;
		}
	}
}

void UpgradeManager::RandomUpgrade()
{
	selectUpgrade_.clear();

	// 攻撃要素を取得
	selectUpgrade_.push_back(Choose(Choose::Attack));

	// 逃走要素を取得
	selectUpgrade_.push_back(Choose(Choose::Escape));
}
