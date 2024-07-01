#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }

	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }

	Vector3 operator*(float scaler) const { return Vector3(x * scaler, y * scaler, z * scaler); }

	Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }

	Vector3 operator/(float scaler) const { return Vector3(x / scaler, y / scaler, z / scaler); }

	Vector3 operator/(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

	Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};