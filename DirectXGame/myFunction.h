#pragma once
#include <cmath>
#include "math.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include <numbers>
#include <iostream>
#include <stdlib.h>
#include <time.h> 


float clamp(float value, float min, float max);

float Distance(const Vector3& a, const Vector3& b);

float Distance(const Vector2& a, const Vector2& b);

float Rand(float min, float max);

Vector3 RandomVector3(float min, float max);

// Lerp
float Lerp(float a, float b, float t);

Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

float LerpShortAngle(float a, float b, float t);

// EaseIn
Vector2 EaseIn(Vector2 start, Vector2 end, float t);

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
