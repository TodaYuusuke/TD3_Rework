#pragma once
#include <Adapter.h>
#include <string>
class PlayerParameter;
class IUpgrade
{
public:

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
private:
	bool isApplied = false;

protected: // ** 派生先用のメンバ変数 ** //

	//アップグレードの内容を決定
	virtual void Upgrade(PlayerParameter* para) = 0;

	// スプライト
	LWP::Primitive::Sprite sprite_;
};