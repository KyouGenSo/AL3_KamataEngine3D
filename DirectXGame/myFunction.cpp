#include "myFunction.h"

const float pi = (float)std::numbers::pi;

float clamp(float value, float min, float max) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

float Distance(const Vector3& a, const Vector3& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)); }

float Distance(const Vector2& a, const Vector2& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }

float Rand(float min, float max) { 
	return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

Vector3 RandomVector3(float min, float max) { return Vector3(Rand(min, max), Rand(min, max), Rand(min, max)); }

// Lerp
float Lerp(float a, float b, float t) { return a + (b - a) * t; }

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) { return Vector3(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t)); }

Vector2 Lerp(const Vector2& a, const Vector2& b, float t) { return Vector2(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t)); }

float LerpShortAngle(float thetaA, float thetaB, float t) {
	float diff = thetaB - thetaA;

	// 2πから-2πに補正
	if (diff > pi * 2) {
		diff = std::fmod(diff, pi * 2);
	} else if (diff < -pi * 2) {
		diff = std::fmod(diff, pi * 2);
	}

	// 180度以上の差がある場合は、逆回転する方向に補間する
	if (diff > pi) {
		diff -= pi * 2;
	} else if (diff < -pi) {
		diff += pi * 2;
	}

	return thetaA + diff * t;
}

// EaseIn
Vector2 EaseIn(Vector2 start, Vector2 end, float t) { return start + (end - start) * t * t; }
