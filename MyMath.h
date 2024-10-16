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
	Matrix4x4 MakeRotateZMatrix(float radian)
	{
		Matrix4x4 m = {
			std::cos(radian),std::sin(radian),0,0,
			-std::sin(radian),std::cos(radian),0,0,
			0,0,1,0,
			0,0,0,1
		};
		return m;
	}

	//4x4行列の和
	Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result.m[row][col] = m1.m[row][col] + m2.m[row][col];
			}
		}

		return  result;
	}

	//4x4行列の積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 result;
		result =
		{
			(m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]), (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]), (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]), (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]),
			(m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]), (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]), (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]), (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]),
			(m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]), (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]), (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]), (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]),
			(m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]), (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]), (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]), (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3])
		};

		return result;
	}

	//アフィン変換
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
	{
		//平行移動の4x4行列の生成
		Matrix4x4 scaleMatrix = {
			scale.x,0,0,0,
			0,scale.y,0,0,
			0,0,scale.z,0,
			0,0,0,1
		};


		//回転で使う4x4行列の宣言
		Matrix4x4 rotateMatrix;
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);		//X軸の回転
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);		//Y軸の回転
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);		//Z軸の回転


		//回転の合成
		rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));


		//平行移動の4x4行列の生成
		Matrix4x4 translateMatrix = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			translate.x,translate.y,translate.z,1
		};


		//3次元のアフィン変換
		Matrix4x4 weight = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

		//アフィン変換した値を返す
		return weight;

	}

	Matrix4x4 Inverse(const Matrix4x4& m);
	

	float cot(float a) { return cos(a) / sin(a); }

	//正規化
	Vector3 Normalize(const Vector3& v)
	{
		Vector3 result;
		result.x = v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		result.y = v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		result.z = v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		return result;
	}

	//透視投影行列
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
	{
		Matrix4x4 m{
		1 / aspectRatio * cot(fovY / 2), 0, 0, 0,
			0, cot(fovY / 2), 0, 0,
			0, 0, farClip / (farClip - nearClip), 1,
			0,0,(-nearClip * farClip) / (farClip - nearClip),0
		};
		return m;
	}

	//正射影行列
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
	{
		Matrix4x4 m{
			2 / (right - left),0,0,0,
			0,2 / (top - bottom),0,0,
			0,0,1 / (farClip - nearClip),0,
			(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1
		};

		return m;
	}

	//ビューポート変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
	{
		Matrix4x4 m{
			width / 2,0,0,0,
			0,-height / 2,0,0,
			0,0,maxDepth - minDepth,0,
			left + (width / 2),top + (height / 2),minDepth,1
		};
		return m;
	}


	Matrix4x4 MakeScaleMatrix(Vector3 scale)
	{
		Matrix4x4 result = {
			scale.x,0.0f,0.0f,0.0f,
			0.0f,scale.y,0.0f,0.0f,
			0.0f,0.0f,scale.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};
		return result;
	}

	Matrix4x4 MakeTranslateMatrix(Vector3 translate)
	{
		Matrix4x4 result = {
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			translate.x,translate.y,translate.z,1.0f
		};
		return result;
	}

}