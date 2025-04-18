#include "Model.h"

#include <fstream>
#include <sstream>

#include "ModelCommon.h"

void Model::Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename)
{
	modelCommon_ = modelCommon;

	//モデル読み込み
	modelData_ = LoadObjFile(directorypath, filename);

	// VertexResourceを作る
	CreateVertexData();

	// マテリアルリソースを作る
	CreateMaterialData();

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);

}

void Model::UpData()
{}

void Model::Draw()
{
	// VertexBufferViewを設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// マテリアルCBufferの場所を設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	

	// SRVのDescriptorTableの先頭を設定。2はrootPatameter[2]である。
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));


	//描画！
	modelCommon_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
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

Model::ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	//中で必要となる変数の宣言
	ModelData modelData; //構築するModelData
	//std::vector<Vector4> positions; //位置
	//std::vector<Vector3> normals; //法線
	//std::vector<Vector2> texcoords; //テクスチャ座標
	//std::string line; //ファイルから読んだ1行を格納するもの

	////ファイルを開く
	//std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	//assert(file.is_open()); //とりあえず開けなかったら止める

	////実際にファイルを読み、ModelDataを構築していく
	//while (std::getline(file, line))
	//{
	//	std::string identifier;
	//	std::istringstream s(line);
	//	s >> identifier; //先頭の識別子を読む

	//	//identifienに応じた処理
	//	if (identifier == "v")
	//	{
	//		Vector4 position;
	//		s >> position.x >> position.y >> position.z;
	//		position.w = 1.0f;
	//		positions.push_back(position);
	//	}
	//	else if (identifier == "vt")
	//	{
	//		Vector2 texcoord;
	//		s >> texcoord.x >> texcoord.y;
	//		texcoords.push_back(texcoord);
	//	}
	//	else if (identifier == "vn")
	//	{
	//		Vector3 normal;
	//		s >> normal.x >> normal.y >> normal.z;
	//		normals.push_back(normal);
	//	}
	//	else if (identifier == "f")
	//	{
	//		VertexData triangle[3];
	//		//面は三角形限定。その他は未対応
	//		for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
	//		{
	//			std::string vertexDefinition;
	//			s >> vertexDefinition;
	//			//頂点は要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
	//			std::istringstream v(vertexDefinition);
	//			uint32_t elementIndices[3];
	//			for (int32_t element = 0; element < 3; ++element)
	//			{
	//				std::string index;
	//				std::getline(v, index, '/'); //  /区切りでインデックスを読んでいく
	//				elementIndices[element] = std::stoi(index);
	//			}
	//			//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
	//			Vector4 position = positions[elementIndices[0] - 1];
	//			Vector2 texcoord = texcoords[elementIndices[1] - 1];
	//			Vector3 normal = normals[elementIndices[2] - 1];

	//			position.x *= -1.0f;
	//			normal.x *= -1.0f;

	//			texcoord.y = 1.0f - texcoord.y;

	//			//VertexData vertex = {position,texcoord,normal};
	//			//modelData.vertices.push_back(vertex);
	//			triangle[faceVertex] = { position,texcoord,normal };
	//		}
	//		modelData.vertices.push_back(triangle[2]);
	//		modelData.vertices.push_back(triangle[1]);
	//		modelData.vertices.push_back(triangle[0]);
	//	}
	//	else if (identifier == "mtllib")
	//	{
	//		//materialTemplateLibraryファイルの名前を取得する
	//		std::string materialFilename;
	//		s >> materialFilename;
	//		//基本的にobjファイルと同一階層にmtlは存在させるので,ディレクトリ名とファイル名を渡す
	//		modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
	//	}
	//}

	////ModelDataを返す
	//return modelData;

	Assimp::Importer importer;
	std::string fullPath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(fullPath.c_str(),aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがない場合はエラー

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がない場合はエラー
		assert(mesh->HasTextureCoords(0)); // TexCoordがない場合はエラー

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形以外はエラー

			for (uint32_t element = 0; element < face.mNumIndices; ++element)
			{
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}

			for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex)
			{
				aiMaterial* material = scene->mMaterials[materialIndex];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0)
				{
					aiString textureFilePath;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
					modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();

				}
			}
		}

	}

	// ノードの情報を取得
	modelData.rootNode = ReadNode(scene->mRootNode);

	//ModelDataを返す
	return modelData;
}

void Model::CreateVertexData()
{
	// VertexResourceを作る
	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// VertexBufferViewを作成する(値を設定するだけ)
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();				//リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());	//使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);		//１頂点当たりのサイズ
	// VertexResourceにデータを描きこむためのアドレスを取得し vertexDataに割り当てる
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

}

void Model::CreateMaterialData()
{
	// マテリアルリソースを作る
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	// マテリアルリソースにデータを描きこむためのアドレスを取得し マテリアルリソースに割り当てる
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// マテリアルデータの初期値を描き来む
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;
	materialData_->phongReflection = false;
	materialData_->halfphongReflection = false;
	materialData_->pointLight = false;
	materialData_->spotLight = false;
}

Model::Node Model::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 行列を転置する
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.localMatrix.m[0][1] = aiLocalMatrix[1][0];
	result.localMatrix.m[0][2] = aiLocalMatrix[2][0];
	result.localMatrix.m[0][3] = aiLocalMatrix[3][0];
	result.localMatrix.m[1][0] = aiLocalMatrix[0][1];
	result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	result.localMatrix.m[1][2] = aiLocalMatrix[2][1];
	result.localMatrix.m[1][3] = aiLocalMatrix[3][1];
	result.localMatrix.m[2][0] = aiLocalMatrix[0][2];
	result.localMatrix.m[2][1] = aiLocalMatrix[1][2];
	result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	result.localMatrix.m[2][3] = aiLocalMatrix[3][2];
	result.localMatrix.m[3][0] = aiLocalMatrix[0][3];
	result.localMatrix.m[3][1] = aiLocalMatrix[1][3];
	result.localMatrix.m[3][2] = aiLocalMatrix[2][3];
	result.localMatrix.m[3][3] = aiLocalMatrix[3][3];

	result.name = node->mName.C_Str(); // nodeの名前を取得
	result.children.resize(node->mNumChildren); // 子ノードの数を取得
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
	{
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

void Model::SetEnableLighting(bool enable)
{
	if (materialData_)
	{
		materialData_->enableLighting = enable ? 1 : 0;
	}
}

void Model::SetEnableDirectionalLight(bool enable)
{
	if (materialData_)
	{
		materialData_->phongReflection = enable ? 1 : 0;
	}
}

void Model::SetEnablePointLight(bool enable)
{
	if (materialData_)
	{
		materialData_->pointLight = enable ? 1 : 0;
	}
}

void Model::SetEnableSpotLight(bool enable)
{
	if (materialData_)
	{
		materialData_->spotLight = enable ? 1 : 0;
	}
}
