#pragma once
#include <Adapter.h>

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetWorldPosition() { return model_.worldTF.GetWorldPosition(); }

private: //*** メンバ変数 ***//

	// モデル
	LWP::Resource::RigidModel model_;
};