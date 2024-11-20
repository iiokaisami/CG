#include "Object3d.h"

#include <fstream>
#include <sstream>

#include "Object3dCommon.h"
#include "Model.h"

void Object3d::Initialize(Object3dCommon* object3dCommon)
{
	object3dCommon_ = object3dCommon;


	// 座標変換行列リソースを作る
	CreateTransformationMatrixData();

	// 平行光源リソースを作る
	CreateDirectionalLight();


	// Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}

void Object3d::Update()
{
	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 cameraMatrix = MyMath::MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = MyMath::Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;
}

void Object3d::Draw(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU)
{
	
	// TransformationMatrixCBufferの場所を設定
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	// 平行光源CBufferの場所を設定
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	if (model_)
	{
		model_->Draw();
	}
}

void Object3d::CreateTransformationMatrixData()
{
	// 座標変換行列リソースを作る
	transformationMatrixResource_ = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレス
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->World = MyMath::MakeIdentity4x4();
	transformationMatrixData_->WVP = MyMath::MakeIdentity4x4();
}

void Object3d::CreateDirectionalLight()
{
	// 平行光源リソースを作る
	directionalLightResource_ = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	//書き込むためのアドレス
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//デフォルト値は以下のようにしておく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = MyMath::Normalize({ 0.0f,-1.0f,0.0f });
	directionalLightData_->intensity = 1.0f;
}