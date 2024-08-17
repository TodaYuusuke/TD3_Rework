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
	// レベルとスキルを初期化
	InitLevelANDSkill();
}

void PlayerParameter::Update(){
#ifdef _DEBUG
	ImGui::Begin("PlayerParameter");
	if (ImGui::Button("IsUpgrade")) {
		LevelUp();
	}
	ImGui::End();
#endif
	for (ISkill* skill : status.skill) {
		if (flags_.isInputAttack) {
			skill->Update();
		}

	}
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
	status.level.level_++;
}

#pragma region

void PlayerParameter::InitMoveSpeed() {
	status.moveSpeed.moveSpeed = config.moveSpeed.moveSpeed;
	status.moveSpeed.attackSpeed = config.moveSpeed.attackSpeed;
	status.moveSpeed.momentSpeed = config.moveSpeed.momentSpeed;
}

void PlayerParameter::InitProgressTime() {
	status.progressTime.attackTime = config.progressTime.attackTime;
	status.progressTime.momentTime = config.progressTime.momentTime;
	status.progressTime.damageTime = config.progressTime.damageTime;
	status.progressTime.damageInvincibleTime = config.progressTime.damageInvincibleTime;
}

void PlayerParameter::InitLengthRadius() {
	status.lengthRadius.playerRadius = config.lengthRadius.playerRadius;
	status.lengthRadius.attackRadius = config.lengthRadius.attackRadius;
}

void PlayerParameter::InitCollectionRatio() {
	status.collectionRatio.attackReduceStart = config.collectionRatio.attackReduceStart;
	status.collectionRatio.attackExtendEnd = config.collectionRatio.attackExtendEnd;
	status.collectionRatio.momentStuckRatio = config.collectionRatio.momentStuckRatio;
}

void PlayerParameter::InitCountGage() {
	status.countGage.MaxAttackCount = 1;
	status.countGage.attackCount = status.countGage.MaxAttackCount;
	status.countGage.MaxHpCount = 3;
	status.countGage.hpCount = status.countGage.MaxHpCount;
}

void PlayerParameter::InitLevelANDSkill()
{
	status.level.level_ = 1;
	status.level.exp_ = 0;
	status.skill.clear();
	
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
		ImGui::SliderFloat("moveSpeed", &status.moveSpeed.moveSpeed, 0.0f, 50.0f);
		ImGui::SliderFloat("attackSpeed", &status.moveSpeed.attackSpeed, 0.0f, 1000.0f);
		ImGui::SliderFloat("momentSpeed", &status.moveSpeed.momentSpeed, 0.0f, 30.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeProgressTime() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("ProgressTime")) {
		ImGui::SliderFloat("attackTime", &status.progressTime.attackTime, 0.0f, 5.0f);
		ImGui::SliderFloat("momentTime", &status.progressTime.momentTime, 0.0f, 5.0f);
		ImGui::SliderFloat("damageTime", &status.progressTime.damageTime, 0.0f, 5.0f);
		ImGui::SliderFloat("damageInvincigleTime", &status.progressTime.damageInvincibleTime, 0.0f, 5.0f);
		
		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeLengthRadius() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("LengthRadius")) {
		ImGui::SliderFloat("playerRadius", &status.lengthRadius.playerRadius, 0.01f, 5.0f);
		ImGui::SliderFloat("attackRadius", &status.lengthRadius.attackRadius, 0.01f, 5.0f);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

void PlayerParameter::DebugTreeCollectionRatio() {
	// 更に Tree でまとめる
	if (ImGui::TreeNode("CollectionRatio")) {
		ImGui::SliderFloat("attackReduceStart", &status.collectionRatio.attackReduceStart, -2.0f, 2.0f);
		ImGui::SliderFloat("attackExtendEnd", &status.collectionRatio.attackExtendEnd, -2.0f, 2.0f);
		ImGui::SliderFloat("momentStuckRatio", &status.collectionRatio.momentStuckRatio, 0.0f, 1.0f);

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
		if(ImGui::SliderInt("MaxAttackCount", &status.countGage.MaxAttackCount, 1, 10))
			status.countGage.attackCount = status.countGage.MaxAttackCount;
		ImGui::SliderInt("attackCount", &status.countGage.attackCount, 0, status.countGage.MaxAttackCount);
		// 最大値が変わった時、HPも変更
		if(ImGui::SliderInt("MaxHpCount", &status.countGage.MaxHpCount, 3, 10))
			status.countGage.hpCount = status.countGage.MaxHpCount;
		ImGui::SliderInt("hpCount", &status.countGage.hpCount, 0, status.countGage.MaxHpCount);

		// Tree を閉じる
		ImGui::TreePop();
		// 見やすいために区切る
		ImGui::Separator();
	}
}

#pragma endregion DebugTree