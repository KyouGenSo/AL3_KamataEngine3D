#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	Vector2 operator+=(const Vector2& v) {
		Vector2 result;
		x += v.x;
		y += v.y;

		return result;
	}

	Vector2 operator-=(const Vector2& v) {
		Vector2 result;
		x -= v.x;
		y -= v.y;

		return result;
	}

	Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }

	Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }

	Vector2 operator*=(float scaler) {
		Vector2 result;
		x *= scaler;
		y *= scaler;

		return result;
	}

	Vector2 operator/=(float scaler) {
		Vector2 result;
		x /= scaler;
		y /= scaler;

		return result;
	}

	Vector2 operator*(float scaler) const { return Vector2(x * scaler, y * scaler); }

	Vector2 operator/(float scaler) const { return Vector2(x / scaler, y / scaler); }
};
