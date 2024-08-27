#pragma once
#include "../IEnemy.h"
#include "../Arrow/Arrow.h"

class ArrowEnemy : public IEnemy
{
public:// パブリックなメンバ関数
	/// 
	/// Default Method
	/// 
	ArrowEnemy() : IEnemy(240.0f, 20.0f, 1.0f) {};
	~ArrowEnemy() {
		for (Arrow* arrow : arrows_) {
			delete arrow;
		}
	}

	void Init()override;
	void Update()override;
	void Attack()override;
	void DyingAnimation()override;

private:// メンバ変数
	// 矢
	std::list<Arrow*> arrows_;
};

