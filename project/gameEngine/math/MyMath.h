#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

#include "Matrix4x4.h"
#include "Vector3.h"

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Matrix3x3
{
	float m[3][3];
};



// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ
float Length(const Vector3& v);
// 正規化 v/||v||
Vector3 Normalize(const Vector3& v);
// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);
// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// -----行列-----
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
// 任意軸回転行列
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);
// ベクトルからベクトルへの変換行列
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);
// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 逆転置行列
Matrix4x4 InverseTranspose(const Matrix4x4& m);
// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// vec * mat
Vector3 Multiply(const Vector3& vec, const Matrix4x4& mat);

// -----座標系-----
// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// -----補間-----
// 線形補間
float Lerp(const float& v1, const float& v2, float t);
// ベクトルの線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
float LerpAngle(float a, float b, float t);

namespace MyMath 
{
	inline float cot(float a) { return cos(a) / sin(a); }
}