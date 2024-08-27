#include "ShieldEnemy.h"

void ShieldEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	attackWaitTime_ = kAttackWaitTime_;

	// HP を設定
	hp_ = 40;
}

void ShieldEnemy::Update() {
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

void ShieldEnemy::Attack() {
	if (attackWaitTime_ <= 0)
	{
		attackWork_.flag = true;
		lwp::Vector3 point{ 0.0f,0.0f,-1.0f };
		attackWork_.targetpoint = (point * lwp::Matrix4x4::CreateRotateXYZMatrix(models_[0].worldTF.rotation))/*ベクトルを反転*/;
		attackWork_.targetpoint = attackWork_.targetpoint.Normalize();
		attackEndWork_.targetpoint = attackWork_.targetpoint * -1/*ベクトルを反転*/;
		attackWaitTime_ = kAttackWaitTime_;

		LockPlayer();
	}

	if (attackWork_.flag) {
		if (attackWork_.t < 1.0f) {
			attackWork_.t += attackWork_.speed;
			models_[0].worldTF.translation += attackWork_.targetpoint * LWP::Info::GetDeltaTimeF();
		}
		else if (attackWork_.t >= 1.0f) {
			attackWork_.flag = false;
			attackWork_.t = 0.0f;

			attackStanbyWork_.flag = true;
			//collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
		}
	}
	if (attackStanbyWork_.flag) {
		attackStanbyWork_.t += attackStanbyWork_.speed;
		if (attackStanbyWork_.t >= 1.0f) {
			attackStanbyWork_.flag = false;
			attackStanbyWork_.t = 0.0f;

			attackEndWork_.flag = true;
		}
	}
	if (attackEndWork_.flag) {
		if (attackEndWork_.t < 1.0f) {
			attackEndWork_.t += attackEndWork_.speed;

			models_[0].worldTF.translation += attackEndWork_.targetpoint * LWP::Info::GetDeltaTimeF() * 10.0f;
		}
		else if (attackEndWork_.t >= 1.0f) {
			attackEndWork_.flag = false;
			attackEndWork_.t = 0.0f;
			isAttack_ = false;
			//collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}
}

void ShieldEnemy::DyingAnimation()
{
}
