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

private:

	// ボスも含めたすべての敵
	std::list<IEnemy*> enemys_;
	// 自機
	Player* player_;

};
