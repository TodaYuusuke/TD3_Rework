#pragma once

#include "PlayerConfig.h"

/// <summary>
/// プレイヤーの基本的な設定を持っている
/// 設定から参照し、計算した結果を格納
/// 設計的に、初期設定と同じ項目が多い
/// 初期値が決まっているものは設定から読み込みたくない
/// </summary>
class PlayerParameter final {
public: //*** メンバ関数 ***//

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// スキルのアップグレードを更新
	/// </summary>
	void Update();

	/// <summary>
	/// レベルがあがっとことを知らせるシグナル
	/// </summary>
	bool GetLevelUpSignal();

	/// <summary>
	/// Tree 形式で内部の情報を表示する
	/// <para>内部でも #ifdef DEMO をしている</para>
	/// </summary>
	void DebugTree();

	//レベルアップのフラグ
	bool IsUpGradeFlag = false;

public: //*** メンバ変数 ***//

	// 基本的な設定
	PlayerConfig config;
	
	Upgrade status;

	// まとめられたフラグ
	Flags flags_;

private: //*** 関数群 ***//

	/// <summary>
	/// レベルアップした時に呼び出す
	/// </summary>
	void LevelUp();

	// 移動値を初期化
	void InitMoveSpeed();
	// 時間を初期化
	void InitProgressTime();
	// 距離や範囲を初期化
	void InitLengthRadius();
	// 補正や割合を初期化
	void InitCollectionRatio();
	// 回数やゲージを初期化
	void InitCountGage();
	//レベルとスキルの初期化
	void InitLevelANDSkill();

	// 移動値のデバッグ表示
	void DebugTreeMoveSpeed();
	// 時間のデバッグ表示
	void DebugTreeProgressTime();
	// 距離や範囲のデバッグ表示
	void DebugTreeLengthRadius();
	// 補正や割合のデバッグ表示
	void DebugTreeCollectionRatio();
	// 回数やゲージのデバッグ表示
	void DebugTreeCountGage();


};