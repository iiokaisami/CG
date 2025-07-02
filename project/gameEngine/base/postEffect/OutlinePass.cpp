#include "OutlinePass.h"

#include <cassert>

#include "../DirectXCommon.h"
#include "../SrvManager.h"

void OutlinePass::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, const std::wstring& vsPath, const std::wstring& psPath)
{
    BasePostEffectPass::Initialize(dxCommon, srvManager, vsPath, psPath);

    constantBuffer_ = dxCommon->CreateUploadBuffer(sizeof(cbData_));

    HRESULT hr = constantBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedCB_));
    assert(SUCCEEDED(hr) && "Failed to map constant buffer");
    hr;

    // 定数バッファ作成
	constantBuffer_ = dxCommon->CreateUploadBuffer(sizeof(cbData_));
	hr = constantBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedCB_));
	assert(SUCCEEDED(hr) && "Failed to map constant buffer");
	// 深度バッファのSRVを設定
    if (depthResource_)
    {
        // SRVインデックスを確保
		depthSrvIndex_ = srvManager->Allocate();
        // SRV生成
		srvManager_->CreateSRVforTexture2D(depthSrvIndex_, depthResource_, DXGI_FORMAT_R24_UNORM_X8_TYPELESS, 1);
    }

}

void OutlinePass::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE inputSrvHandle, ID3D12Resource* inputResource, D3D12_RESOURCE_STATES& currentState)
{
    currentState;
    inputResource;

    if (!isActive_)
    {
        return;
    }

    // 深度バッファをPSRに遷移
	if (depthResource_ && depthResourceState_ != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = depthResource_;
		barrier.Transition.StateBefore = depthResourceState_;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		cmdList->ResourceBarrier(1, &barrier);
		depthResourceState_ = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	}

    // CBV(定数バッファ)のアップロード等
    UpdateConstantBuffer();

    // SRVバインド
	cmdList->SetGraphicsRootDescriptorTable(0, inputSrvHandle);
    if (depthResource_)
    {
		cmdList->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(depthSrvIndex_));
    }

    cmdList->SetPipelineState(pipelineState_.Get());
    cmdList->SetGraphicsRootSignature(rootSignature_.Get());
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ID3D12DescriptorHeap* heaps[] = {
        srvManager_->GetHeap().Get(),
        dxCommon_->GetSamplerHeap().Get()
    };
    cmdList->SetDescriptorHeaps(_countof(heaps), heaps);

    cmdList->SetGraphicsRootDescriptorTable(0, inputSrvHandle);
    //cmdList->SetGraphicsRootDescriptorTable(1, dxCommon_->GetSamplerHandle());
    cmdList->SetGraphicsRootConstantBufferView(2, constantBuffer_->GetGPUVirtualAddress());

    cmdList->DrawInstanced(3, 1, 0, 0);

	// 深度バッファをDEPTH_WRITEに戻す
	if (depthResource_ && depthResourceState_ != D3D12_RESOURCE_STATE_DEPTH_WRITE)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = depthResource_;
		barrier.Transition.StateBefore = depthResourceState_;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		cmdList->ResourceBarrier(1, &barrier);
		depthResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	}

}

void OutlinePass::UpdateConstantBuffer()
{
    if (mappedCB_)
    {
        *mappedCB_ = cbData_;
    }
}
