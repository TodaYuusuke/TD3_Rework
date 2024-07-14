#pragma once
#include <scene/IScene.h>

#include "Player/Player.h"
#include "Enemy/Maneger/EnemyManager.h"
#include "Enemy/Normal/NormalEnemy.h"

class PlayScene final : public IScene{
public:
	PlayScene() = default;	// コンストラクタは自由に定義可能
	~PlayScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//


public: //*** 関数群 ***//

private: //*** 変数群 ***//
	Player* player_;
	EnemyManager* enemyManager_;
};