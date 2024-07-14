#include "EnemyManager.h"

void EnemyManager::Init(){
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Init();
	enemy->SetTarget(player_);
	enemys_.push_back(enemy);
}

void EnemyManager::Update(){

	//敵キャラのIsDeadがtrueなら削除
	enemys_.remove_if([this](IEnemy* enemy){
		if (enemy->GetIsDead())
		{
			delete enemy;
			return true;
		}
		return false;
	});

	//enemyの更新処理
	for (IEnemy* enemy : enemys_)
	{
		enemy->Update();
	}
}
