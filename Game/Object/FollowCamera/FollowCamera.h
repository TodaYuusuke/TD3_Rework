#pragma once
#include "../Adapter/Adapter.h"

class Player;
class FollowCamera {
public:
	/// 
	/// Default Method
	/// 

	// コンストラクタ
	FollowCamera();
	~FollowCamera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="target">追従対象</param>
	/// <param name="pCamera">メインカメラのアドレス</param>
	/// <param name="player">自機</param>
	void Init(lwp::Camera* pCamera);

	// 更新処理
	void Update();

	/// 
	/// User Method
	/// 

	/// <summary>
	/// fovの演出開始
	/// </summary>
	/// <param name="goalFov">fovの最終的な値</param>
	void StartFovDirection(float goalFov) { goalFov_ = goalFov; }

	// fovの演出終了
	void EndFovDirection() { goalFov_ = 90; }

	// 向きをリセットする
	void ResetAngle();

#pragma region Getter
	/// <summary>
	/// カメラのクォータニオンを取得
	/// </summary>
	LWP::Object::TransformQuat GetTransformQuat() const { return pCamera_->transform; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 追従対象を設定
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const LWP::Object::TransformQuat* target) { target_ = target; }
#pragma endregion

private:/// プライベートな関数
	// キーボード,ゲームパッドの入力処理
	void InputAngle();

	///
	/// 数学関数
	///

	// 指数補間
	float ExponentialInterpolate(const float& current, const float& target, float damping, float deltaTime);
	// オフセットの計算
	LWP::Math::Vector3 CalcOffset() const;

	float LerpShortAngle(float a, float b, float t);

private:/// 定数
	///
	/// 視点操作の設定
	/// 

	// x軸の最低値
	const float kMinAxisX = 0.2f;
	// x軸の最大値
	const float kMaxAxisX = 0.4f;
	// 入力感度
	const LWP::Math::Vector2 kSensitivity = { 0.05f, 0.02f };
	// 視点移動の滑らかさ(0~1の間で設定)
	const float kRotationSmoothness = 0.2f;
	// カメラの後追い速度(0~1の間で設定)
	const float kFollowRate = 1.0f;

	// 追従対象との距離
	const LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,-60.0f };

	// 初期角度 
	LWP::Math::Vector3 kStartAngle = { 0.3f, 0.0f, 0.0f };

public:/// パブリックな変数
	LWP::Object::Camera* pCamera_;

private:/// プライベートな変数
	// 自機のアドレス
	Player* player_;

	// 追従対象
	const LWP::Object::TransformQuat* target_ = nullptr;
	// 追従対象の残像座標
	LWP::Math::Vector3 interTarget_;
	// 目標角度
	LWP::Math::Vector2 destinationAngle_;

	// 現在の時間(frame単位)
	float currentFrame_;

	// 後追いのレート
	float followRate_;

	// 目標のfov
	float goalFov_;
};