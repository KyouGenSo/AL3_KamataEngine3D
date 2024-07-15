#pragma once
#include <cmath>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"


float clamp(float value, float min, float max);

float Distance(const Vector3& a, const Vector3& b);

float Distance(const Vector2& a, const Vector2& b);
