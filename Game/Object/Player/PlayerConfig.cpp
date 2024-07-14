#include "PlayerConfig.h"

#include <Adapter.h>

void PlayerConfig::Init() {
	InitMoveSpeed();
	InitProgressTime();
}

void PlayerConfig::DebugTree() {
#ifdef DEMO
	// Tree で表示
	if (ImGui::TreeNode("PlayerConfig")) {
		// 移動値の表示
		DebugTreeMoveSpeed();
		// 時間の表示
		DebugTreeProgressTime();

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();

	}
#endif // DEMO
}

void PlayerConfig::InitMoveSpeed() {
	moveSpeed.moveSpeed = 5.0f;
	moveSpeed.attackSpeed = 200.0f;
	moveSpeed.momentSpeed = 1.0f;
}

void PlayerConfig::InitProgressTime() {
	progressTime.attackTime = 0.2f;
	progressTime.momentTime = 0.5f;
}

void PlayerConfig::DebugTreeMoveSpeed() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("MoveSpeed")) {
		ImGui::SliderFloat("moveSpeed", &moveSpeed.moveSpeed, 0.0f, 20.0f);
		ImGui::SliderFloat("attackSpeed", &moveSpeed.attackSpeed, 0.0f, 1000.0f);
		ImGui::SliderFloat("momentSpeed", &moveSpeed.momentSpeed, 0.0f, 10.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerConfig::DebugTreeProgressTime() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("ProgressTime")) {
		ImGui::SliderFloat("attackTime", &progressTime.attackTime, 0.0f, 5.0f);
		ImGui::SliderFloat("momentTime", &progressTime.momentTime, 0.0f, 5.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}
