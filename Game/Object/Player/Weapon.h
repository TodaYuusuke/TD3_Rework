#pragma once

#include "Adapter.h"

#include <optional>
#include <array>

class Weapon final
{
public:

	enum class Behavior : unsigned int
	{
		Root = 0u,	// 待機状態
		Slash,		// 居合
		Moment,		// 後隙

		_COUNT,		// カウント用
	};

public: //*** パブリック関数 ***//

	// コンストラクタ
	Weapon() = default;
	// デストラクタ
	~Weapon() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();

public:	//*** セッター,ゲッター ***//

	lwp::TransformQuat* GetWorldTransform() { return &model_.worldTF; }
	void SetParent(lwp::TransformQuat* p) { model_.worldTF.Parent(p); }

	void SetIsActive(bool flag) { model_.isActive = flag; }

	void SetBehavior(Behavior b) { reqBehavior_ = b; }

	// 武器のモデル取得
	LWP::Resource::RigidModel* GetMesh() { return &model_; }

private: //*** プライベート関数 ***//

	void InitRoot();
	void InitSlash();
	void InitMoment();

	void UpdateRoot();
	void UpdateSlash();
	void UpdateMoment();

private: //*** プライベート変数 ***//


	//*** 計算用 ***//

	// モデル
	LWP::Resource::RigidModel model_;

	lwp::Vector3 prePos_ = { 0.0f,0.0f,0.0f };

	// イージング用
	float t_ = 0.0f;

	// 状態
	Behavior behavior_ = Behavior::Root;
	std::optional<Behavior> reqBehavior_ = std::nullopt;


};

