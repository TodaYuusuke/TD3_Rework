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
	targetVel.y = 0;
	//プレイヤーの現在の向き
	targetVel = targetVel.Normalize();

	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,0.0f,1.0f }, targetVel).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,0.0f,1.0f }, targetVel);

	//行きたい方向のQuaternionの作成
	models_[0].worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));
}

void IEnemy::Dying()
{
	DyingAnimation();
	deadFrame_--;
	if (deadFrame_ <= kDeadFlame_) {
		isDead_ = true;
	}
	if (isDead_) {
		Dead();
	}
}

void IEnemy::DyingAnimation()
{

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

void IEnemy::InitCollider()
{
	collider.SetBroadShape(lwp::Collider::AABB());
	collider.SetFollowTarget(&models_[0].worldTF);
	collider.mask.SetBelongFrag(ColMask0);
	collider.name = "Enemy";
	// ヒットしたときの処理を設定
	collider.enterLambda = [this](lwp::Collider::Collider* hitTarget) { EnterEnemy(hitTarget); };
}

void IEnemy::EnterEnemy(LWP::Object::Collider::Collider* hitTarget)
{
	if (hitTarget->name == "Player") {
		
	}
}
