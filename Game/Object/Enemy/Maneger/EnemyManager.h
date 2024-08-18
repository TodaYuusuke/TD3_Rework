#pragma once

#include "../Normal/NormalEnemy.h"
#include "../Boss/DashBoss.h"
#include "../Boss/ArrowBoss.h"
#include "../../Particle/DeadParticle.h"
#include "../../FollowCamera/FollowCamera.h"
#include "Game/Object/GameTimer/GameTimer.h"

class Player;
class EnemyManager
{
public:
	EnemyManager(Player* player,EXPManager* expManager) {
		player_ = player;
		expManager_ = expManager;
	};
	~EnemyManager() {
		for (IEnemy* enemy : enemys_)
		{
			delete enemy;
		}
		deadParticles_.clear();
	};

	void Init(FollowCamera* followCamera);
	void Update();

private:// 関数

	void EnemySpawn();

	lwp::Vector3 genalateRumdomPos();

	void NormalEnemySpawn(lwp::Vector3 pos);
	void ArrowEnemySpawn(lwp::Vector3 pos);
	void ShieldEnemySpawn(lwp::Vector3 pos);

	// 死亡パーティクルの生成
	void CreateDeadParticle(lwp::Vector3 pos);

private:// 定数

	// 敵発生頻度
	const int kSpawnFrequency = 120;

private:// 変数
	// ゲームタイマー
	GameTimer* gameTimer_;
	// 追従カメラのアドレス
	FollowCamera* followCamera_;

	// ボスも含めたすべての敵
	std::list<IEnemy*> enemys_;
	// 自機
	Player* player_;

	// 死亡パーティクル
	std::vector<DeadParticle*> deadParticles_;

	// 現在のフレーム
	int currentFrame_ = 0;
	EXPManager* expManager_;
};
