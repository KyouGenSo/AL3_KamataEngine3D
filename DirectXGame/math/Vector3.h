#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(const Vector3& v) {
		Vector3 result;
		x += v.x;
		y += v.y;
		z += v.z;

		return result;
	}

	Vector3 operator+=(const float scaler) {
		Vector3 result;
		x += scaler;
		y += scaler;
		z += scaler;

		return result;
	}

	Vector3 operator-=(const Vector3& v) {
		Vector3 result;
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return result;
	}

	Vector3 operator-=(const float scaler) {
		Vector3 result;
		x -= scaler;
		y -= scaler;
		z -= scaler;

		return result;
	}

	Vector3 operator*=(const Vector3& v) {
		Vector3 result;
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return result;
	}

	Vector3 operator*=(float scaler) {
		Vector3 result;
		x *= scaler;
		y *= scaler;
		z *= scaler;

		return result;
	}

	Vector3 operator/=(const Vector3& v) {
		Vector3 result;
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return result;
	}

	Vector3 operator/=(float scaler) {
		Vector3 result;
		x /= scaler;
		y /= scaler;
		z /= scaler;

		return result;
	}
};