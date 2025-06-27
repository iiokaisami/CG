#pragma once

#include "MyMath.h"

#include <random>

class Camera
{
public:

	Camera();

	// 更新
	void Update();

	// シェイクを開始
	void StartShake(float duration, float magnitude);

	// シェイクを更新
	void UpdateShake(float deltaTime);

public: // セッター

	void SetRotate(Vector3 rotate) { transform_.rotate = rotate; }
	void SetPosition(Vector3 translate) { transform_.translate = translate; }
	void SetFovY(float fovY) { fovY_ = fovY; }
	void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(float farClip) { farClip_ = farClip; }

public: // ゲッター

	const Vector3& GetRotate() const { return transform_.rotate; }
	const Vector3& GetPosition() const { return transform_.translate; }
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	bool IsShaking() const { return isShaking_; }

private:

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

private:

	Transform transform_{};
	Matrix4x4 worldMatrix_{};
	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatrix_{};
	float fovY_;			// 水平方向視野角
	float aspectRatio_;     // アスペクト比
	float nearClip_;		// ニアクリップ距離
	float farClip_;		    // ファークリップ距離

	Matrix4x4 viewProjectionMatrix_{};

	// シェイクのための変数
	bool isShaking_ = false;
	float shakeDuration_ = 0.0f;
	float shakeMagnitude_ = 0.0f;
	float shakeTimeElapsed_ = 0.0f;
	std::default_random_engine randomEngine_;
	std::uniform_real_distribution<float> randomDistribution_;
};