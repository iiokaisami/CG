#include <Windows.h>

#include <format>

#include <cassert>

#include <numbers>
#include <vector>
#include <DirectXMath.h>
#include <fstream>
#include <sstream>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "MyMath.h"

/// <summary>
///  dxCommmon->CompileShader
/// </summary>



//struct D3DResourceLeakChecker
//{
//	~D3DResourceLeakChecker()
//	{
//		//リソースリークチェック
//		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
//		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
//		{
//			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
//			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
//			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
//		}
//	}
//};



class ResourceObject
{
public:
	ResourceObject(ID3D12Resource* resource)
		:resource_(resource)
	{}

	~ResourceObject()
	{
		if (resource_)
		{
			resource_->Release();
		}
	}

	ID3D12Resource* Get() { return resource_; }

private:
	ID3D12Resource* resource_;
};

//depthStencilResource





//mtlファイルを読む関数
MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd")
		{
			std::string textureFilename;
			s >> textureFilename;

			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}

//Objファイルを読む関数
ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	//中で必要となる変数の宣言
	ModelData modelData; //構築するModelData
	std::vector<Vector4> positions; //位置
	std::vector<Vector3> normals; //法線
	std::vector<Vector2> texcoords; //テクスチャ座標
	std::string line; //ファイルから読んだ1行を格納するもの

	//ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら止める

	//実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; //先頭の識別子を読む

		//identifienに応じた処理
		if (identifier == "v")
		{
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt")
		{
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn")
		{
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f")
		{
			VertexData triangle[3];
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
			{
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点は要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element)
				{
					std::string index;
					std::getline(v, index, '/'); //  /区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				position.x *= -1.0f;
				normal.x *= -1.0f;

				texcoord.y = 1.0f - texcoord.y;

				//VertexData vertex = {position,texcoord,normal};
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib")
		{
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるので,ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	//ModelDataを返す
	return modelData;

}





//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	//D3DResourceLeakChecker leakCheck;


	// ポインタ
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;


	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	// スプライト共通部分の初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);



	
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = dxCommon->GetDevice();
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = dxCommon->GetCommandAllocator();
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon->GetCommandList();
	// DescriptorHeap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap = dxCommon->GetSrvDescriptorHeap();
    // descriptorSizeSRV
	uint32_t descriptorSizeSRV = dxCommon->GetDescriptorSizeSRV();
	// ビューポート
	D3D12_VIEWPORT viewport = dxCommon->GetViewport();
	// シザー矩形
	D3D12_RECT scissorRect = dxCommon->GetScissorRect();





	






	// ポインタ
	Input* input = nullptr;

	input = new Input();
	input->Initialize(winApp);


	//VertexResourceを生成する
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device, sizeof(VertexData) * 1536);

	


	/*
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 1536;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	*/

	



	//モデル読み込み
	ModelData modelData = LoadObjFile("resources", "axis.obj");
	//頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();				//リソースの先頭のアドレスから使う
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());	//使用するリソースのサイズは頂点のサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);		//１頂点当たりのサイズ

	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());



	/*
	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレス取得
	vertexResource->Map(
		0,
		nullptr,
		reinterpret_cast<void**>(&vertexData)
	);

	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };


	//左下２
	vertexData[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	vertexData[3].texcoord = { 0.0f,1.0f };
	//上２
	vertexData[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[4].texcoord = { 0.5f,0.0f };
	//右下２
	vertexData[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };
	*/



	



	



	//平行光源用のリソース
	DirectionalLight* directionalLightData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = dxCommon->CreateBufferResource(sizeof(DirectionalLight));

	directionalLightResource->Map(
		0,
		nullptr,
		reinterpret_cast<void**>(&directionalLightData)
	);

	//デフォルト値は以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = MyMath::Normalize({ 0.0f,-1.0f,0.0f });
	directionalLightData->intensity = 1.0f;








	//////////////////////////////////////
	std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 5; ++i)
	{
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon);
		sprites.push_back(sprite);

		Vector2 position = sprite->GetPosition();
		// 座標を変更する
		position.x = 100.0f * sprites.size();
		// 変更を反映する
		sprite->SetPosition(position);

		Vector2 size = sprite->GetSize();
		size.x = 70.0f;
		size.y = 70.0f;
		sprite->SetSize(size);
	}
	
	//////////////////////////////////////






	

	

	

	

	


	//TransformationMatrix用のResourceを作る

	//WVP用のリソースを作る。Matrix4x4　１つ分サイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = dxCommon->CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく

	wvpData->WVP = MyMath::MakeIdentity4x4();
	wvpData->World = MyMath::MakeIdentity4x4();


	//Transform変数を作る
	Transform transform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};


	Transform cameraTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0,-10.0f}
	};

	//UVTransform用の変数
	Transform uvTransformSprite{
		{ 1.0f,1.0f,1.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f }
	};

	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MyMath::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = MyMath::Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;



	
	
	

	


	//頂点位置を計算する

	/*
	//分割数
	const uint32_t kSubdivision = 16;
	//π(円周率)
	const float pi = std::numbers::pi_v<float>;
	//経度分割１つ分の角度φ
	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	//緯度分割１つ分の角度Θ
	const float kLatEvery = pi / float(kSubdivision);
	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		//緯度
		float lat = -pi / 2.0f + kLatEvery * latIndex;		//Θ
		//texcoord

		//経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			//texcoord
			float v = 1.0f - static_cast<float>(latIndex) / static_cast<float>(kSubdivision);
			float u = static_cast<float>(lonIndex) / static_cast<float>(kSubdivision);
			//経度
			float lon = lonIndex * kLonEvery;	//φ


			///===================================================================
			///1枚目の三角形
			///===================================================================

			//頂点にデータを入力する。基準点a 左下
			vertexData[start].position.x = std::cos(lat) * std::cos(lon);
			vertexData[start].position.y = std::sin(lat);
			vertexData[start].position.z = std::cos(lat) * std::sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u,v };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;

			//残りの５頂点も順番に計算して入力していく
			//基準点b　左上
			start++;
			vertexData[start].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[start].position.y = std::sin(lat + kLatEvery);
			vertexData[start].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u,v - 1.0f / kSubdivision };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;

			//基準点c　右下
			start++;
			vertexData[start].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[start].position.y = std::sin(lat);
			vertexData[start].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u + 1.0f / kSubdivision,v };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;


			///===================================================================
			///２枚目の三角形
			///===================================================================

			//基準点b　左上
			start++;
			vertexData[start].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[start].position.y = std::sin(lat + kLatEvery);
			vertexData[start].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u,v - 1.0f / kSubdivision };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;

			//基準点d　右上
			start++;
			vertexData[start].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[start].position.y = std::sin(lat + kLatEvery);
			vertexData[start].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u + 1.0f / kSubdivision ,v - 1.0f / kSubdivision };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;

			//基準点c 右下
			start++;
			vertexData[start].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[start].position.y = std::sin(lat);
			vertexData[start].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u + 1.0f / kSubdivision,v };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;
		}
	}
*/



//Textureを読んで転送する


	//2枚目
	DirectX::ScratchImage mipImages2 = dxCommon->LoadTexture(modelData.material.textureFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = dxCommon->CreateTextureResource(device, metadata2);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource2 = dxCommon->UploadTextureData(textureResource2, mipImages2);


	//実際にShaderResourceViewを作る



	DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = dxCommon->CreateTextureResource(device, metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = dxCommon->UploadTextureData(textureResource, mipImages);


	//metadataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	device->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);



	

	//metadata2をもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	

	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);

	//SRVの生成
	device->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);





	bool useMonsterBall = true;




	//メインループ
	//ウィンドウのxボタンが押されるまでループ
	while (true)
	{

		// Windowsのメッセージ処理
		if (winApp->ProcessMessage())
		{
			// ゲームループを抜ける
			break;
		}
		else
		{

			// 入力の更新
			input->Update();
			// 数字の0キーが押されていたら
			if (input->TriggerKey(DIK_0))
			{
				OutputDebugStringA("Hit 0\n");
			}

			//フレームの先頭でImGuiに、ここからフレームが始まる旨を告げる
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();



			//ゲームの処理		更新処理



			//ImGui


			ImGui::Begin("Setting");

			ImGui::Text("camera");
			ImGui::SliderFloat3("cameraPosition", &cameraTransform.translate.x, -100.0f, 0.0f);
			ImGui::Checkbox("useMonsterBall", &useMonsterBall);
			
			//改行
			ImGui::NewLine();

		
			
			if (ImGui::CollapsingHeader("vertexData"))
			{
				ImGui::SliderFloat3("translate", &transform.translate.x, -20.0f, 20.0f);
				ImGui::SliderAngle("rotationX", &transform.rotate.x);
				ImGui::SliderAngle("rotationY", &transform.rotate.y);
				ImGui::SliderAngle("rotationZ", &transform.rotate.z);
				ImGui::SliderFloat3("scale", &transform.scale.x, 0.0f, 5.0f);
			}
			if (ImGui::CollapsingHeader("Lighting"))
			{
				ImGui::ColorEdit4("color", &directionalLightData->color.x);
				ImGui::SliderFloat3("direction", &directionalLightData->direction.x, -1.0f, 1.0f);
				ImGui::SliderFloat("intensity", &directionalLightData->intensity, 0.0f, 1.0f);

			}

			if (ImGui::CollapsingHeader("UVTransform"))
			{
				ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
				ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
				ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
			}
			

			ImGui::End();


			//ゲームの処理が終わり描画処理に入る前にImGuiの内部コマンドを生成する
			ImGui::Render();


			worldMatrix = MyMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			cameraMatrix = MyMath::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			viewMatrix = MyMath::Inverse(cameraMatrix);
			projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
			wvpData->WVP = worldViewProjectionMatrix;
			wvpData->World = worldMatrix;



			



			Matrix4x4 uvTransformMatrix = MyMath::MakeScaleMatrix(uvTransformSprite.scale);
			uvTransformMatrix = MyMath::Multiply(uvTransformMatrix, MyMath::MakeRotateZMatrix(uvTransformSprite.rotate.z));
			uvTransformMatrix = MyMath::Multiply(uvTransformMatrix, MyMath::MakeTranslateMatrix(uvTransformSprite.translate));
			//materialDataSprite->uvTransform = uvTransformMatrix;


			for (Sprite* sprite : sprites)
			{
				sprite->Update();
			}

			//// 現在の座標を変数で受ける
			//Vector2 position = sprite->GetPosition();
			//// 座標を変更する
			//position.x += 0.1f;
			//position.y += 0.1f;
			//// 変更を反映する
			//sprite->SetPosition(position);

			// 角度を変化させるテスト
			/*float rotation = sprite->GetRotation();
			rotation += 0.01f;
			sprite->SetRotation(rotation);*/

			//// 色を変化させるテスト
			//Vector4 color = sprite->GetColor();
			//color.x += 0.01f;
			//if (color.x > 1.0f) 
			//{
			//	color.x -= 1.0f;
			//}
			//sprite->SetColor(color);

			// サイズを変化させるテスト
			/*Vector2 size = sprite->GetSize();
			size.x += 0.1f;
			size.y += 0.1f;
			sprite->SetSize(size);*/



			//ゲームの処理		描画処理








			//コマンドを積み込んで確定させる




			dxCommon->PreDraw();

			spriteCommon->CommonDrawSetting();

			for (Sprite* sprite : sprites)
			{
				sprite->Draw(textureSrvHandleGPU);
			}


			//いざ描画

			//commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
			//
			//
			////wvp用のCBufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			//

			//commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
			////描画！
			//commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);






			//実際のcommandListのImGuiの描画コマンドを積む
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());


			dxCommon->PostDraw();


			//次フレーム用のコマンドリストを準備
			hr = commandAllocator->Reset();
			assert(SUCCEEDED(hr));
			hr = commandList->Reset(commandAllocator.Get(), nullptr);
			assert(SUCCEEDED(hr));

		}
	}

	

	//ImGuiの終了処理。
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// 入力解放
	delete input;
	
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;

	// スプライト共通部分解放
	delete spriteCommon;

	// DirectX解放
	delete dxCommon;

	//スプライト解放
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}

	//CloseHandle(fenceEvent);
	

	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}

	return 0;
}