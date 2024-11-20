#pragma once

#include "MyMath.h"
#include "TextureManager.h"

class Object3dCommon;



class Object3d
{
public:

	// 初期化
	void Initialize(Object3dCommon* object3dCommon);

	// 更新処理
	void Update();

	// 描画処理
	void Draw(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);

private:

	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct MaterialData
	{
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	struct ModelData
	{
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight
	{
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	//mtlファイルを読む関数
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	//Objファイルを読む関数
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	// 頂点データ生成
	void CreateVertexData();

	// マテリアルデータ生成
	void CreateMaterialData();

	// 座標変換行列データ生成
	void CreateTransformationMatrixData();

	// 平行光源生成
	void CreateDirectionalLight();

private:

	Object3dCommon* object3dCommon_ = nullptr;

	// Objファイルのデータ
	ModelData modelData_;

	// バッファリソース
	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};
	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};
	//TransformationMatrix用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_{};
	// 平行光源リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_{};

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Transform transform_{};
	Transform cameraTransform_{};
};

