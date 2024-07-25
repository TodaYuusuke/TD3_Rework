#include "IEnemy.h"
#include "Object/Player/Player.h"

void IEnemy::ImGui()
{
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("translate",&models_[0].worldTF.translation.x);

	if (ImGui::Button("HP")) {
		hp_ = 0;
	}
	ImGui::End();

}

void IEnemy::CommonUpdate()
{



	Update();
}

void IEnemy::Move()
{
	lwp::Vector3 moveVec = GetDirectVel();
	moveVec.y = 0.0f;
	models_[0].worldTF.translation += moveVec * moveSpeed_ * LWP::Info::GetDeltaTimeF();
}

void IEnemy::LockPlayer()
{
	// 自機に向かうベクトル
	LWP::Math::Vector3 targetVel = GetDirectVel();
	// 狙う対象に身体を向ける
	lwp::Vector3 sub = targetVel - models_[0].worldTF.translation;

	//プレイヤーの現在の向き
	sub = sub.Normalize();

	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,0.0f,1.0f }, sub).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

	//行きたい方向のQuaternionの作成
	models_[0].worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));
}

void IEnemy::Dying()
{
	DyingAnimation();
	deadFlame_--;
	if (deadFlame_ <= kDeadFlame_) {
		IsDead_ = true;

	}
	if (IsDead_) {
		Dead();
	}
}

void IEnemy::Dead()
{
	//IsDead_ = true;
	//collider_.isActive = false;
	//// 経験値を生成
	//manager_->Create(models_[0].transform.translation);
	expManager_->AddEXP(models_[0].worldTF.translation);
}

bool IEnemy::CheckAttackRange()
{
	// 自機との距離
	float distance = (models_[0].worldTF.translation - player_->GetWorldPosition()).Length();
	if (distance <= kAttackRange_) {
		return true;
	}
	return false;
}

lwp::Vector3 IEnemy::GetDirectVel(){
	return (player_->GetWorldPosition() - models_[0].worldTF.translation).Normalize();
}
