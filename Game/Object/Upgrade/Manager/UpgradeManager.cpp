#include "UpgradeManager.h"

void UpgradeManager::Init(PlayerParameter& Input)
{
	parameter_ = &Input;
	attackUpgrade_.push_back(new AttackUp);
	attackUpgrade_.push_back(new TestSkill);
	escapeUpgrade_.push_back(new HPUp);
	escapeUpgrade_.push_back(new AttackUp);
	//初期化をする
	for (IUpgrade* upgrade: attackUpgrade_) {
		upgrade->Init();
	}
	for (IUpgrade* upgrade: escapeUpgrade_) {
		upgrade->Init();
	}

	sprite_.material.texture = LWP::Resource::LoadTexture("cursor2.png");
	sprite_.anchorPoint = { 0.5f,0.5f };
	sprite_.worldTF.translation = cursorPos;
	sprite_.isUI = true;
	sprite_.isActive = false;
	sprite_.material.enableLighting = false;
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

	sprite_.worldTF.translation.x = 640.0f * (cursorIndex_ + 1);

	// カーソルのスプライトを上下に揺らす
	cursorAnimFrame_ += lwp::GetDefaultDeltaTimeF() * 60;
	sprite_.worldTF.translation.y += sinf(cursorAnimFrame_ * (float)std::numbers::pi / 20) * 0.4f;
	
	//決定ボタンを押したら
	if ((lwp::Keyboard::GetPress(DIK_SPACE) ||
		lwp::Pad::GetPress(XINPUT_GAMEPAD_A))) {
		isPress_ = true;
	}
	else {
		isPress_ = false;
	}

	//決定ボタンが押されていた時
	if (isPress_) {
		KeyHoldAction();
	}
	//選択
	else {
		Select();
		KeyReleaseAction();
	}
	//設定した後にアニメーションを付けるための時間
	if (isSelected_) {
		SelectedFlame_ += lwp::GetDefaultDeltaTimeF();
		if (SelectedFlame_ > kSelectedFlame_) {
			UpgradeFlag = false;
		}
	}

}

void UpgradeManager::LevelUp()
{
	if (parameter_->status.level.level_ > attackUpgrade_.size()) {
		return;
	}

	//抽選を開始
	RandomUpgrade();
	UpgradeFlag = true;
	//初期化
	cursorIndex_ = 0;
	choiceIndex_ = 0;

	isSelected_ = false;
	SelectedFlame_ = 0;

	pressTime_ = 0;

	cursorAnimFrame_ = 0;

	//UIを表示
	attackUpgrade_[select_[0]]->SetUIPos(UIpos[0]);
	escapeUpgrade_[select_[1]]->SetUIPos(UIpos[1]);
	// カーソルUIを表示
	sprite_.isActive = true;
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

void UpgradeManager::Select()
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
}

void UpgradeManager::Apply()
{
	if (isSelected_) {
		return;
	}

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
	UIoff();
	isSelected_ = true;
}

void UpgradeManager::UIoff()
{
	//カーソル
	sprite_.isActive = false;
	//アップグレード
	attackUpgrade_[select_[0]]->UIoff();
	escapeUpgrade_[select_[1]]->UIoff();
}

void UpgradeManager::KeyHoldAction()
{
	//押している間加算
	pressTime_ += lwp::GetDefaultDeltaTimeF();

	// カーソルをつぶしたり伸ばしたりする
	sprite_.worldTF.scale.y -= (sinf(pressTime_ * 60 * (float)std::numbers::pi / 10) * 0.05f);
	sprite_.worldTF.scale.x += (sinf(pressTime_ * 60 * (float)std::numbers::pi / 10) * 0.05f);

	if (pressTime_ > kPressTime_) {
		Apply();
	}
}

void UpgradeManager::KeyReleaseAction()
{
	pressTime_ = 0;
	sprite_.worldTF.scale.y = 1.0f;
	sprite_.worldTF.scale.x = 1.0f;
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
