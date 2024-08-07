#pragma once
#include <scene/IScene.h>

#include "Object/Player/Player.h"
#include "Object/Enemy/Maneger/EnemyManager.h"
#include "Object/Enemy/Normal/NormalEnemy.h"

class GamePlayScene final : public IScene{
public:
	GamePlayScene();	// コンストラクタは自由に定義可能
	~GamePlayScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//


public: //*** 関数群 ***//

private: //*** 変数群 ***//
	Player player_;
	EnemyManager enemyManager_;

	// 太陽
	LWP::Object::PointLight sun_;
};