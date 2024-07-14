#include "PlayerParameter.h"

#include <Adapter.h>

void PlayerParameter::Init() {
	// 最初に設定を初期化する
	config.Init();

	// 移動値を初期化
	InitMoveSpeed();
	// 時間を初期化
	InitProgressTime();
	// 距離や範囲を初期化
	InitLengthRadius();
	// 補正や割合を初期化
	InitCollectionRatio();
}

void PlayerParameter::DebugTree() {
#ifdef DEMO
	// 設定の表示
	config.DebugTree();

	// Tree で表示
	if (ImGui::TreeNode("PlayerParameter")) {
		// 初期化する
		if (ImGui::Button("Init")) {
			Init();
		}

		// 移動値の表示
		DebugTreeMoveSpeed();
		// 時間の表示
		DebugTreeProgressTime();
		// 距離の表示
		DebugTreeLengthRadius();
		// 補正や割合の表示
		DebugTreeCollectionRatio();

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
#endif // DEMO
}

void PlayerParameter::InitMoveSpeed() {
	moveSpeed.moveSpeed = config.moveSpeed.moveSpeed;
	moveSpeed.attackSpeed = config.moveSpeed.attackSpeed;
	moveSpeed.momentSpeed = config.moveSpeed.momentSpeed;
}

void PlayerParameter::InitProgressTime() {
	progressTime.attackTime = config.progressTime.attackTime;
	progressTime.momentTime = config.progressTime.momentTime;
}

void PlayerParameter::InitLengthRadius() {
	lengthRadius.playerRadius = config.lengthRadius.playerRadius;
	lengthRadius.attackRadius = config.lengthRadius.attackRadius;
}

void PlayerParameter::InitCollectionRatio() {
	collectionRatio.attackReduceStart = config.collectionRatio.attackReduceStart;
	collectionRatio.attackExtendEnd = config.collectionRatio.attackExtendEnd;
	collectionRatio.momentStuckRatio = config.collectionRatio.momentStuckRatio;
}

void PlayerParameter::DebugTreeMoveSpeed() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("MoveSpeed")) {
		ImGui::SliderFloat("moveSpeed", &moveSpeed.moveSpeed, 0.0f, 50.0f);
		ImGui::SliderFloat("attackSpeed", &moveSpeed.attackSpeed, 0.0f, 1000.0f);
		ImGui::SliderFloat("momentSpeed", &moveSpeed.momentSpeed, 0.0f, 30.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeProgressTime() {
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

void PlayerParameter::DebugTreeLengthRadius() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("LengthRadius")) {
		ImGui::SliderFloat("playerRadius", &lengthRadius.playerRadius, 0.01f, 5.0f);
		ImGui::SliderFloat("attackRadius", &lengthRadius.attackRadius, 0.01f, 5.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeCollectionRatio() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("CollectionRatio")) {
		ImGui::SliderFloat("attackReduceStart", &collectionRatio.attackReduceStart, -2.0f, 2.0f);
		ImGui::SliderFloat("attackExtendEnd", &collectionRatio.attackExtendEnd, -2.0f, 2.0f);
		ImGui::SliderFloat("momentStuckRatio", &collectionRatio.momentStuckRatio, 0.0f, 1.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}