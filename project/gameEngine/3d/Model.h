#pragma once

#include "MyMath.h"
#include "TextureManager.h"

class ModelCommon;

class Model
{
public:

	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);

	void UpData();

	void Draw();

public: // セッター

	void SetEnableLighting(bool enable);
	void SetEnableDirectionalLight(bool enable);
	void SetEnablePointLight(bool enable);
	void SetEnableSpotLight(bool enable);

private: // 構造体、関数

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
		float shininess;
		int32_t phongReflection;
		int32_t halfphongReflection;
		int32_t pointLight;
		int32_t spotLight;
	};

	//mtlファイルを読む関数
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	//Objファイルを読む関数
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	// 頂点データ生成
	void CreateVertexData();

	// マテリアルデータ生成
	void CreateMaterialData();


public: // ゲッター

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView()const { return vertexBufferView_; }
	ModelData GetModelData() { return modelData_; }

private:
	
	ModelCommon* modelCommon_ = nullptr;

	// Objファイルのデータ
	ModelData modelData_;

	// バッファリソース
	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};
	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// Transform
	Transform transform_;
};

