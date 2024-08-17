#pragma once
#include <Adapter.h>
#include <string>
#include "Game/Object/Player/PlayerParameter.h"
//class PlayerParameter;
class IUpgrade
{
public:
	void Init() {
		sprite_.material.texture = LWP::Resource::LoadTexture(GetTexturePass());
		sprite_.isUI = true;
		sprite_.isActive = false;
		sprite_.anchorPoint = { 0.5f,0.5f };
		sprite_.material.enableLighting = false;
	};

	//アップデートを適用する
	void Apply(PlayerParameter* para) {
		Upgrade(para);
		isApplied = true;
	};

	/// <summary>
	/// UI用のテクスチャのパスを返す関数
	/// </summary>
	/// <returns>resources/texture/以降のパス</returns>
	virtual std::string GetTexturePass() = 0;

	/// <summary>
	/// アップグレードスキルの名前を返す関数
	/// </summary>
	/// <returns>TreeNode の名前になる</returns>
	virtual std::string GetUpgradeName() = 0;

	bool GetIsAppliedFlag() { return isApplied; };

	void SetUIPos(const LWP::Math::Vector2& pos) {
		// スプライトを描画させる
		sprite_.isActive = true;
		sprite_.worldTF.translation.x = pos.x;
		sprite_.worldTF.translation.y = pos.y;
	}

	void UIoff() {
		sprite_.isActive = false;
	}

private:
	bool isApplied = false;

protected: // ** 派生先用のメンバ変数 ** //

	//アップグレードの内容を決定
	virtual void Upgrade(PlayerParameter* Input) = 0;

	// スプライト
	LWP::Primitive::Sprite sprite_;
};