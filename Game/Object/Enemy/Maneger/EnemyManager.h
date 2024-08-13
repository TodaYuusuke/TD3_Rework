#pragma once

#include "../Normal/NormalEnemy.h"
#include "../Boss/DashBoss.h"
#include "../Boss/ArrowBoss.h"
#include "../../Particle/DeadParticle.h"

class Player;
class EnemyManager
{
public:
	EnemyManager(Player* player) {
		player_ = player;
	};
	~EnemyManager() {
		for (IEnemy* enemy : enemys_)
		{
			delete enemy;
		}
	};

	void Init();
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

	// 現在のフレーム
	int currentFrame_ = 0;

	// ボスも含めたすべての敵
	std::list<IEnemy*> enemys_;
	// 自機
	Player* player_;

	// 死亡パーティクル
	std::vector<DeadParticle*> deadParticle_;
};
