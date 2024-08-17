#include "Weapon.h"
void Weapon::Initialize()
{
	// モデル読み込み
	model_.LoadShortPath("Weapon/Weapon.obj");
	model_.isActive = true;

	model_.worldTF.translation.z = 1.0f;

	behavior_ = Behavior::Root;
}

void Weapon::Update()
{
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		t_ = 0.0f;
		prePos_ = model_.worldTF.translation;
		switch (behavior_)
		{
		case Weapon::Behavior::Root:
			InitRoot();
			break;
		case Weapon::Behavior::Slash:
			InitSlash();
			break;
		case Weapon::Behavior::Moment:
			InitMoment();
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}
	switch (behavior_)
	{
	case Weapon::Behavior::Root:
		// アニメーション来るまで放置
		UpdateRoot();
		break;
	case Weapon::Behavior::Slash:
		UpdateSlash();
		break;
	case Weapon::Behavior::Moment:
		UpdateMoment();
		break;
	default:
		break;
	}
	t_ += lwp::GetDeltaTimeF() * 2;
	t_ = std::clamp<float>(t_, 0.0f, 1.0f);
}

void Weapon::InitRoot()
{
	model_.worldTF.translation = { 0.0f,0.0f,1.0f };
}

void Weapon::InitSlash()
{
}

void Weapon::InitMoment()
{
}

void Weapon::UpdateRoot()
{
	if (lwp::Keyboard::GetTrigger(DIK_SPACE) ||
		lwp::Pad::GetTrigger(XINPUT_GAMEPAD_A, 0)) {
		reqBehavior_ = Weapon::Behavior::Slash;
	}
}

void Weapon::UpdateSlash()
{
	float easeT = LWP::Utility::Easing::OutExpo(t_);
	model_.worldTF.translation.z = (10.0f * (easeT)) + (prePos_.z * (1.0f - easeT));
	if (easeT >= 1.0f) {
		reqBehavior_ = Weapon::Behavior::Moment;
	}
}

void Weapon::UpdateMoment()
{
	float easeT = LWP::Utility::Easing::InOutQuart(t_);
	model_.worldTF.translation = (lwp::Vector3(0.0f, 0.0f, 1.0f) * (easeT)) + (prePos_ * (1.0f - easeT));
	if (easeT >= 1.0f) {
		reqBehavior_ = Weapon::Behavior::Root;
	}
}