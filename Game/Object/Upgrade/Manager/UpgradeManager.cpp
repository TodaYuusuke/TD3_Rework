#include "UpgradeManager.h"

void UpgradeManager::Init(PlayerParameter& Input)
{
	parameter_ = &Input;
	attackUpgrade_.push_back(new AttackUp);
	attackUpgrade_.push_back(new AttackUp);
	escapeUpgrade_.push_back(new HPUp);
	escapeUpgrade_.push_back(new AttackUp);
}

void UpgradeManager::Update()
{
	ImGui::Begin("Upgrade");
	if (ImGui::Button("LevelUp")) {
		LevelUp();
	}
	for (std::string String:SelectUpgradeName) {
		ImGui::Text(String.c_str());
	}
	ImGui::End();
	//アップグレードを選択
	if (UpgradeFlag) {
	SelectUpgrade();
	}
}

void UpgradeManager::SelectUpgrade()
{
	//selectの0番目にアタック用、1番目にエスケープ用の選択された番号が入っている
	attackUpgrade_[select_[0]]->Apply(parameter_);
	escapeUpgrade_[select_[1]]->Apply(parameter_);
	SelectUpgradeName.push_back(attackUpgrade_[select_[0]]->GetUpgradeName());
	SelectUpgradeName.push_back(escapeUpgrade_[select_[1]]->GetUpgradeName());
	UpgradeFlag = false;
}

void UpgradeManager::LevelUp()
{
	//抽選を開始
	RandomUpgrade();
	UpgradeFlag = true;
}

int UpgradeManager::Choose(bool select)
{
	// 攻撃
	if (select == true)
	{
		// 抽選候補に入れる
		while (1)
		{
			// 取得する範囲の添え字を受け取る
			// 同じ数だけ作れば問題なし
			int result = LWP::Utility::GenerateRandamNum(0, (int)attackUpgrade_.size() - 1);

			// 既に選択していたら再抽選
			if (attackUpgrade_[result]->GetIsAppliedFlag())
			{
				continue;
			}
			return result;
		}
	}
	else{
		// 抽選候補に入れる
		while (1)
		{
			// 取得する範囲の添え字を受け取る
			// 同じ数だけ作れば問題なし
			int result = LWP::Utility::GenerateRandamNum(0, (int)escapeUpgrade_.size() - 1);

			// 既に選択していたら再抽選
			if (escapeUpgrade_[result]->GetIsAppliedFlag())
			{
				continue;
			}
			return result;
		}
	}
}

void UpgradeManager::RandomUpgrade()
{
	select_.clear();

	// 攻撃要素を取得
	select_.push_back(Choose(true));

	// 逃走要素を取得
	select_.push_back(Choose(false));
}
