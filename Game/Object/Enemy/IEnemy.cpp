#include "IEnemy.h"
#include "Object/Player/Player.h"

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
	//プレイヤーの現在の向き
	targetVel = targetVel.Normalize();

	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,0.0f,1.0f }, targetVel).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,0.0f,1.0f }, targetVel);

	//行きたい方向のQuaternionの作成
	models_[0].worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));
}

void IEnemy::CreateSpawnDirection(lwp::Vector3 pos) {
	// 光の柱
	lightPillarMotion_.Add(&lightPillar_.worldTF.scale, LWP::Math::Vector3{ 1.5f,1.5f,1.5f }, 0, 0.1f)
		.Add(&lightPillar_.worldTF.scale, LWP::Math::Vector3{ -2.5f,-1.5f,-2.5f }, 0.1f, 0.1f);

	// 出現時の光の柱
	lightPillar_.material.texture = LWP::Resource::LoadTexture("Title.png");
	lightPillar_.name = "LightPillar";
	lightPillar_.worldTF.scale = { 1,100,1 };
	lightPillar_.worldTF.translation = pos;

	lightPillarMotion_.Start();
}

void IEnemy::Dying()
{
	DyingAnimation();
	deadFrame_--;
	if (deadFrame_ <= kDeadFrame_) {
		isDead_ = true;
	}
	if (isDead_) {
		Dead();
	}
}

void IEnemy::Dead()
{
	//IsDead_ = true;
	//collider_.isActive = false;
	//// 経験値を生成
	//manager_->Create(models_[0].transform.translation);
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
