#include "ParticleManager.h"

#include <wrl.h>
#include <stdexcept>
#include <vector>
#include <random>
#include <numbers>

#include "Object3dCommon.h"
#include "ModelManager.h"

ParticleManager* ParticleManager::instance_ = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new ParticleManager();
    }
    return instance_;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
    dxCommon_ = dxCommon;
    srvManager_ = srvManager;
    object3dCommon_ = Object3dCommon::GetInstance();
    // ランダムエンジンの初期化
    randomEngine_ = std::mt19937{ std::random_device{}() };

    // パイプライン生成
    CreatePipeline();

    transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

    // ビルボード行列の作成
    backToFrontMatrix_ = MakeRotateYMatrix(std::numbers::pi_v<float>);

    //マテリアル
    //modelマテリアる用のリソースを作る。今回color1つ分のサイズを用意する
    materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));
    //マテリアルにデータを書き込む
    materialData_ = nullptr;
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    //色
    materialData_->color = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
    materialData_->enableLighting = false;//有効にするか否か
    materialData_->uvTransform = MakeIdentity4x4();

    camera_ = object3dCommon_->GetDefaultCamera();
}

void ParticleManager::Finalize()
{
    delete instance_;
    instance_ = nullptr;
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
    D3D12_ROOT_PARAMETER rootParameter[3] = {};
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

    descriptionRootSignature_.pParameters = rootParameter;
    descriptionRootSignature_.NumParameters = _countof(rootParameter);

    //Smaplerの設定
    staticSamplers_[0].Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    staticSamplers_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
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
    blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;//zero
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
    depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void ParticleManager::CreateParticleGroup(const std::string& name, const std::string& textureFilePath, const std::string& modelFilePath, bool isMakeRing)
{
    ModelManager::GetInstance()->LoadModel(modelFilePath);

    model_ = ModelManager::GetInstance()->FindModel(modelFilePath);

    if (particleGroups.contains(name))
    {
        return;
    }

    // パーティクルグループを作成、コンテナに登録
    ParticleGroup newGroup = {};
    particleGroups.insert(std::make_pair(name, std::move(newGroup)));
    // テクスチャファイルパスを登録
    particleGroups.at(name).materialData.textureFilePath = textureFilePath;
    // テクスチャを読み込んでSRVを生成
    TextureManager::GetInstance()->LoadTexture(textureFilePath);
    // SRVインデックスを登録
    particleGroups.at(name).materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
    // 最大数インスタンス
    uint32_t MaxInstanceCount = 1024;
    // インスタンス数を初期化
    particleGroups.at(name).instanceCount = 0;
    // インスタンス用リソースを生成
    particleGroups.at(name).instancingResource = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * MaxInstanceCount);
    // インスタンス用リソースをマップ
    particleGroups.at(name).instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroups.at(name).instancingData));
    // インスタンスのデータを初期化
    ParticleForGPU particleForGPU = {};
    particleForGPU.WVP = MakeIdentity4x4();
    particleForGPU.world = MakeIdentity4x4();
    particleForGPU.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    // インスタンスのデータを登録
    for (uint32_t i = 0; i < MaxInstanceCount; ++i)
    {
        particleGroups.at(name).instancingData[i] = particleForGPU;
    }

    // インスタンス用のSRVインデックス
    particleGroups.at(name).srvIndex = srvManager_->Allocate();
    // srvを生成
    srvManager_->CreateSRVforStructuredBuffer(particleGroups.at(name).srvIndex, particleGroups.at(name).instancingResource.Get(), MaxInstanceCount, sizeof(ParticleForGPU));

   
    if (isMakeRing)
    {
        // リングの頂点データを生成
        MakeRing();
    }
}

void ParticleManager::Update()
{
    camera_ = object3dCommon_->GetDefaultCamera();

    Matrix4x4 matrix = backToFrontMatrix_ * camera_->GetViewMatrix();
    matrix.m[3][0] = 0.0f;
    matrix.m[3][1] = 0.0f;
    matrix.m[3][2] = 0.0f;

    Matrix4x4 viewMatrix = camera_->GetViewMatrix();
    Matrix4x4 projectionMatrix = camera_->GetProjectionMatrix();

    for (auto& [name, Particlegroup] : particleGroups)
    {
        uint32_t count = 0;
        // 各パーティクルの処理
        for (auto it = Particlegroup.particleList.begin(); it != Particlegroup.particleList.end();)
        {
            // 寿命が終わったパーティクルを削除
            if ((*it).currentTime >= (*it).lifeTime)
            {
                it = Particlegroup.particleList.erase(it);
                continue;
            }

            // パーティクルの位置を更新
            (*it).transform.translate += (*it).velocity * kDeltaTime_;
            // パーティクルの寿命
            (*it).currentTime += kDeltaTime_;
            float alpha = 1.0f - ((*it).currentTime / (*it).lifeTime);

            // アルファ値をパーティクルの色に適用
            (*it).color.w = alpha; 

            Matrix4x4 worldMatrix = MakeScaleMatrix((*it).transform.scale)  *
                MakeRotateXMatrix((*it).transform.rotate.x) *
                MakeRotateYMatrix((*it).transform.rotate.y) * 
                MakeRotateZMatrix((*it).transform.rotate.z) * 
                matrix * MakeTranslateMatrix((*it).transform.translate);
            Matrix4x4 wVPMatrix = worldMatrix * viewMatrix * projectionMatrix;

            if (count < Particlegroup.instanceCount)
            {
                Particlegroup.instancingData[count].WVP = wVPMatrix;
                Particlegroup.instancingData[count].world = worldMatrix;
                Particlegroup.instancingData[count].color = it->color;
                ++count;
            }
           

            // 次のパーティクルへ
            ++it;
        }
    }
}

void ParticleManager::Draw()
{
    // パーティクルグループが設定されていなければ描画しない
    if (particleGroups.empty())
    {
        return;
    }

    // ルートシグネイチャの設定
    dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    // PSO (Pipeline State Object) を設定
    dxCommon_->GetCommandList()->SetPipelineState(pipelineState_.Get());
    // プリミティブトポロジーの設定
    dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   
    // インデックスバッファを設定(モデルから取得)
    indexBufferView_ = model_->GetIndexBufferView();
    dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

    // パーティクルグループごとに描画
    for (const auto& [name, ParticleGroup] : particleGroups)
    {
        // インスタンス数が0なら描画しない
        if (ParticleGroup.instanceCount == 0)
        {
            continue;
        }

        // VertexBufferViewを設定
        vertexBufferView_ = model_->GetVertexBufferView();
        dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
        // マテリアルのCBufferを設定
        dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
        // インスタンシングデータのSRVを設定
        srvManager_->SetGraphicsRootDescriptorTable(2, ParticleGroup.materialData.textureIndex);
        // テクスチャのSRVを設定
        srvManager_->SetGraphicsRootDescriptorTable(1, ParticleGroup.srvIndex);
       
        
        // DrawCall (インスタンシング描画)
        dxCommon_->GetCommandList()->DrawIndexedInstanced(
            static_cast<UINT>(model_->GetModelData().indices.size()), 
            ParticleGroup.instanceCount, 
            0, 0, 0);

    }
}

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{
    // パーティクルグループが登録されているかチェック
    assert(particleGroups.contains(name));
    // パーティクルグループのパーティクルリストに新しいパーティクルを追加
    for (uint32_t i = 0; i < count; ++i)
    {
        // 新しいパーティクルを追加
        //particleGroups.at(name).particleList.push_back(MakeNewParticle(randomEngine_, position));
    
        particleGroups.at(name).particleList.push_back(MakeTestParticle(randomEngine_, position));
    }
    // パーティクルグループのインスタンス数を更新
    particleGroups.at(name).instanceCount = count;

}

Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& position)
{
    std::uniform_real_distribution<float>distribution(-1.0f, 1.0f);
    std::uniform_real_distribution<float>distColor(0.0f, 1.0f);
    std::uniform_real_distribution<float>distLifeTime(1.0f, 3.0f);

    Particle newParticle;
    Vector3 randomTranslate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };

    newParticle.transform.scale = { 1.0f,1.0f,1.0f };
    newParticle.transform.translate = position + randomTranslate;
    newParticle.velocity = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
    newParticle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
    newParticle.lifeTime = distLifeTime(randomEngine);
    newParticle.currentTime = 0.0f;

    return newParticle;
}

Particle ParticleManager::MakeTestParticle(std::mt19937& randomEngine, const Vector3& translate)
{
    std::uniform_real_distribution<float> distScale(0.5f, 1.5f); // スケールの範囲
    std::uniform_real_distribution<float> distRotate(-std::numbers::pi_v<float>, std::numbers::pi_v<float>); // 回転の範囲
    std::uniform_real_distribution<float> distVelocity(-1.0f, 1.0f); // 速度の範囲


    Particle particle;

    particle.transform.scale = { 0.1f, distScale(randomEngine), 1.0f};
    particle.transform.rotate = { 0.0f, 0.0f, distRotate(randomEngine) };
    particle.transform.translate = translate;
    particle.velocity = {0.0f,0.0f,0.0f};
    particle.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    particle.lifeTime = 1.0f;
    particle.currentTime = 0.0f;

    return particle;
}

void ParticleManager::MakeRing()
{
    const uint32_t kRingDivide = 32; // 分割数
    const float kOuterRadius = 1.0f; // 外側の半径
    const float kInnerRadius = 0.2f; // 内側の半径
    const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide);

    for (uint32_t index = 0; index < kRingDivide; ++index)
    {
        float sin = std::sin(index * radianPerDivide);
        float cos = std::cos(index * radianPerDivide);
        float sinNext = std::sin((index + 1) * radianPerDivide);
        float cosNext = std::cos((index + 1) * radianPerDivide);
        float u = float(index) / float(kRingDivide);
        float uNext = float(index + 1) / float(kRingDivide);

        // 頂点追加 + 基準インデックス保存
        uint32_t baseIndex = model_->GetVertexCount();

        // 三角形1: 外側の現在の頂点 -> 内側の現在の頂点 -> 外側の次の頂点
        model_->AddVertex({ -sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f }, { u, 0.0f }, { 0.0f, 0.0f, 1.0f });
        model_->AddVertex({ -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f });
        model_->AddVertex({ -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f });

        model_->AddIndex(baseIndex + 0);
        model_->AddIndex(baseIndex + 1);
        model_->AddIndex(baseIndex + 2);

        // 三角形2: 外側の次の頂点 -> 内側の現在の頂点 -> 内側の次の頂点
        model_->AddVertex({ -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f });
        model_->AddVertex({ -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f });
        model_->AddVertex({ -sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f }, { uNext, 1.0f }, { 0.0f, 0.0f, 1.0f });
    
        model_->AddIndex(baseIndex + 3);
        model_->AddIndex(baseIndex + 4);
        model_->AddIndex(baseIndex + 5);
    
    }

	// すべての頂点・インデックス追加後にバッファを一括更新
	model_->UpdateVertexBuffer();
	model_->UpdateIndexBuffer();

}