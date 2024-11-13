#include "Object3dCommon.h"

void Object3dCommon::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
	device_ = dxCommon_->GetDevice();
	commandList_ = dxCommon_->GetCommandList();


	CreateGraphicsPipeline();
}

void Object3dCommon::CreateRootSignature()
{
}

void Object3dCommon::CreateGraphicsPipeline()
{
}

void Object3dCommon::CommonDrawSetting()
{
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	commandList_->SetGraphicsRootSignature(rootSignature_.Get());
	commandList_->SetPipelineState(graphicsPipelineState_.Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}