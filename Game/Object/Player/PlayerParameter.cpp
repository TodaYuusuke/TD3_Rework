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
	// 回数やゲージを初期化
	InitCountGage();
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
		// 回数やゲージの表示
		DebugTreeCountGage();

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
	progressTime.damageTime = config.progressTime.damageTime;
	progressTime.damageInvincibleTime = config.progressTime.damageInvincibleTime;
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

void PlayerParameter::InitCountGage() {
	countGage.MaxAttackCount = 1;
	countGage.attackCount = countGage.MaxAttackCount;
	countGage.MaxHpCount = 3;
	countGage.hpCount = countGage.MaxHpCount;
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
		ImGui::SliderFloat("damageTime", &progressTime.damageTime, 0.0f, 5.0f);
		ImGui::SliderFloat("damageInvincigleTime", &progressTime.damageInvincibleTime, 0.0f, 5.0f);
		
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

void PlayerParameter::DebugTreeCountGage() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("CountGage")) {
		// 最大値が変わった時、攻撃可能回数も変更
		if(ImGui::SliderInt("MaxAttackCount", &countGage.MaxAttackCount, 1, 10))
			countGage.attackCount = countGage.MaxAttackCount;
		ImGui::SliderInt("attackCount", &countGage.attackCount, 0, countGage.MaxAttackCount);
		// 最大値が変わった時、HPも変更
		if(ImGui::SliderInt("MaxHpCount", &countGage.MaxHpCount, 3, 10))
			countGage.hpCount = countGage.MaxHpCount;
		ImGui::SliderInt("hpCount", &countGage.hpCount, 0, countGage.MaxHpCount);
		

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}
