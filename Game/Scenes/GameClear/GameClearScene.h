#pragma once
#include <scene/IScene.h>

class GameClearScene final : public IScene {
public:
	GameClearScene() = default;	// コンストラクタは自由に定義可能
	~GameClearScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//

public: //*** 関数群 ***//

private: //*** 変数群 ***//

};