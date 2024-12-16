#include "ParticleManager.h"

#include <wrl.h>
#include <stdexcept>
#include <vector>
#include <random>

#include "Object3dCommon.h"

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
    object3dCommon_ = Object3dCommon::GetInstance();
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

    camera_ = object3dCommon_->GetDefaultCamera();
}

void ParticleManager::CreatePipeline()
{
    HRESULT result = S_FALSE;

    CreateRootSignature();

    //PSOを生成する
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
    D3D12_ROOT_PARAMETER rootParameter[4] = {};
    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[0].Descriptor.ShaderRegister = 0;

    rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParameter[1].DescriptorTable.pDescriptorRanges = descriptorRange_;
    rootParameter[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

    rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
    rootParameter[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

    rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameter[3].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う

    descriptionRootSignature_.pParameters = rootParameter;
    descriptionRootSignature_.NumParameters = _countof(rootParameter);

	//Smaplerの設定
	staticSamplers_[0].Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    staticSamplers_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    staticSamplers_[0].MaxLOD = D3D12_FLOAT32_MAX;
    staticSamplers_[0].ShaderRegister = 0;
    staticSamplers_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    descriptionRootSignature_.pStaticSamplers = staticSamplers_;
    descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers_);



    // シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    result = D3D12SerializeRootSignature(&descriptionRootSignature_,
        D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(result)) {
        Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
        assert(false);
    }
    // バイナリを元に生成
    result = dxCommon_->GetDevice()->CreateRootSignature(0,
        signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));
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
    blendDesc_.RenderTarget[0].BlendEnable = TRUE;
    blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	// RasterizerStateの設定
	// 裏面(時計回り)の表示の有無 (NONE / BACK)
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	// 塗りつぶすかどうか
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	// Shaderをコンパイル 
	vertexShaderBlob_ = dxCommon_->CompileShader(L"resources/shaders/Particle.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);
	pixelShaderBlob_ = dxCommon_->CompileShader(L"resources/shaders/Particle.PS.hlsl", L"ps_6_0");
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

void ParticleManager::Update()
{
    for (auto& group : particleGroups)
    {
        ParticleGroup& particleGroup = group.second;

       

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

            // ビュー行列とプロジェクション行列をカメラから取得
            Matrix4x4 worldMatrix = MakeAffineMatrix(particle.transform.scale, particle.transform.rotate, particle.transform.translate);
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

            // インスタンシング用データ1コ分の書き込み
            uint32_t instanceIndex = static_cast<uint32_t>(std::distance(particleGroup.particleList.begin(), it));
            particleGroup.instancingData[instanceIndex].WVP = worldViewProjectionMatrix;
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
