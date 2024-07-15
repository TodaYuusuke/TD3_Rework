#pragma once
#include <scene/IScene.h>

#include "Particle/DamageParticle.h"
#include "Particle/DeadParticle.h"
#include "Particle/SpawnParticle.h"
#include "Particle/AccumulateParticle.h"

class TestScene final
	: public IScene
{
public:
	TestScene() = default;	// コンストラクタは自由に定義可能
	~TestScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入


private: //*** 変数群 ***//

	// パーティクルテスト
	DamageParticle damageParticle_;
	DeadParticle deadParticle_;
	SpawnParticle spawnParticle_;
	AccumulateParticle accumulateParticle_;
};