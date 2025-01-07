#include "Sprite.h"

#include "SpriteCommon.h"

#include <fstream>

void Sprite::Initialize(std::string textureFilePath,
	Vector2 position, Vector4 color, Vector2 anchorpoint)
{
	spriteCommon_ = SpriteCommon::GetInstance();

	std::ifstream file;
	// 基本パスを指定（"Resources/images/"）
	std::string basePath = "resources/images/";
	std::string fullPath = basePath + textureFilePath;
	textureFilePath_ = fullPath;

	vertexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);
	indexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);
	materialResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	transformationMatrixResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	TextureManager::GetInstance()->LoadTexture(textureFilePath_);
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
	materialData_->uvTransform = MakeIdentity4x4();

	//書き込むためのアドレス
	transformationMatrixResource_->Map(0,nullptr,reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->World = MakeIdentity4x4();
	transformationMatrixData_->WVP = MakeIdentity4x4();

	
	transform_={
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	// 単位行列を書き込んでおく
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath_);

	AdjustTextureSize();

	SetPosition(position);
	SetColor(color);
	SetAnchorPoint(anchorpoint);

	color_ = color;
}

void Sprite::Update()
{
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// 左右反転
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}

	// 上下反転
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	// 色と透明度を更新
	if (materialData_ != nullptr)
	{
		materialData_->color = color_;
	}

	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureFilePath_);
	float tex_left = textureLeftTop_.x / metadata.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x)/ metadata.width;
	float tex_top = textureLeftTop_.y / metadata.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;


	vertexData_[0].position = { left,bottom,0.0f,1.0f };
	vertexData_[0].texcoord = { tex_left,tex_bottom };
	//vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[1].position = { left,top,0.0f,1.0f };
	vertexData_[1].texcoord = { tex_left,tex_top };
	//vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[2].position = { right,bottom,0.0f,1.0f };
	vertexData_[2].texcoord = { tex_right,tex_bottom };
	//vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[3].position = { right,top,0.0f,1.0f };
	vertexData_[3].texcoord = { tex_right,tex_top };
	//vertexData_[3].texcoord = { 1.0f,0.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	indexData_[0] = 0;		indexData_[1] = 1;		indexData_[2] = 2;
	indexData_[3] = 1;		indexData_[4] = 3;		indexData_[5] = 2;


	transform_.translate = { position_.x,position_.y ,0.0f};
	transform_.rotate = { 0.0f,0.0f,rotation_};
	transform_.scale = { size_.x,size_.y,1.0f };


	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixData_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData_->World = worldMatrixSprite;

	
}

void Sprite::Draw()
{

	// Spriteの描画。変更が必要なものだけ変更する
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);


	// マテリアルCBufferの場所を設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// TransformationMatrixCBufferの場所を設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定。2はrootPatameter[2]である。
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));
	// スプライトの描画(DrawCall//ドローコール)
	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


}

void Sprite::SetColorChange(const Vector4& color)
{
	color_ = color;
	if (materialData_ != nullptr)
	{
		materialData_->color = color_;
	}
}

void Sprite::AdjustTextureSize()
{
	// テクスチャメタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureFilePath_);

	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);
	// 画像サイズをテクスチャサイズに合わせる
	size_ = textureSize_;
}
