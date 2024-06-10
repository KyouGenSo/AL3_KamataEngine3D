#pragma once
#include <list>
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include <cassert>
#include "ImGuiManager.h"
#include "myFunction.h"
#include <algorithm>
#include "PlayerBullet.h"
#include "Vector3.h"
#include "Vector3Function.h"
#include "WinApp.h"

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
	void Update(ViewProjection& viewProjection);
	// 描画
	void Draw3D(ViewProjection& viewProjection);
	void DrawUI();
	// 旋回
	void Rotate();
	// 攻撃
	void Attack();
	// 衝突判定
	void OnCollision();

	// 3Dレティクルの座標計算
	void Update3DReticle(ViewProjection& viewProjection);

	// 親子関係を設定
	void SetParent(const WorldTransform* parent);

	const std::list<playerBullet*>& GetBullets() const { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetWorldPosition3DReticle();

	Vector3 GetWorldRotation();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	// 半径を取得
	float GetRadius() const { return radius_; }

	float GetBulletRadius() const { return bullets_.front()->GetRadius(); }

private: // メンバ変数

	// モデル
	Model* model_ = nullptr;
	Model* model3DReticle_ = nullptr;
	// ワールド変換
	WorldTransform worldTransform_;
	WorldTransform worldTransform3DReticle_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dレティカルのスプライトハンドル
	Sprite* sprite2DReticle_ = nullptr;
	//キーボード入力
	Input* input_ = nullptr;

	// 半径
	float radius_ = 1.0f;

	// プレイヤーの弾
	std::list<playerBullet*> bullets_;

};