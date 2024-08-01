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

void PlayerParameter::Update(){
#ifdef _DEBUG
	ImGui::Begin("PlayerParameter");
	if (ImGui::Button("IsUpgrade")) {
		LevelUp();
	}
	ImGui::End();
#endif

}

bool PlayerParameter::GetLevelUpSignal(){
	if (IsUpGradeFlag) {
		IsUpGradeFlag = false;
		return true;
	}
	else {
		return false;
	}
}

void PlayerParameter::LevelUp(){
	IsUpGradeFlag = true;
	//TODO:レベルアップした時の処理を色々する
}

#pragma region

void PlayerParameter::InitMoveSpeed() {
	upgrade.moveSpeed.moveSpeed = config.moveSpeed.moveSpeed;
	upgrade.moveSpeed.attackSpeed = config.moveSpeed.attackSpeed;
	upgrade.moveSpeed.momentSpeed = config.moveSpeed.momentSpeed;
}

void PlayerParameter::InitProgressTime() {
	upgrade.progressTime.attackTime = config.progressTime.attackTime;
	upgrade.progressTime.momentTime = config.progressTime.momentTime;
	upgrade.progressTime.damageTime = config.progressTime.damageTime;
	upgrade.progressTime.damageInvincibleTime = config.progressTime.damageInvincibleTime;
}

void PlayerParameter::InitLengthRadius() {
	upgrade.lengthRadius.playerRadius = config.lengthRadius.playerRadius;
	upgrade.lengthRadius.attackRadius = config.lengthRadius.attackRadius;
}

void PlayerParameter::InitCollectionRatio() {
	upgrade.collectionRatio.attackReduceStart = config.collectionRatio.attackReduceStart;
	upgrade.collectionRatio.attackExtendEnd = config.collectionRatio.attackExtendEnd;
	upgrade.collectionRatio.momentStuckRatio = config.collectionRatio.momentStuckRatio;
}

void PlayerParameter::InitCountGage() {
	upgrade.countGage.MaxAttackCount = 1;
	upgrade.countGage.attackCount = upgrade.countGage.MaxAttackCount;
	upgrade.countGage.MaxHpCount = 3;
	upgrade.countGage.hpCount = upgrade.countGage.MaxHpCount;
}

#pragma endregion Init

#pragma region

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



void PlayerParameter::DebugTreeMoveSpeed() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("MoveSpeed")) {
		ImGui::SliderFloat("moveSpeed", &upgrade.moveSpeed.moveSpeed, 0.0f, 50.0f);
		ImGui::SliderFloat("attackSpeed", &upgrade.moveSpeed.attackSpeed, 0.0f, 1000.0f);
		ImGui::SliderFloat("momentSpeed", &upgrade.moveSpeed.momentSpeed, 0.0f, 30.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeProgressTime() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("ProgressTime")) {
		ImGui::SliderFloat("attackTime", &upgrade.progressTime.attackTime, 0.0f, 5.0f);
		ImGui::SliderFloat("momentTime", &upgrade.progressTime.momentTime, 0.0f, 5.0f);
		ImGui::SliderFloat("damageTime", &upgrade.progressTime.damageTime, 0.0f, 5.0f);
		ImGui::SliderFloat("damageInvincigleTime", &upgrade.progressTime.damageInvincibleTime, 0.0f, 5.0f);
		
		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeLengthRadius() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("LengthRadius")) {
		ImGui::SliderFloat("playerRadius", &upgrade.lengthRadius.playerRadius, 0.01f, 5.0f);
		ImGui::SliderFloat("attackRadius", &upgrade.lengthRadius.attackRadius, 0.01f, 5.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeCollectionRatio() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("CollectionRatio")) {
		ImGui::SliderFloat("attackReduceStart", &upgrade.collectionRatio.attackReduceStart, -2.0f, 2.0f);
		ImGui::SliderFloat("attackExtendEnd", &upgrade.collectionRatio.attackExtendEnd, -2.0f, 2.0f);
		ImGui::SliderFloat("momentStuckRatio", &upgrade.collectionRatio.momentStuckRatio, 0.0f, 1.0f);

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
		if(ImGui::SliderInt("MaxAttackCount", &upgrade.countGage.MaxAttackCount, 1, 10))
			upgrade.countGage.attackCount = upgrade.countGage.MaxAttackCount;
		ImGui::SliderInt("attackCount", &upgrade.countGage.attackCount, 0, upgrade.countGage.MaxAttackCount);
		// 最大値が変わった時、HPも変更
		if(ImGui::SliderInt("MaxHpCount", &upgrade.countGage.MaxHpCount, 3, 10))
			upgrade.countGage.hpCount = upgrade.countGage.MaxHpCount;
		ImGui::SliderInt("hpCount", &upgrade.countGage.hpCount, 0, upgrade.countGage.MaxHpCount);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

#pragma endregion DebugTree