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

float Lerp(const float a, const float b, const float t) { return a + (b - a) * t; }

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	float dot = Dot(v1, v2);

	dot = dot > 1.0f ? 1.0f : dot;
	dot = dot < -1.0f ? -1.0f : dot;

	float theta = (float)acos(dot) * t;

	float sinTheta = (float)sin(theta);

	float sinThetaFrom = (float)sin((1.0f - t) * theta);
	float sinThetaTo = (float)sin(t * theta);

	float length1 = (float)Length(v1);
	float length2 = (float)Length(v2);

	float length = Lerp(length1, length2, t);

	if (sinTheta < 1.0e-5) {

		return v1;

	} else {

		return Multiply(Add(Multiply(v1, sinThetaFrom / sinTheta), Multiply(v2, sinThetaTo / sinTheta)), length);
	}
}

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	return ((p0 * -1.0f + p1 * 3.0f - p2 * 3.0f + p3) * t * t * t + (p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t * t + (p0 * -1.0f + p2) * t + p1 * 2.0f) * 0.5f;
}