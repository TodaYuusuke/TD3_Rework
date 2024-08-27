#include "EnemyManager.h"
#include "Object/Player/Player.h"

void EnemyManager::Init(FollowCamera* followCamera) {
	// 追従カメラのアドレスを設定
	followCamera_ = followCamera;

	//NormalEnemySpawn(lwp::Vector3{ 0,0,10 });

	// 突進するボス
	//DashBoss* dashBoss = new DashBoss();
	//dashBoss->Init();
	//dashBoss->SetTarget(player_);
	//dashBoss->CreateSpawnDirection(dashBoss->GetWorldPosition());
	//enemys_.push_back(dashBoss);

	/*ArrowBoss* arrowBoss = new ArrowBoss();
	arrowBoss->Init();
	arrowBoss->SetFollowCamera(followCamera_);
	arrowBoss->SetTarget(player_);
	arrowBoss->CreateSpawnDirection(arrowBoss->GetWorldPosition());
	enemys_.push_back(arrowBoss);*/
}

void EnemyManager::Update() {

	//敵キャラのIsDeadがtrueなら削除
	enemys_.remove_if([this](IEnemy* enemy) {
		if (enemy->GetIsDead()) {
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
		enemy->ClearSpawnDirection();
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

#pragma region particleの解放処理
	deadParticles_.remove_if([this](DeadParticle* particle) {
		if (!particle->isActive) {
			delete particle;
			return true;
		}
		return false;
		});
	spawnParticles_.remove_if([this](SpawnParticle* particle) {
		if (!particle->isActive) {
			delete particle;
			return true;
		}
		return false;
		});
#pragma endregion
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
		ShieldEnemySpawn(pos);
		//ArrowEnemySpawn(pos);
	}
	else if (number <= 0.8f)
	{
		ShieldEnemySpawn(pos);
		//ArrowEnemySpawn(pos);
	}
	else
	{
		ShieldEnemySpawn(pos);
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
	enemy->CreateSpawnDirection(pos);
	CreateSpawnParticle(pos);

	enemys_.push_back(enemy);
}
//TODO: 生成する敵を指定のものに変更
void EnemyManager::ArrowEnemySpawn(lwp::Vector3 pos)
{
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Init();
	enemy->SetPosition(pos);
	enemy->SetTarget(player_);
	enemy->CreateSpawnDirection(pos);
	CreateSpawnParticle(pos);

	enemys_.push_back(enemy);
}

void EnemyManager::ShieldEnemySpawn(lwp::Vector3 pos)
{
	ShieldEnemy* enemy = new ShieldEnemy();
	enemy->Init();
	enemy->SetPosition(pos);
	enemy->SetTarget(player_);
	enemy->CreateSpawnDirection(pos);
	CreateSpawnParticle(pos);

	enemys_.push_back(enemy);
}

void EnemyManager::DashBossSpawn(lwp::Vector3 pos) {
	DashBoss* dashBoss = new DashBoss();
	dashBoss->Init();
	dashBoss->SetTarget(player_);
	dashBoss->CreateSpawnDirection(dashBoss->GetWorldPosition());
	CreateSpawnParticle(pos);

	enemys_.push_back(dashBoss);
}

void EnemyManager::ArrowBossSpawn(lwp::Vector3 pos) {
	ArrowBoss* arrowBoss = new ArrowBoss();
	arrowBoss->Init();
	arrowBoss->SetFollowCamera(followCamera_);
	arrowBoss->SetTarget(player_);
	arrowBoss->CreateSpawnDirection(arrowBoss->GetWorldPosition());
	CreateSpawnParticle(pos);

	enemys_.push_back(arrowBoss);
}

void EnemyManager::CreateDeadParticle(lwp::Vector3 pos) {
	DeadParticle* deadParticle = new DeadParticle();
	deadParticle->model.LoadCube();
	deadParticle->model.worldTF.translation = pos;
	deadParticle->Add(64);
	deadParticles_.push_back(deadParticle);
}

void EnemyManager::CreateSpawnParticle(lwp::Vector3 pos) {
	SpawnParticle* spawnParticle = new SpawnParticle();
	spawnParticle->model.LoadCube();
	spawnParticle->model.worldTF.translation = pos;
	spawnParticle->Add(32);
	spawnParticles_.push_back(spawnParticle);
}