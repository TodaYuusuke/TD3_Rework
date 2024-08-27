#include "NormalEnemy.h"

void NormalEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;
}

void NormalEnemy::Update() {
	// 攻撃範囲内にいるか
	if (CheckAttackRange()) {
		isAttack_ = true;
	}

	// 攻撃処理
	if (isAttack_) {
		// 攻撃処理(アニメーション処理もアリ)
		Attack();
	}
	// 自機に向かって歩く
	else {
		LockPlayer();
		Move();
	}

	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
}

void NormalEnemy::Attack()
{
	////後ろに移動
	//atackWork = AtackWork::Start;
	//if (atackWork == AtackWork::Start) {
	//	atackWork = AtackWork::Interval;
	//}
	//else if (atackWork == AtackWork::Interval) {
	//	atackWork = AtackWork::End;
	//}
	//else if (atackWork == AtackWork::End) {
	//	IsAttack_ = false;
	//}
	////貯める

	//突進
	if (attackWaitTime_ <= 0)
	{
#pragma region
		attackRotWork_.flag = true;
		Rot_ = models_[0].worldTF.rotation;
		attackRotWork_.targetRot = Rot_;
		EndRot_ = attackRotWork_.targetRot;
		// 回転を足す
		attackRotWork_.targetRot += lwp::Quaternion::CreateFromAxisAngle(lwp::Vector3{ 0,1,0 }, -3.14f);

		lwp::Vector3 point{ 0.0f,0.0f,-1.0f };
		attackMoveWork_.targetpoint = (point * lwp::Matrix4x4::CreateRotateXYZMatrix(models_[0].worldTF.rotation))/*ベクトルを反転*/;
		attackMoveWork_.targetpoint = attackMoveWork_.targetpoint.Normalize();
		attackMoveEndWork_.targetpoint = attackMoveWork_.targetpoint * -2/*ベクトルを反転*/;
#pragma endregion Body

		attackWaitTime_ = kAttackWaitTime_;
		LockPlayer();
	}

	lwp::Quaternion quat{};
	if (attackRotWork_.flag)
	{
		if (attackRotWork_.t < 1.0f)
		{
			// 回転
			attackRotWork_.t += attackRotWork_.speed;
			quat = LerpQuat(Rot_, attackRotWork_.targetpoint, attackRotWork_.t);
			models_[0].worldTF.rotation = quat;
			// 後ろに移動
			models_[0].worldTF.translation += attackMoveWork_.targetpoint * LWP::Info::GetDeltaTimeF() * 10.0f;

		}
		else if (attackRotWork_.t >= 1.0f)
		{
			attackRotWork_.flag = false;
			attackRotWork_.t = 0.0f;

			attackStanbyWork_.flag = true;
			LockPlayer();
			//collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
		}
	}
	if (attackStanbyWork_.flag)
	{
		attackStanbyWork_.t += attackStanbyWork_.speed;
		if (attackStanbyWork_.t >= 1.0f)
		{
			attackStanbyWork_.flag = false;
			attackStanbyWork_.t = 0.0f;

			attackEndWork_.flag = true;
		}
	}
	if (attackEndWork_.flag)
	{
		if (attackEndWork_.t < 1.0f)
		{
			attackEndWork_.t += attackEndWork_.speed;
			// 突進
			models_[0].worldTF.translation += attackMoveEndWork_.targetpoint * LWP::Info::GetDeltaTimeF() * 30.0f;
		}
		else if (attackEndWork_.t >= 1.0f)
		{
			attackEndWork_.flag = false;
			attackEndWork_.t = 0.0f;
			isAttack_ = false;
			//collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}
}

void NormalEnemy::DyingAnimation()
{
}
