#include "Object3d.h"

#include <fstream>
#include <sstream>

#include "Object3dCommon.h"
#include "Model.h"
#include "ModelManager.h"

void Object3d::Initialize(const std::string& filePath)
{
	object3dCommon_ = Object3dCommon::GetInstance();
	// モデルを設定
	SetModel(filePath);

	// 座標変換行列リソースを作る
	CreateTransformationMatrixData();

	// 平行光源リソースを作る
	CreateDirectionalLight();

	// Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };

	camera_ = object3dCommon_->GetDefaultCamera();

	// カメラリソースを作る
	CreateCamera();
}

void Object3d::Update()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	
	if (camera_)
	{
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = (worldMatrix * viewProjectionMatrix);
	}
	else
	{
		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;

#ifdef _DEBUG


	ImGui::Begin("Object3d");

	ImGui::SliderFloat3("position", &cameraData_->worldPosition.x, -50.0f, 50.0f);

	ImGui::End();

#endif // _DEBUG


}

void Object3d::Draw()
{
	// TransformationMatrixCBufferの場所を設定
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	// 平行光源CBufferの場所を設定
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	// カメラCBufferの場所を設定
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource_->GetGPUVirtualAddress());

	if (model_)
	{
		model_->Draw();
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::CreateTransformationMatrixData()
{
	// 座標変換行列リソースを作る
	transformationMatrixResource_ = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレス
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->World = MakeIdentity4x4();
	transformationMatrixData_->WVP = MakeIdentity4x4();
}

void Object3d::CreateDirectionalLight()
{
	// 平行光源リソースを作る
	directionalLightResource_ = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	//書き込むためのアドレス
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//デフォルト値は以下のようにしておく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = Normalize({ 0.0f,-1.0f,0.0f });
	directionalLightData_->intensity = 1.0f;
}

void Object3d::CreateCamera()
{
	// カメラリソースを作る
	cameraResource_ = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(CameraForGPU));
	//書き込むためのアドレス
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	//デフォルト値は以下のようにしておく
	cameraData_->worldPosition = camera_->GetPosition(); // ここカメラセットしてない
}
