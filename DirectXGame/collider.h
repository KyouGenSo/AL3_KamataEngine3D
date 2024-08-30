#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Collider {
private:
	// コリジョンの半径
	float radius_ = 1.5f;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 種別ID
	uint32_t typeID_ = 0u;

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
	virtual void OnCollision([[maybe_unused]] Collider* other) {}

	/// <summary>
	/// Getters
	/// </summary>
	float GetRadius() const { return radius_; }
	virtual Vector3 GetCenter() const = 0;
	uint32_t GetTypeID() const { return typeID_; }

	/// <summary>
	/// Setters
	/// </summary>
	void SetRadius(float radius) { radius_ = radius; }
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }
};