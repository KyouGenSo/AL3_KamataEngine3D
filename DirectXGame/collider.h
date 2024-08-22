#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class Collider {
private:
	// コリジョンの半径
	float radius_ = 1.5f;

	WorldTransform worldTransform_;

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	 virtual ~Collider() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	 void Initialize();

	/// <summary>
	/// ワールトトランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Model* model, const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突判定
	/// </summary>
	virtual void OnCollision() {};

	/// <summary>
	/// Getters
	/// </summary>
	float GetRadius() const { return radius_; }
	virtual Vector3 GetCenter() const = 0;

	/// <summary>
	/// Setters
	/// </summary>
	void SetRadius(float radius) { radius_ = radius; }
};