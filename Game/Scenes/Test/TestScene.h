#pragma once
#include <scene/IScene.h>
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Normal/NormalEnemy.h"
#include "../../Object/FollowCamera/FollowCamera.h"
//#include "../../SceneTransition/SceneTransition.h"

class TestScene final
	: public IScene
{
public:
	TestScene() = default;	// コンストラクタは自由に定義可能
	~TestScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

public: //*** 関数群 ***//

private: //*** 変数群 ***//
	Player player_;
	FollowCamera followCamera_;
	NormalEnemy enemy_;
};