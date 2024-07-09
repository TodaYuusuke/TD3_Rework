#include "IEnemy.h"
#include "../Player/Player.h"

void IEnemy::Move()
{
	lwp::Vector3 MoveVec = GetDirectVel();
	MoveVec.y = 0.0f;
	models_[0].worldTF.translation += MoveVec * MoveSpeed * LWP::Info::GetDeltaTimeF();
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

lwp::Vector3 IEnemy::GetDirectVel(){
	return (player_->GetWorldPosition() - models_[0].worldTF.translation).Normalize();
}
