#pragma once
#include "ImGuiManager.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "Vector3.h"
#include "Vector3Function.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "WorldTransform.h"
#include "myFunction.h"
#include <algorithm>
#include <cassert>
#include <list>

class Enemy;

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
	void Update(ViewProjection& viewProjection, std::list<Enemy*> enemies);

	// 描画
	void Draw3D(ViewProjection& viewProjection);
	void DrawUI();

	// 旋回
	void Rotate();

	// マウスで視点移動
	void MouseMove();

	// 攻撃
	void SingleAttack(std::list<Enemy*> enemies);
	void MultiAttack(std::list<Enemy*> enemies);

	// 衝突判定
	void OnCollision();

	// 3Dレティクルの座標計算
	void Update3DReticle(ViewProjection& viewProjection, std::list<Enemy*> enemies);

	// レティクルのシングルロックオン
	void ReticleSingleLockOn(ViewProjection& viewProjection, std::list<Enemy*> enemies);

	// レティクルのマルチロックオン
	void ReticleMultiLockOn(ViewProjection& viewProjection, std::list<Enemy*> enemies);

	// 親子関係を設定
	void SetParent(const WorldTransform* parent);

	void SetLockOn(bool isLockOn) { isLockOn_ = isLockOn; }

	void Set2DReticlePosition(Vector2 pos) { sprite2DReticle_->SetPosition(pos); }

	void Set2DReticleLockOnPosition(Vector2 pos) { sprite2DReticleLockOn_->SetPosition(pos); }

	bool GetLockOn() { return isLockOn_; }

	const std::list<playerBullet*>& GetBullets() const { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetWorldPosition3DReticle();

	Vector3 GetWorldRotation();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	// 半径を取得
	float GetRadius() const { return radius_; }

	float GetBulletRadius() const { return bullets_.front()->GetRadius(); }

	Vector2 Get2DReticlePosition() { return sprite2DReticle_->GetPosition(); }

private: // メンバ変数
	// モデル
	Model* model_ = nullptr;
	Model* model3DReticle_ = nullptr;
	// ワールド変換
	WorldTransform worldTransform_;
	WorldTransform worldTransform3DReticle_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// レティカルのスプライトハンドル
	Sprite* sprite2DReticle_ = nullptr;
	Sprite* sprite2DReticleLockOn_ = nullptr;

	//std::list<Sprite*> sprite2DReticleLockOns_;

	// キーボード入力
	Input* input_ = nullptr;

	// 半径
	float radius_ = 1.0f;

	bool isLockOn_ = false;
	int enemyLockOnCount_ = 0;

	bool isEased_ = false;

	float easingT_ = 0.0f;

	// プレイヤーの弾
	std::list<playerBullet*> bullets_;
};