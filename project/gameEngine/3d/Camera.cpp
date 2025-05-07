#include "Camera.h"
#include "WinApp.h"

Camera::Camera()
	: transform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} })
	, fovY_(0.45f)
	, aspectRatio_(float(WinApp::kClientWidth) / float(WinApp::kClientHeight))
	, nearClip_(0.1f)
	, farClip_(100.0f)
	, worldMatrix_(MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate))
	, viewMatrix_(Inverse(worldMatrix_))
	, projectionMatrix_(MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_))
	, viewProjectionMatrix_(viewMatrix_* projectionMatrix_)
{}

void Camera::Update()
{
	worldMatrix_= MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;
}

void Camera::StartShake(float duration, float magnitude)
{
	isShaking_ = true;
	shakeDuration_ = duration;
	shakeMagnitude_ = magnitude;
	shakeTimeElapsed_ = 0.0f;
	randomDistribution_ = std::uniform_real_distribution<float>(-1.0f, 1.0f);
}

void Camera::UpdateShake(float deltaTime)
{
	if (!isShaking_) return;

	shakeTimeElapsed_ += deltaTime;
	if (shakeTimeElapsed_ >= shakeDuration_)
	{
		isShaking_ = false;
		return;
	}

	float offsetX = randomDistribution_(randomEngine_) * shakeMagnitude_;
	float offsetY = randomDistribution_(randomEngine_) * shakeMagnitude_;
	float offsetZ = randomDistribution_(randomEngine_) * shakeMagnitude_;

	// カメラの位置をシェイク
	Vector3 originalPosition = GetPosition();
	SetPosition(originalPosition + Vector3(offsetX, offsetY, offsetZ));
}
