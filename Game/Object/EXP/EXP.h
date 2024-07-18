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

	// サイズ
	const float kSize_ = 0.25f;
};


