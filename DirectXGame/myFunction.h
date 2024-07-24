#pragma once
#include <cmath>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"


float clamp(float value, float min, float max);

float Distance(const Vector3& a, const Vector3& b);

float Distance(const Vector2& a, const Vector2& b);

// Lerp
float Lerp(float a, float b, float t);

Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

// EaseIn
Vector2 EaseIn(Vector2 start, Vector2 end, float t);
