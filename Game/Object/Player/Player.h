#pragma once
#include <Adapter.h>
#include "PlayerParameter.h"
#include "Weapon.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Object::Camera* pCamera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetWorldPosition() { return model_.worldTF.GetWorldPosition(); }

	PlayerParameter& GetPlayerParameter() {
		return parameter_;
	}
	/// <summary>
	/// ワールドトランスフォーム
	/// </summary>
	/// <returns></returns>
	const LWP::Object::TransformQuat* GetWorldTransform() { return &model_.worldTF; }

	void SetCamera(lwp::Camera* pCamera) {
		pCamera_ = pCamera;
	}

private: //*** サブクラス ***//

	// 状態
	enum class Behavior : int {
		Idle = 0,
		Move,
		Attack,
		Moment,
		Damage,

		_Count,	// 状態最大数
	};

	// 処理中の時間をまとめた構造体
	struct Times {
		float behaviorTime;		// 各行動中の経過時間
		float invincibleTime;	// 無敵時間
	};

	// コライダーと形状をまとめた構造体
	// 形状はカプセル
	struct CapsuleCollider {
		LWP::Object::Collider::Collider collider;	// 当たり判定
		LWP::Object::Collider::Capsule& capsule;	// 形状

		/// <summary>
		/// コンストラクタ
		/// <para>参照変数のために用意</para>
		/// </summary>
		CapsuleCollider() : capsule(collider.SetBroadShape(lwp::Collider::Capsule())) {
		}
	};

private: //*** メンバ変数 ***//

	// モデル
	LWP::Resource::RigidModel model_;
	// カメラ
	LWP::Object::Camera* pCamera_;

	// フォローカメラ
	FollowCamera followCamera_;

	// 今の状態を格納
	Behavior behavior_ = Behavior::Idle;
	// 状態が変わった時に値が入る
	std::optional<Behavior> reqBehavior_ = std::nullopt;

	// 初期化関数ポインタの配列
	void (Player::* stateInit_[(int)Behavior::_Count])();
	// 更新関数ポインタの配列
	void (Player::* stateUpdate_[(int)Behavior::_Count])();

	// 各種パラメータを格納
	PlayerParameter parameter_;

	// 向いている方向
	// 操作があった時に向く方向を決定している
	LWP::Math::Vector3 destinate_ = { 0.0f,0.0f,1.0f };

#pragma region
	lwp::Vector3 lookPoint;
#pragma endregion キャラの回転

	// プレイヤーの速度を格納
	// カメラ方向も計算している
	LWP::Math::Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	// まとめられた時間
	Times times_;

	// 武器
	std::unique_ptr<Weapon> weapon_;

	// 当たり判定は外部でまとめたい
	// プレイヤーの当たり判定
	CapsuleCollider playerCollider_;
	// 攻撃の当たり判定
	CapsuleCollider attackCollider_;

private: //*** メンバ関数 ***//

	// 体力を減らす
	void DecreaseHP();

	// 移動方向、向いている方向の入力を受け取る
	void CheckInputMove();

	// 攻撃の入力を受け取る
	void CheckInputAttack();

	// 関数ポインタ配列を初期化
	void InitStateFunctions();

	// 当たり判定を初期化
	void InitColliders();


private: //*** 各状態の関数群 ***//

	// 当たり判定の初期化

	// プレイヤー自体の当たり判定
	void InitColliderPlayer();
	// 攻撃の当たり判定
	void InitColliderAttack();

	// プレイヤーの当たり判定関数
	void EnterPlayer(LWP::Object::Collider::Collider* hitTarget);
	void StayPlayer(LWP::Object::Collider::Collider* hitTarget);
	void ExitPlayer(LWP::Object::Collider::Collider* hitTarget);

	// 攻撃の当たり判定関数
	void EnterAttack(LWP::Object::Collider::Collider* hitTarget);
	void StayAttack(LWP::Object::Collider::Collider* hitTarget);
	void ExitAttack(LWP::Object::Collider::Collider* hitTarget);


	// 状態の初期化

	// 待機状態
	void InitIdle();
	// 移動状態
	void InitMove();
	// 攻撃状態
	void InitAttack();
	// 攻撃後の後隙状態
	void InitMoment();
	// ダメージを喰らった状態
	void InitDamage();


	// 状態の更新

	// 待機状態
	void UpdateIdle();
	// 移動状態
	void UpdateMove();
	// 攻撃状態
	void UpdateAttack();
	// 攻撃後の後隙状態
	void UpdateMoment();
	// ダメージを喰らった状態
	void UpdateDamage();

public: //*** デバッグ用の関数 ***//

	/// <summary>
	/// デバッグウィンドウを表示する
	/// <para>内部で #ifdef DEMO している</para>
	/// </summary>
	void DebugWindow();
};

lwp::Vector3 TransformNormal(const lwp::Vector3& v, const lwp::Matrix4x4& m);