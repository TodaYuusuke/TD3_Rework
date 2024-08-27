#include "FollowCamera.h"
#include "../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace LWP::Input::Pad;

FollowCamera::FollowCamera() {
	currentFrame_ = 0;
	followRate_ = kFollowRate;
}

void FollowCamera::Init(lwp::Camera* pCamera) {
	pCamera_ = pCamera;
	goalFov_ = pCamera_->fov;
}

void FollowCamera::Update() {
	// 入力処理
	 // 回転する向き
	Vector2 dir = { 0.0f,0.0f };

	// キーボードでの回転
	if (Keyboard::GetPress(DIK_UP)) {
		dir.x += 1.0f;
	}
	if (Keyboard::GetPress(DIK_DOWN)) {
		dir.x -= 1.0f;
	}
	if (Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += 1.0f;
	}
	if (Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= 1.0f;
	}

	// コントローラーでの回転
	dir.x -= Pad::GetRStick().y;
	dir.y += Pad::GetRStick().x;

	// 任意軸を生成
	if (std::abs(dir.x) >= 0.4f) {
		pCamera_->transform.rotation = pCamera_->transform.rotation * Quaternion::CreateFromAxisAngle(Vector3{ 1, 0, 0 }, 0.03f * dir.x);
	}
	// Y軸は常に上を向くように固定
	//if()
	if (std::abs(dir.y) >= 0.4f) {
		pCamera_->transform.rotation = Quaternion::CreateFromAxisAngle(Vector3{ 0, 1, 0 }, 0.03f * dir.y) * pCamera_->transform.rotation;
	}

	//pCamera_->transform.rotation.x = std::clamp<float>(pCamera_->transform.rotation.x, 0.03f, 0.16f);

	// カメラの座標を決定
	pCamera_->transform.translation = target_->GetWorldPosition() + kTargetDist * Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);

	// fovの設定
	pCamera_->fov = ExponentialInterpolate(pCamera_->fov, goalFov_, 0.2f, 1.0f);

	ImGui::Begin("Camera");
	ImGui::DragFloat3("rotation", &pCamera_->transform.rotation.x, 0);
	ImGui::End();
}

void FollowCamera::ResetAngle() {
	destinationAngle_ = { kStartAngle.x, target_->rotation.y };
	// オフセットの計算
	LWP::Math::Vector3 offset = CalcOffset();
	// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
	pCamera_->transform.translation = interTarget_ + offset;
}

void FollowCamera::InputAngle() {
	// 入力感度
	const LWP::Math::Vector2 sensitivity = kSensitivity;
#pragma  region キーボード入力
	// Y軸
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		destinationAngle_.y += sensitivity.x;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		destinationAngle_.y -= sensitivity.x;
	}
	// X軸
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		destinationAngle_.x -= sensitivity.y;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		destinationAngle_.x += sensitivity.y;
	}
#pragma endregion

#pragma  region ゲームパッド入力
	// Rスティック
	destinationAngle_.x -= Controller::GetRStick().y * sensitivity.y;
	destinationAngle_.y += Controller::GetRStick().x * sensitivity.x;

	// 上下の回転に制限をかける
	destinationAngle_.x = std::clamp<float>(destinationAngle_.x, kMinAxisX, kMaxAxisX);

	// Rスティック押し込み
	// 角度リセット
	if (Controller::GetTrigger(XINPUT_GAMEPAD_RIGHT_THUMB)) {
		ResetAngle();
	}
#pragma endregion
}

#pragma region 数学関数
float FollowCamera::ExponentialInterpolate(const float& current, const float& target, float damping, float deltaTime) {
	float factor = 1.0f - std::exp(-damping * deltaTime);
	return current + (target - current) * factor;
}

LWP::Math::Vector3 FollowCamera::CalcOffset() const {
	LWP::Math::Vector3 offset = kTargetDist;
	// カメラの角度から回転行列を計算
	LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
	// オフセットをカメラの回転に合わせて回転
	offset = offset * rotateMatrix;

	return offset;
}
#pragma endregion