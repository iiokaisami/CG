#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

struct Vector2
{
	float x;
	float y;
};

struct Vector3
{
	float x;
	float y;
	float z;
};

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

struct Matrix4x4
{
	float m[4][4];
};

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Transform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material
{
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct DirectionalLight
{
	Vector4 color;
	Vector3 direction;
	float intensity;
};


struct MaterialData
{
	std::string textureFilePath;
};

struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};

namespace MyMath {
	Matrix4x4 MakeIdentity4x4();
	
	//x軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);
	
	//y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);

	//z軸回転行列
	Matrix4x4 MakeRotateZMatrix(float radian);
	
	//4x4行列の和
	Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	//4x4行列の積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	//アフィン変換
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	
	Matrix4x4 Inverse(const Matrix4x4& m);
	
	inline float cot(float a) { return cos(a) / sin(a); }

	//正規化
	Vector3 Normalize(const Vector3& v);
	
	//透視投影行列
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	//正射影行列
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	//ビューポート変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	
	Matrix4x4 MakeScaleMatrix(Vector3 scale);
	
	Matrix4x4 MakeTranslateMatrix(Vector3 translate);

}