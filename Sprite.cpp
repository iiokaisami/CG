#include "Sprite.h"

#include "SpriteCommon.h"

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;



	vertexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);
	indexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);
	materialResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	transformationMatrixResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点６つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズをインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;


	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));


	//IndexResourceにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// マテリアルデータにデータの初期値を書き込む
	//色を変える
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//Lightingを有効にする
	materialData_->enableLighting = false;
	materialData_->uvTransform = MyMath::MakeIdentity4x4();

	//書き込むためのアドレス
	transformationMatrixResource_->Map(0,nullptr,reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->World = MyMath::MakeIdentity4x4();
	transformationMatrixData_->WVP = MyMath::MakeIdentity4x4();

	
	transform_={
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
}

void Sprite::Update()
{

	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[3].position = { 1.0f,0.0f,0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,0.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	indexData_[0] = 0;		indexData_[1] = 1;		indexData_[2] = 2;
	indexData_[3] = 1;		indexData_[4] = 3;		indexData_[5] = 2;


	transform_.translate = { position_.x,position_.y ,0.0f};
	transform_.rotate = { 0.0f,0.0f,rotation_ };
	transform_.scale = { size_.x,size_.y,1.0f };


	Matrix4x4 worldMatrixSprite = MyMath::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 viewMatrixSprite = MyMath::MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MyMath::MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = MyMath::Multiply(worldMatrixSprite, MyMath::Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixData_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData_->World = worldMatrixSprite;

	
}

void Sprite::Draw(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU)
{
	

	//Spriteの描画。変更が必要なものだけ変更する
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);


	//マテリアルCBufferの場所を設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootPatameter[2]である。
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	//スプライトの描画(DrawCall//ドローコール)
	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


}