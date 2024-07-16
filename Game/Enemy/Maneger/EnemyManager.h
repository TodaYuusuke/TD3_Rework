#pragma once

#include "Game/Enemy/IEnemy.h"
#include "Game/Enemy/Normal/NormalEnemy.h"

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

};
