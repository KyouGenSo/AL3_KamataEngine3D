#pragma once
#include <list>
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include <cassert>
#include "ImGuiManager.h"
#include "myFunction.h"
#include <algorithm>
#include "PlayerBullet.h"
#include "Vector3.h"

// 自キャラクラス
class player {

public: // メンバ関数

	// コンストラクタ
	player();
	// デストラクタ
	~player();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ViewProjection& viewProjection);
	// 旋回
	void Rotate();
	// 攻撃
	void Attack();

	void OnCollision();

	// 親子関係を設定
	void SetParent(const WorldTransform* parent);

	const std::list<playerBullet*>& GetBullets() const { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetWorldRotation();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	// 半径を取得
	float GetRadius() const { return radius_; }

	float GetBulletRadius() const { return bullets_.front()->GetRadius(); }

private: // メンバ変数

	// モデル
	Model* model_ = nullptr;
	// ワールド変換
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//キーボード入力
	Input* input_ = nullptr;

	// 半径
	float radius_ = 1.0f;

	// プレイヤーの弾
	std::list<playerBullet*> bullets_;

};