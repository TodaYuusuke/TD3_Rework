#pragma once

#include "../Normal/NormalEnemy.h"
#include "../Boss/DashBoss.h"
#include "../Boss/ArrowBoss.h"
#include "../../Particle/DeadParticle.h"
#include "../../Particle/SpawnParticle.h"
#include "../../FollowCamera/FollowCamera.h"

class Player;
class EnemyManager
{
public:
	EnemyManager(Player* player) { player_ = player; };
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
	// 突進ボスの生成
	void DashBossSpawn(lwp::Vector3 pos);
	// 遠距離ボスの生成
	void ArrowBossSpawn(lwp::Vector3 pos);

	// 死亡パーティクルの生成
	void CreateDeadParticle(lwp::Vector3 pos);
	// 登場パーティクルの生成
	void CreateSpawnParticle(lwp::Vector3 pos);

private:// 定数
	// 敵発生頻度
	const int kSpawnFrequency = 120;

private:// 変数
	// 追従カメラのアドレス
	FollowCamera* followCamera_;

	// ボスも含めたすべての敵
	std::list<IEnemy*> enemys_;
	// 自機
	Player* player_;

	// 死亡パーティクル
	std::list<DeadParticle*> deadParticles_;
	std::list<SpawnParticle*> spawnParticles_;

	// 現在のフレーム
	int currentFrame_ = 0;
};
