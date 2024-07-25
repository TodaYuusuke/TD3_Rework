#pragma once
#include <Adapter.h>
class EXP
{
public:
	EXP(){};
	~EXP(){};

	void Init(lwp::Vector3 position);

	void Update();

private:
	LWP::Resource::RigidModel model_;

	lwp::Vector3 Rotate_;
	float rotateSpeed_ = 0.3f;

	// サイズ
	const float kSize_ = 0.25f;
};


