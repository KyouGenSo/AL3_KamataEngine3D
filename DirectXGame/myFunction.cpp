#include"myFunction.h"

float clamp(float value, float min, float max) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

float Distance(const Vector3& a, const Vector3& b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

float Distance(const Vector2& a, const Vector2& b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Lerp
float Lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) { return Vector3(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t)); }

Vector2 Lerp(const Vector2& a, const Vector2& b, float t) { return Vector2(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t)); }

// EaseIn
Vector2 EaseIn(Vector2 start, Vector2 end, float t) { return start + (end - start) * t * t; }