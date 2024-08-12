#pragma once
#include "Object/Enemy/IEnemy.h"

class ArrowBoss : public IEnemy
{
private:// �\����
	// �U��܂�
	enum class Behavior {
		kRoot,	 // �ʏ���
		kAiming, // ���@��_��
		kNormalShot,   // ����
		kHomingShot  // �z�[�~���O�e
	};

public:// �p�u���b�N�ȃ����o�֐�
	//*** �������z�֐� ***//
	~ArrowBoss() override;
	void Init()override;
	void Update()override;

private:// �v���C�x�[�g�ȃ����o�֐�
	//*** �������z�֐� ***//
	// �U��
	void Attack()override;

	// �S�Ă̒e�̍X�V����
	void ArrowsUpdate();

	// �e�̔��ˊp�������_���ɂ���
	LWP::Math::Vector3 RandomShootingAngle();

#pragma region �U��܂�
	// �ʏ��Ԃ̏�����
	void B_RootInit();
	// �ʏ��Ԃ̍X�V����
	void B_RootUpdate();

	// ���@��_����Ԃ̏�����
	void B_AimingInit();
	// ���@��_����Ԃ̍X�V����
	void B_AimingUpdate();

	// �ˌ���Ԃ̏�����
	void B_NormalShotInit();
	// �ˌ���Ԃ̍X�V����
	void B_NormalShotUpdate();

	// �z�[�~���O�ˌ���Ԃ̏�����
	void B_HomingShotInit();
	// �z�[�~���O�ˌ���Ԃ̍X�V����
	void B_HomingShotUpdate();
#pragma endregion

private:// �萔
#pragma region �s���̃t���[�����
	// �������Ȃ���Ԃ̑S�̃t���[��
	const float kStunAllFrame = 120;
	// ���@�_����Ԃ̑S�̃t���[��
	const float kAimAllFrame = 120;
	// �ʏ�ˌ���Ԃ̑S�̃t���[��
	const float kNormalShotAllFrame = 180;
	// �z�[�~���O�ˌ��̑S�̃t���[��
	const float kHomingShotAllFrame = 180;

	// �ʏ�e�̘A�ˑ��x(���̒e�����܂ł̎���)
	const float kNormalShotDelayFrame = 0;
	// �z�[�~���O�e�̘A�ˑ��x(���̒e�����܂ł̎���)
	const float kHomingShotDelayFrame = 6;
#pragma endregion

	// �ʏ�e��
	const int kMaxNormalShotCount = 4;
	// �z�[�~���O�e��
	const int kMaxHomingShotCount = 9;

	// �U������͈�
	const float kAttackRange = 60.0f;

	// ��Z���s���Ƃ��̃J�����̎���p
	const float kEffectFov = 120;

private:// �v���C�x�[�g�ȕϐ�
	// �������Ȃ���Ԃ̌o�߃t���[��
	float stunFrame_;
	// �ʏ�ˌ���Ԃ̌o�߃t���[��
	float shotFrame_;

	// �ˌ��̃f�B���C
	float shotDelay_;
	// �ˌ���
	int shotCount_;

	// ���@��_����ԂɂȂ邩
	bool isAiming_;

	// ���݂̐U��܂�
	Behavior behavior_ = Behavior::kRoot;
	// ���̐U��܂����N�G�X�g
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// �ʏ�e
	//std::list<Arrow*> normalArrows_;
	// �z�[�~���O�e
	//std::list<HomingArrow*> homingArrows_;
};