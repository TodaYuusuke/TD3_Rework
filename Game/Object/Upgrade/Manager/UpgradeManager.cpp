#include "UpgradeManager.h"

void UpgradeManager::Init(PlayerParameter& Input)
{
	parameter_ = &Input;
	attackUpgrade_.push_back(new AttackUp);
	attackUpgrade_.push_back(new TestSkill);
	escapeUpgrade_.push_back(new HPUp);
	escapeUpgrade_.push_back(new AttackUp);
}

void UpgradeManager::Update()
{

	if (parameter_->GetLevelUpSignal()) {
		LevelUp();
	}
#ifdef _DEBUG
	ImGui();
#endif

	//アップグレードを選択
	if (UpgradeFlag) {
		SelectUpgrade();
	}
}

void UpgradeManager::SelectUpgrade()
{
	lwp::Vector2 stick = LWP::Input::Controller::GetLStick();

	//右を選択(攻撃)
	if (0 < cursorIndex_ &&
		(lwp::Keyboard::GetTrigger(DIK_A) ||
			lwp::Keyboard::GetTrigger(DIK_LEFT) ||
			lwp::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT) ||
			0.0f > stick.x)) {
		cursorIndex_ = 0;
	}
	//左を選択(回避)
	else if (cursorIndex_ < kUpgradNum_ &&
		(lwp::Keyboard::GetTrigger(DIK_D) ||
			lwp::Keyboard::GetTrigger(DIK_RIGHT) ||
			lwp::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) ||
			stick.x > 0.0f)) {
		cursorIndex_ = 1;
	}
	//決定ボタンを押したら
	if ((lwp::Keyboard::GetTrigger(DIK_SPACE) ||
		lwp::Pad::GetTrigger(XINPUT_GAMEPAD_A))) {
		//selectの0番目にアタック用、1番目にエスケープ用の選択された番号が入っている
		//右はアタック用
		if (cursorIndex_ == 0) {
			attackUpgrade_[select_[0]]->Apply(parameter_);
			SelectUpgradeName.push_back(attackUpgrade_[select_[0]]->GetUpgradeName());
		}
		//左はエスケープ用
		else if (cursorIndex_ == 1) {
			escapeUpgrade_[select_[1]]->Apply(parameter_);
			SelectUpgradeName.push_back(escapeUpgrade_[select_[1]]->GetUpgradeName());
		}
		UpgradeFlag = false;
	}

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

void UpgradeManager::Apply()
{

}

void UpgradeManager::ImGui()
{
	ImGui::Begin("Upgrade");
	for (std::string String : SelectUpgradeName) {
		ImGui::Text(String.c_str());
	}
	ImGui::Text("SelectIndex : %d", cursorIndex_);
	ImGui::End();
}
