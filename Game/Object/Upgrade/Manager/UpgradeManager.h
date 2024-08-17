#pragma once
#include <vector>
#include "Game/Object/Upgrade/IUpgrade.h"
#include "Game/Object/Upgrade/Upgrade/AttackUp.h"
#include "Game/Object/Upgrade/Upgrade/HPUp.h"
#include "Game/Object/Upgrade/Upgrade/TestSkill.h"

class UpgradeManager
{
public:

	void Init(PlayerParameter& Input);

	void Update();

	void SelectUpgrade();

	void LevelUp();

	bool GetUpgradeFlag() {return UpgradeFlag;};

private:
	//抽選をする関数
	int Choose(bool select);
	//抽選をまとめた関数
	void RandomUpgrade();
	//選択したアップグレードを適応する
	void Apply();
	//UIを非表示にする
	void UIoff();

	void ImGui();

private://定数
	// 表示するアップグレードの数
	const int kUpgradNum_ = 2;

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

	// カーソルを合わせているアップグレード(何番目か)
	int cursorIndex_ = 0;

	// 選択しているアップグレード
	int choiceIndex_ = 0;

	bool isPress_ = false;

#pragma region
	// 選択場所のわかるUI
	LWP::Primitive::Sprite sprite_;

	lwp::Vector3 cursorPos = { LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2),240.0f };

	// 長押し用のフラグ
	bool isPress_ = false;

	// 長押しした時間
	float pressTime_ = 0.0f;
	// 決定に必要な時間
	const float kPressTime_ = 1.0f;
#pragma endregion 選択カーソル
#pragma region
	lwp::Vector2 UIpos[2] = {
		{640.0f,625.0f},
		{1280.0f,625.0f},
	};

#pragma endregion UI
};