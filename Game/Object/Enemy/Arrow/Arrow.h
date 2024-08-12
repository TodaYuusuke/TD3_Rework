#pragma once
#include <Adapter.h>
#include "Object/Enemy/IEnemy.h"
#include "Object/Player/Player.h"

using namespace LWP::Object::Collider;

class Arrow
{
public:
	~Arrow() = default;
	void Init(lwp::TransformQuat transform);
	void Update();
	void Attack();

	// �z�[�~���O�̍X�V����
	void HomingUpdate();

	//*** Getter ***//


	//*** Setter ***//
	void SetIsCollision(bool isActive) { aabb_.isActive = isActive; }
	// ������ݒ�
	void SetRotate(LWP::Math::Vector3 rotate) { model_.worldTF.rotation = rotate; }
	// ���x��ݒ�
	void SetVelocity(LWP::Math::Vector3 velocity) { velocity_ = velocity; }
	// ���@�̃A�h���X��ݒ�
	void SetPlayer(Player* player) { player_ = player; }

public:
	void Death();

private:// �萔
	// �e�̎���
	const float kLifeTime = 600;

	// �ʏ�e�̑��x
	const float kNormalSpeed = 10.0f;

private:
	//���f���C���X�^���X
	LWP::Resource::RigidModel model_;
	// ���@�̃A�h���X
	Player* player_;

	// AABB����
	AABB aabb_;

	// �e�̎���
	float deadTimer_;

	// �����x�N�g��
	LWP::Math::Vector3 velocity_;
};