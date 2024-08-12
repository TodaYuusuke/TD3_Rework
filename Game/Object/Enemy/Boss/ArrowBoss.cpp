#include "ArrowBoss.h"
#include "Object/Player/Player.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Object::Collider;

ArrowBoss::~ArrowBoss() {
	/*for (Arrow* arrow : normalArrows_) {
		delete arrow;
	}

	for (HomingArrow* arrow : homingArrows_) {
		delete arrow;
	}

	normalArrows_.clear();
	homingArrows_.clear();*/
}

void ArrowBoss::Init()
{
	// �����蔻��̃C���X�^���X����
	models_.reserve(1);
	models_.emplace_back();
	models_[0].LoadCube();
	models_[0].materials[1].enableLighting = false;

	// �傫��
	models_[0].worldTF.scale = { 2,3,2 };

	shotCount_ = 0;

	hp_ = 200;
}

void ArrowBoss::Update()
{
	// ���S���A�j���[�V����
	// ���񂾂��ǂ����͂����ɔ���
	if (IsDead_) {
		Dying();
		DyingAnimation();
		return;
	}

	// ������
	if (behaviorRequest_) {
		//  �U��܂���ύX
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			B_RootInit();
			break;
			// ���@��_��
		case Behavior::kAiming:
			B_AimingInit();
			break;
			// ����
		case Behavior::kNormalShot:
			B_NormalShotInit();
			break;
			// �z�[�~���O�e
		case Behavior::kHomingShot:
			B_HomingShotInit();
		}
		// �U��܂����N�G�X�g�����Z�b�g
		behaviorRequest_ = std::nullopt;
	}
	// �X�V����
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		B_RootUpdate();
		break;
		// ���@��_��
	case Behavior::kAiming:
		B_AimingUpdate();
		break;
		// �ʏ�ˌ�
	case Behavior::kNormalShot:
		B_NormalShotUpdate();
		// �z�[�~���O�e
	case Behavior::kHomingShot:
		B_HomingShotUpdate();
		break;
	}

	// �S�Ă̒e�̍X�V����
	ArrowsUpdate();
}

void ArrowBoss::Attack()
{
	// ���@��_��
	LockPlayer();
#pragma region �ʏ�e
	if (behavior_ == Behavior::kNormalShot) {
		// �E�ɔ��ł����e
		for (int i = 0; i < 2; i++) {
			// �e�𐶐�
			//HomingArrow* homingArrow = new HomingArrow(
			//	[this](Math::Vector3 pos) {
			//		// ���W��ύX���ăp�[�e�B�N���������
			//		*missileContrail_.Transform() = pos;
			//		missileContrail_.Add(1);
			//	}
			//);
			//// ��ɒe������(x,y���������_���ŏ�����])
			//LWP::Math::Vector3 rotate = { -M_PI / 10.0f * (i + 1), 2.0f * M_PI / 3.0f, 0 };
			//homingArrow->SetShootingAngle(rotate);
			//// �z�[�~���O�J�n����
			//homingArrow->SetHomingStartFrame(30);

			//// ���@�̃A�h���X��ݒ�
			//homingArrow->SetPlayer(player_);

			//// ������
			//homingArrow->Init(models_[0].transform);

			//homingArrows_.push_back(homingArrow);
		}

		// ���ɔ��ł����e
		for (int i = 0; i < 2; i++) {
			// �e�𐶐�
			//HomingArrow* homingArrow = new HomingArrow(
			//	[this](Math::Vector3 pos) {
			//		// ���W��ύX���ăp�[�e�B�N���������
			//		*missileContrail_.Transform() = pos;
			//		missileContrail_.Add(1);
			//	}
			//);
			//// ��ɒe������(x,y���������_���ŏ�����])
			//LWP::Math::Vector3 rotate = { -M_PI / 10.0f * (i + 1), -2.0f * M_PI / 3.0f, 0 };
			//homingArrow->SetShootingAngle(rotate);
			//// �z�[�~���O�J�n����
			//homingArrow->SetHomingStartFrame(30);

			//// ���@�̃A�h���X��ݒ�
			//homingArrow->SetPlayer(player_);

			//// ������
			//homingArrow->Init(models_[0].transform);

			//homingArrows_.push_back(homingArrow);
		}
	}
#pragma endregion

#pragma region �z�[�~���O�e
	else if (behavior_ == Behavior::kHomingShot) {
		// �e�𐶐�
		//HomingArrow* homingArrow = new HomingArrow(
		//	[this](Math::Vector3 pos) {
		//		// ���W��ύX���ăp�[�e�B�N���������
		//		*missileContrail_.Transform() = pos;
		//		missileContrail_.Add(1);
		//	}
		//);
		//// ��ɒe������(x,y���������_���ŏ�����])
		//LWP::Math::Vector3 rotate = RandomShootingAngle();
		//homingArrow->SetShootingAngle(rotate);

		//// ���@�̃A�h���X��ݒ�
		//homingArrow->SetPlayer(player_);

		//// ������
		//homingArrow->Init(models_[0].transform);

		//homingArrows_.push_back(homingArrow);
	}
#pragma endregion 
	IsAttack_ = false;
}

void ArrowBoss::ArrowsUpdate() {
	// �ʏ�e�̍X�V����
	/*for (Arrow* arrow : normalArrows_) {
		arrow->Update();
	}
	normalArrows_.remove_if([](Arrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		return false;
		});*/

	// �z�[�~���O�e�̍X�V����
	/*for (HomingArrow* arrow : homingArrows_) {
		arrow->Update();
	}
	homingArrows_.remove_if([](HomingArrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		return false;
		});*/
}

LWP::Math::Vector3 ArrowBoss::RandomShootingAngle() {
	//�����_�������p
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	// x,y���������_���ɉ�]
	std::uniform_real_distribution<float> distributionX(M_PI / 2.5f, 1.5f * M_PI / 2.5f);// 45~135�x
	std::uniform_real_distribution<float> distributionY(0, 2 * M_PI);				   // 0~360�x
	LWP::Math::Vector3 rotate = { -distributionX(randomEngine), distributionY(randomEngine), 0 };

	return rotate;
}

/// Behavior�֐��������火
void ArrowBoss::B_RootInit() {
	// �U����ԉ���
	IsAttack_ = false;
	// ���@�_����Ԃ̃N�[���^�C��
	stunFrame_ = kStunAllFrame;

	//followCamera_->EndEffectFov();
}
void ArrowBoss::B_RootUpdate() {
	// �U���̑҂�����
	if (stunFrame_ <= 0) {
		isAiming_ = true;
	}

	// �U���͈͓��ɂ��邩
	if (CheckAttackRange()) {
		// �U���\��Ԃ�
		if (isAiming_) {
			behaviorRequest_ = Behavior::kAiming;
		}
	}
	else {
		// �ړ�����
		Move();
	}

	stunFrame_--;
}

void ArrowBoss::B_AimingInit() {
	// ���@�_����ԊJ�n
	isAiming_ = true;
	attackWaitTime_ = (int)kAimAllFrame;
}
void ArrowBoss::B_AimingUpdate() {
	// �̂����@�Ɍ�����
	LockPlayer();

	// ����̎��Ԃ��߂�����s���J�n
	if (attackWaitTime_ <= 0) {
		int randomBehavior = GenerateRandamNum(2, 3);
		behaviorRequest_ = static_cast<Behavior>(randomBehavior);//Behavior::kNormalShot;
	}

	attackWaitTime_--;
}

void ArrowBoss::B_NormalShotInit() {
	// ���@�_����ԉ���
	isAiming_ = false;
	// �U���J�n
	IsAttack_ = true;
	// �ˌ��̑S�̃t���[��
	shotFrame_ = kNormalShotAllFrame;
	// �ˌ��̃f�B���C
	shotDelay_ = kNormalShotDelayFrame;
	// �ˌ���
	shotCount_ = kMaxNormalShotCount;
}
void ArrowBoss::B_NormalShotUpdate() {
	// �ˌ��Ԋu
	if (shotDelay_ <= 0 && shotCount_ >= 1) {
		Attack();
		shotDelay_ = kNormalShotDelayFrame;
		shotCount_--;
	}
	// ����̎��Ԃ��߂�����U���I��
	if (shotFrame_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
	}

	shotDelay_--;
	shotFrame_--;
}

void ArrowBoss::B_HomingShotInit() {
	// ���@�_����ԉ���
	isAiming_ = false;
	// �U���J�n
	IsAttack_ = true;
	// �ˌ��̑S�̃t���[��
	shotFrame_ = kHomingShotAllFrame;
	// �ˌ��̃f�B���C
	shotDelay_ = kNormalShotDelayFrame;
	// �ˌ���
	shotCount_ = kMaxHomingShotCount;

	// ����p����������
	//followCamera_->StartEffectFov(kEffectFov);
}
void ArrowBoss::B_HomingShotUpdate() {
	// �ˌ��Ԋu
	if (shotDelay_ <= 0 && shotCount_ >= 1) {
		Attack();
		shotDelay_ = kHomingShotDelayFrame;
		shotCount_--;
	}
	// ����̎��Ԃ��߂�����U���I��
	if (shotFrame_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
	}

	shotDelay_--;
	shotFrame_--;
}