#include "ParticleManager.h"

#include <wrl.h>
#include <stdexcept>
#include <vector>
#include <random>

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	// ランダムエンジンの初期化
    randomEngine_ = std::mt19937{ std::random_device{}() };

    // パイプライン生成
    CreatePipeline();

    // 頂点データの初期化（座標など）
    struct Vertex {
        float position[3];
        float color[4];
    };

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 左下
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 右下
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}  // 上
    };

    // 頂点データのサイズを計算
    const UINT vertexBufferSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));

    //頂点リソース用のヒープ設定
    D3D12_HEAP_PROPERTIES uploadHeapProperties{};
    uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    //頂点リソースの設定
    D3D12_RESOURCE_DESC bufferResourceDesc{};
    //バッファリソース。テクスチャの場合はまた別の設定をする
    bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferResourceDesc.Width = vertexBufferSize;
    //バッファの場合はこれらは１にする決まり
    bufferResourceDesc.Height = 1;
    bufferResourceDesc.DepthOrArraySize = 1;
    bufferResourceDesc.MipLevels = 1;
    bufferResourceDesc.SampleDesc.Count = 1;

    // 頂点リソース生成
    HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &bufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer_)
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create vertex buffer.");
    }

    // 頂点バッファビュー生成
    vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = vertexBufferSize;
    vertexBufferView_.StrideInBytes = sizeof(Vertex);

    // 頂点リソースに頂点データを書き込む
    void* mappedData = nullptr;
    hr = vertexBuffer_->Map(0, nullptr, &mappedData);
    if (SUCCEEDED(hr)) {
        memcpy(mappedData, vertices.data(), vertexBufferSize);
        vertexBuffer_->Unmap(0, nullptr);
    }
    else {
        throw std::runtime_error("Failed to map vertex buffer.");
    }

    accelerationField_.acceleration = { 15.0f,0.0f,0.0f };
    accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
    accelerationField_.area.max = { 1.0f,1.0f,1.0f };
}

void ParticleManager::CreatePipeline()
{
    HRESULT result = S_FALSE;

    CreateRootSignature();


    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;
    graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),vertexShaderBlob_->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),pixelShaderBlob_->GetBufferSize() };
    graphicsPipelineStateDesc.BlendState = blendDesc_;
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;
    //DepthStencilの設定
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc_;
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;




    //書き込むRTVの情報
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    //利用するトポロジ（形状）のタイプ。三角形
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    //どのように画面に色を打ち込むかの設定（気にしなくていい）
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    //実際に生成
    result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipelineState_));
    assert(SUCCEEDED(result));

    // カリングしない（裏面も表示させる）
    rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
}

void ParticleManager::CreateRootSignature()
{
	HRESULT result = S_FALSE;

	//ディスクリプタレンジの生成
	descriptorRange_[0].BaseShaderRegister = 0;
	descriptorRange_[0].NumDescriptors = 1;
	descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameter作成。複数設定できるので配列。今回は結果１つだけなので長さ１の配列
	rootParameters_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//CBVを使う
	rootParameters_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;		//PixelShaderで使う
	rootParameters_[0].Descriptor.ShaderRegister = 0;						//レジスタ番号０とバインド

	rootParameters_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters_[1].Descriptor.ShaderRegister = 0;

	rootParameters_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;			//DescriptorTableを使う
	rootParameters_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;						//PixelShaderで使う
	rootParameters_[2].DescriptorTable.pDescriptorRanges = descriptorRange_; 				//Tableの中身の配列を指定
	rootParameters_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);		//Tableで利用する数

	rootParameters_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[3].Descriptor.RegisterSpace = 0;
	rootParameters_[3].Descriptor.ShaderRegister = 1;

	descriptionRootSignature_.pParameters = rootParameters_;					//ルートパラメータ配列へのポインタ
	descriptionRootSignature_.NumParameters = _countof(rootParameters_);		//配列の長さ

	//Smaplerの設定
	staticSamplers_[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;				//バイリニアフィルタ
	staticSamplers_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//0～1の範囲外をリピート
	staticSamplers_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;			//比較しない
	staticSamplers_[0].MaxLOD = D3D12_FLOAT32_MAX;							//ありったけのMipmapを使う
	staticSamplers_[0].ShaderRegister = 0;									//レジスタ番号0を使う
	staticSamplers_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;		//PixelShaderを使う
	descriptionRootSignature_.pStaticSamplers = staticSamplers_;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers_);



	//シリアライズしてバイナリする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	result = D3D12SerializeRootSignature(&descriptionRootSignature_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result))
	{
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリをもとに生成
	result = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));


	// InputLayoutの設定
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[1].SemanticName = "TEXCOORD";
	inputElementDescs_[1].SemanticIndex = 0;
	inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[2].SemanticName = "NORMAL";
	inputElementDescs_[2].SemanticIndex = 0;
	inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);

	// BlendStateの設定
	// 全ての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasterizerStateの設定
	// 裏面(時計回り)の表示の有無 (NONE / BACK)
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	// 塗りつぶすかどうか
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	// Shaderをコンパイル 
	vertexShaderBlob_ = dxCommon_->CompileShader(L"Resources/shaders/Object3d.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);
	pixelShaderBlob_ = dxCommon_->CompileShader(L"Resources/shaders/Object3d.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);

	// DepthStencilStateの設定
	// Depthの機能を有効化
	depthStencilDesc_.DepthEnable = true;
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void ParticleManager::CreateParticleGroup(const std::string& name, const std::string& textureFilePath) {
    // 登録済みの名前かチェック
    assert(particleGroups.find(name) == particleGroups.end() && "Particle group with the same name already exists.");

    // 新たな空のパーティクルグループを生成
    ParticleGroup newGroup = {};

    // テクスチャを読み込む
    uint32_t srvIndex = srvManager_->LoadTexture(textureFilePath);

    // マテリアルデータにテクスチャファイルパスとSRVインデックスを設定
    newGroup.materialData.textureFilePath = textureFilePath;
    newGroup.srvIndex = srvIndex;

    // インスタンシング用リソースの生成
    const uint32_t kNumMaxInstance = 1024; // 最大インスタンス数
    newGroup.kNumMaxInstance = kNumMaxInstance;

    //頂点リソース用のヒープ設定
    D3D12_HEAP_PROPERTIES uploadHeapProperties{};
    uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    //頂点リソースの設定
    D3D12_RESOURCE_DESC bufferResourceDesc{};
    //バッファリソース。テクスチャの場合はまた別の設定をする
    bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferResourceDesc.Width = sizeof(ParticleForGPU) * kNumMaxInstance;
    //バッファの場合はこれらは１にする決まり
    bufferResourceDesc.Height = 1;
    bufferResourceDesc.DepthOrArraySize = 1;
    bufferResourceDesc.MipLevels = 1;
    bufferResourceDesc.SampleDesc.Count = 1;

    HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &bufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&newGroup.instancingResource)
    );
    assert(SUCCEEDED(hr) && "Failed to create instancing resource.");

    // インスタンシング用データのマッピング
    hr = newGroup.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&newGroup.instancingData));
    assert(SUCCEEDED(hr) && "Failed to map instancing resource.");

    // SRVを生成（StructuredBuffer用設定）
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.NumElements = kNumMaxInstance;
    srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    dxCommon_->GetDevice()->CreateShaderResourceView(
        newGroup.instancingResource.Get(),
        &srvDesc,
        srvManager_->GetCpuHandle(srvIndex)
    );

    // コンテナに登録
    particleGroups[name] = std::move(newGroup);
}

void ParticleManager::Update(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix)
{
    for (auto& group : particleGroups)
    {
        ParticleGroup& particleGroup = group.second;

        // ビュー行列とプロジェクション行列をカメラから取得
        Matrix4x4 viewProjectionMatrix = viewMatrix * projectionMatrix;

        // 各パーティクルの処理
        for (auto it = particleGroup.particleList.begin(); it != particleGroup.particleList.end();)
        {
            Particle& particle = *it;

            // 寿命が終わったパーティクルを削除
            if (particle.currentTime >= particle.lifeTime)
            {
                it = particleGroup.particleList.erase(it);
                continue;
            }

            // 場の影響を計算（加速処理）
            // 移動処理（速度を座標に加算）
            particle.velocity += accelerationField_.acceleration * kDeltaTime_;

            // 速度を適応
            particle.transform.translate += particle.velocity * kDeltaTime_;

            // 時間経過を加算
            particle.currentTime += kDeltaTime_;

            // ワールド行列を計算（位置、回転、スケール）
            Matrix4x4 worldMatrix = MakeAffineMatrix(particle.transform.scale, particle.transform.rotate, particle.transform.translate);

            // ワールドビュー・プロジェクション行列を合成
            Matrix4x4 wvpMatrix = worldMatrix * viewProjectionMatrix;

            // インスタンシング用データ1コ分の書き込み
            uint32_t instanceIndex = static_cast<uint32_t>(std::distance(particleGroup.particleList.begin(), it));
            particleGroup.instancingData[instanceIndex].WVP = wvpMatrix;
            particleGroup.instancingData[instanceIndex].world = worldMatrix;
            particleGroup.instancingData[instanceIndex].color = particle.color;

            ++it;
        }
    }
}

void ParticleManager::Draw()
{
    // ルートシグネイチャの設定
    auto commandList = dxCommon_->GetCommandList();
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    // PSO (Pipeline State Object) を設定
    commandList->SetPipelineState(pipelineState_.Get());

    // プリミティブトポロジーの設定（例えば三角形リスト）
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // VBV (Vertex Buffer View) の設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);

    // すべてのパーティクルグループに対して処理
    for (auto& group : particleGroups)
    {
        ParticleGroup& particleGroup = group.second;

        // テクスチャのSRVのDescriptorTableを設定
        commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(particleGroup.materialData.textureFilePath));

        // インスタンシングデータのSRVのDescriptorTableを設定
        commandList->SetGraphicsRootDescriptorTable(3, srvManager_->GetGPUDescriptorHandle(particleGroup.srvIndex));

        // DrawCall (インスタンシング描画)
        // 1グループにつき1回のDrawInstancedを呼び出す
        commandList->DrawInstanced(UINT(particleGroup.particleList.size()), 1, 0, 0);
    }
}

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{
    // 登録されているパーティクルグループかチェック
    auto groupIt = particleGroups.find(name);
    assert(groupIt != particleGroups.end() && "Particle group not found.");

    // パーティクルグループの取得
    ParticleGroup& particleGroup = groupIt->second;

    // 最大インスタンス数を超えないようにチェック
    if (particleGroup.particleList.size() + count > particleGroup.kNumMaxInstance)
    {
        // エラーハンドリング：最大数を超えてしまう場合
        assert(false && "Exceeded max number of particles for this group.");
        return;
    }

    // 新しいパーティクルの生成
    for (uint32_t i = 0; i < count; ++i)
    {
        Particle newParticle;

        // 初期位置（引数のpositionを利用）
        newParticle.transform.translate = position;

        // ランダムな速度（例えばランダムな向きと速さ）
        newParticle.velocity = Vector3(
            std::uniform_real_distribution<float>(-1.0f, 1.0f)(randomEngine_),
            std::uniform_real_distribution<float>(-1.0f, 1.0f)(randomEngine_),
            std::uniform_real_distribution<float>(-1.0f, 1.0f)(randomEngine_)
        );

        // ランダムな色（例えばランダムにRGBを設定）
        newParticle.color = Vector4(
            std::uniform_real_distribution<float>(0.0f, 1.0f)(randomEngine_),
            std::uniform_real_distribution<float>(0.0f, 1.0f)(randomEngine_),
            std::uniform_real_distribution<float>(0.0f, 1.0f)(randomEngine_),
            1.0f // アルファ値を1に設定
        );

        // パーティクルリストに追加
        particleGroup.particleList.push_back(newParticle);
    }
}