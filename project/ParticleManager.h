#pragma once

#include <unordered_map>
#include <random> 
#include <TextureManager.h>

#include "MyMath.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "Particle.h"
#include "CameraManager.h"

class Object3dCommon;

// 構造体(Emitterでも使いたい)
struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

struct ParticleForGPU
{
	Matrix4x4 WVP;
	Matrix4x4 world;
	Vector4 color;
};

struct ParticleGroup
{
	MaterialData materialData;
	std::list<Particle> particleList;
	uint32_t srvIndex;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
	uint32_t kNumMaxInstance;
	ParticleForGPU* instancingData;
};

class ParticleManager
{
public:

	static ParticleManager* GetInstance()
	{
		static ParticleManager instance;  // インスタンスを1回だけ作成
		return &instance;
	}


	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	// パイプライン生成
	void CreatePipeline();

	// ルートシグネイチャ生成
	void CreateRootSignature();

	// パーティクルグループの生成
	void CreateParticleGroup(const std::string& name, const std::string& textureFilePath);

	// 更新
	void Update();

	// 描画
	void Draw();

	void Emit(const std::string name, const Vector3& position, uint32_t count);

public: // セッター

	void SetCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }

private: // 構造体

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

	struct AABB
	{
		Vector3 min;//!<最小点
		Vector3 max;//!< 最大点
	};

	struct AccelerationField 
	{
		Vector3 acceleration; //!< 加速度
		AABB area;            //!< 範囲
	};

private:

	ParticleManager() = default;  // コンストラクタはプライベート

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	Object3dCommon* object3dCommon_ = nullptr;

	std::shared_ptr<Camera> camera_ = nullptr;

	// 頂点バッファ関連
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	// パイプライン関連
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

	// 乱数生成
	std::mt19937 randomEngine_;

	//ディスクリプタレンジの生成
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1]{};
	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
	//InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_{};
	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	//shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	D3D12_ROOT_PARAMETER rootParameters_[4] = {};
	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1] = {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};

	std::unordered_map<std::string, ParticleGroup> particleGroups;


	AccelerationField accelerationField_;

	const float kDeltaTime_ = 1.0f / 60.0f;
};

