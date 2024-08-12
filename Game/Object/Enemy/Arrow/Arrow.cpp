#include "Arrow.h"

void Arrow::Init(lwp::TransformQuat transform)
{
	// ���f���̍쐬
	//model_.LoadFile("Arrow/Arrow.obj");
	model_.LoadCube();
	model_.worldTF.translation = transform.translation;
	model_.worldTF.rotation = transform.rotation;
	model_.materials[1].enableLighting = true;

	// �����蔻���ݒ�
	//aabb_ = new LWP::Object::Collider::AABB();
	//aabb_.CreateFromPrimitive(&model_);
	//aabb_.mask.SetBelongFrag(GameMask::EnemyAttack());
	//aabb_.mask.SetHitFrag(GameMask::Player() | GameMask::Weapon());
	//aabb_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {
	//	data;
	//	if (!(data.state == OnCollisionState::None) &&
	//		data.hit &&
	//		(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
	//	{
	//		Death();
	//	}
	//	});

	//aabb_.isActive = true;
}

void Arrow::Update()
{
	// �ړ�����
	Attack();

	// �e���������Ԃ��z���Ă����玀��
	if (deadTimer_ >= kLifeTime)
	{
		Death();
	}
	// ������i�߂�
	deadTimer_++;
}

void Arrow::Attack()
{
	//// �e�������Ă�������ɓ�������
	//LWP::Math::Vector3 velocity = { 0,0,1 };
	//LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
	//velocity = velocity * rotateMatrix;
	//velocity_ = velocity.Normalize();
	//velocity_ *= kNormalSpeed;

	// ���@�Ɍ������x�N�g��
	LWP::Math::Vector3 targetVel = (player_->GetWorldPosition() - model_.worldTF.translation).Normalize();
	// �_���Ώۂɐg�̂�������
	lwp::Vector3 sub = targetVel - model_.worldTF.translation;

	//�v���C���[�̌��݂̌���
	sub = sub.Normalize();

	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,0.0f,1.0f }, sub).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

	//�s������������Quaternion�̍쐬
	model_.worldTF.rotation = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));



	// ���Z
	//model_.worldTF.translation += velocity_ * LWP::Info::GetDeltaTime();
}

void Arrow::Death() {
	
}