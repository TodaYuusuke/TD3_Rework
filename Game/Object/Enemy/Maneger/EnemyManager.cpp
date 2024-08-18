#include "EnemyManager.h"
#include "Object/Player/Player.h"

void EnemyManager::Init(FollowCamera* followCamera){
	gameTimer_ = GameTimer::GetInstance();
	// 追従カメラのアドレスを設定
	followCamera_ = followCamera;
	// 突進するボス
	//DashBoss* dashBoss = new DashBoss();
	//dashBoss->Init();
	//dashBoss->SetTarget(player_);
	//enemys_.push_back(dashBoss);
	
	//ArrowBoss* arrowBoss = new ArrowBoss();
	//arrowBoss->Init();
	//arrowBoss->SetFollowCamera(followCamera_);
	//arrowBoss->SetTarget(player_);
	//enemys_.push_back(arrowBoss);
	NormalEnemySpawn({0.0f,0.0f,10.0f});
}

void EnemyManager::Update(){

	//敵キャラのIsDeadがtrueなら削除
	enemys_.remove_if([this](IEnemy* enemy){
		if (enemy->GetIsDead())	{
			// 死亡パーティクル発生
			CreateDeadParticle(enemy->GetWorldPosition());
			delete enemy;
			return true;
		}
		return false;
	});
	//enemyの更新処理
	for (IEnemy* enemy : enemys_)
	{
		enemy->Update();
		enemy->ImGui();
		enemy->CommonUpdate();
	}

	currentFrame_++;

	// 通常敵の出現
	if (currentFrame_ >= kSpawnFrequency) {
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		std::uniform_int_distribution<int> distribution(1, 3);
		int spawn = distribution(randomEngine);
		for (int It = 0; It < spawn; It++) {
			EnemySpawn();
		}
		currentFrame_ = 0;
	}
}

void EnemyManager::EnemySpawn()
{
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	//eds
	float number = distribution(randomEngine);

	lwp::Vector3 pos = genalateRumdomPos();

	if (number <= 0.5f)
	{
		//NormalEnemySpawn(pos);
		//ArrowEnemySpawn(pos);
	}
	else if (number <= 0.8f)
	{
		//NormalEnemySpawn(pos);
		//ArrowEnemySpawn(pos);
	}
	else
	{
		//NormalEnemySpawn(pos);
		//ShieldEnemySpawn(pos);
	}
}

lwp::Vector3 EnemyManager::genalateRumdomPos()
{
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	//方向を決める
	float divideX = distribution(randomEngine);
	float divideZ = 1.0f - divideX;
	float signX = distribution(randomEngine);
	float signY = distribution(randomEngine);
	if (signX <= 0.5f)
	{
		signX = 1;
	}
	else
	{
		signX = -1;
	}
	if (signY <= 0.5f)
	{
		signY = 1;
	}
	else
	{
		signY = -1;
	}

	std::uniform_real_distribution<float> distribution2(8.0f, 10.0f);
	float PtoE = distribution2(randomEngine);

	lwp::Vector3 pos = { PtoE * divideX * signX * 5 , 0.5f , PtoE * divideZ * signY * 5 };

	return pos;
}

void EnemyManager::NormalEnemySpawn(lwp::Vector3 pos)
{
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Init();
	enemy->SetPosition(pos);
	enemy->SetTarget(player_);
	enemy->SetEXPManager(expManager_);
	enemys_.push_back(enemy);
}
//TODO: 生成する敵を指定のものに変更
void EnemyManager::ArrowEnemySpawn(lwp::Vector3 pos)
{
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Init();
	enemy->SetPosition(pos);
	enemy->SetTarget(player_);
	enemy->SetEXPManager(expManager_);
	enemys_.push_back(enemy);
}

void EnemyManager::ShieldEnemySpawn(lwp::Vector3 pos)
{
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Init();
	enemy->SetPosition(pos);
	enemy->SetTarget(player_);
	enemy->SetEXPManager(expManager_);
	enemys_.push_back(enemy);
}

void EnemyManager::CreateDeadParticle(lwp::Vector3 pos) {
	DeadParticle* deadParticle = new DeadParticle();
	deadParticle->model.LoadCube();
	deadParticle->model.worldTF.translation = pos;
	deadParticle->Add(64);
	deadParticles_.push_back(deadParticle);
}