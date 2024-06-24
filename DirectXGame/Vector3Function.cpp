#include "Vector3Function.h"

Vector3 Add(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

Vector3 Subtract(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }

Vector3 Multiply(const Vector3& v, float scaler) { return Vector3(v.x * scaler, v.y * scaler, v.z * scaler); }

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

double Length(const Vector3& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector3 Normalize(const Vector3& v) {
	double length = Length(v);
	return Vector3(v.x / (float)length, v.y / (float)length, v.z / (float)length);
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	float dot = Dot(v1, v2);
	dot = dot > 1.0f ? 1.0f : dot;
	dot = dot < -1.0f ? -1.0f : dot;
	float theta = (float)acos(dot) * t;
	Vector3 relativeVec = Subtract(v2, Multiply(v1, dot));
	relativeVec = Normalize(relativeVec);
	return Add(Multiply(v1, (float)cos(theta)), Multiply(relativeVec, (float)sin(theta)));
}