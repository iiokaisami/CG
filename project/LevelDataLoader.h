#pragma once

#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>

#include <Model.h>
#include <Object3d.h>

#include "externals/json/json.hpp"

class LevelData 
{
public:

	struct ObjectData 
	{
		std::string fileName;
		Vector3 translation{};
		Vector3 rotation{};
		Vector3 scale{ 1.0f, 1.0f, 1.0f };
	};

	std::vector<ObjectData> objects;
};

class LevelDataLoader
{
public:

	static constexpr const char* kDefaultBaseDirectory = "resources/levels/";
	static constexpr const char* kExtension = ".json";

	LevelData* LoadLevelData(const std::string& fileName);

private:

	void LoadObjectsRecursive(const nlohmann::json& objectsJson, LevelData& levelData);

	// ファイル読み込み 以下処理
	/*
	// 連結してフルパスを得る
	std::string fullPath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);
	// ファイルオープンチェック
	if (file.fail())
	{
		assert(0 && "Failed to open level data file.");
	}

	*/


	// ファイルチェック 以下処理
	/*
	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object() && "Invalid level data file format.");
	assert(deserialized.contains("name") && "Level data file must contain 'name' array.");
	assert(deserialized["name"].is_string() && "Level data 'name' must be a string.");

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0 && "Level data file must be 'scene'.");

	*/


	// オブジェクト走査 以下処理
	/*
	// レベルデータ格納用のインスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for(nlohmann::json& object : deserialized["objects"])
	{
	assert(object.constains("type") && "Object must contain 'type' field.");

	// 種別を取得
	std::string type = object["type"].get<std::string>();

    種類ごとの処理

	再帰処理

	}

	*/

	// メッシュの読み込み 以下処理
	/*
	// MESH
	if(type.compare("MESH") == 0)
	{
	// 要素追加
	levelData->objects.emplace_back(LevelData::ObjectData{});
	// 今追加した要素の参照を得る
	LevelData::ObjectData& objectData = levelData->objects.back();

	if(!object.contains("file_name"))
	{
	// ファイル名
	objectData.fileName = object["file_name"];
	
	}

	// トランスフォームのパラメータ読み込み
	nlohmann::json& transform = object["transform"];

	// 平行移動
	objectData.translation.x = (float)transform["translation"][0];
	objectData.translation.y = (float)transform["translation"][1];
	objectData.translation.z = (float)transform["translation"][2];

	// 回転角
	objectData.rotation.x = -(float)transform["rotation"][0];
	objectData.rotation.y = -(float)transform["rotation"][1];
	objectData.rotation.z = -(float)transform["rotation"][2];

	// スケーリング
	objectData.scale.x = (float)transform["scale"][0];
	objectData.scale.y = (float)transform["scale"][1];
	objectData.scale.z = (float)transform["scale"][2];

	// TODO: コライダーのパラメータ読み込み

	}

	*/

	// "objects"の全オブジェクトを走査 以下処理
    /*
	for(nlohmann::json& object : deserialized["objects"])
	{
	assert(object.contains("type") && "Object must contain 'type' field.");

	// 種別を取得
	std::string type = object["type"].get<std::string>();

	種類ごとの処理

	// TODO: オブジェクト走査を再起関数にまとめ、再帰呼び出しで枝を走査する
	if(object.contains("children"))
	{

	}

	}
	*/

};

// 使用例（シーン制御側）
/*
✅ 使用例（シーン側）
cpp
コードをコピーする
// LevelDataLoaderで読み込み
std::unique_ptr<LevelData> levelData = LevelDataLoader::LoadLevelData("stage01");

// 読み込んだデータからオブジェクトを生成
for (const auto& obj : levelData->objects) {
	// モデル検索
	Model* model = nullptr;
	auto it = models.find(obj.fileName);
	if (it != models.end()) {
		model = it->second;
	}

	// Object3d生成（モデル付き）
	Object3d* newObj = Object3d::Create(model);
	newObj->SetPosition(obj.translation);
	newObj->SetRotate(obj.rotation);
	newObj->SetScale(obj.scale);

	// シーンに登録
	objects.push_back(newObj);
}
*/

// 以下シーン制御クラス このように使いたい
/*

// レベルデータからオブジェクトを生成、配置
for(auto& objectData : levelData->objects)
{

// ファイル名から登録済みのモデルを検索
Model* model = nullptr;
decltype(models)::iterator it = models.find(objectData.fileName);
if (it != models.end())
{
	model = it->second;
}
// モデルを指定して3Dオブジェクトを生成
Object3d* newObject = Object3d::Create(model);
// 座標
newObject->SetPosition(objectData.position);
// 回転角
newObject->SetRotate(objectData.rotation);
// スケール
newObject->SetScale(objectData.scale);
// 解列に登録
objects.push_back(newObject);

}

*/