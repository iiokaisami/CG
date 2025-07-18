#pragma once

#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>

#include <Model.h>
#include <Object3d.h>

#include "../externals/json/json.hpp"

class LevelData 
{
public:

	// プレイヤーのスポーンデータ
	struct PlayerSpawnData
	{
		Vector3 position;
		Vector3 rotation;
	};

	struct ObjectData 
	{
		std::string type;
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scale;
	};

	std::vector<ObjectData> objects;


	// プレイヤー配列
	std::vector<PlayerSpawnData> players;

};

class LevelDataLoader
{
public:

	static constexpr const char* kDefaultBaseDirectory = "resources/levels/";
	static constexpr const char* kExtension = ".json";

	static std::unique_ptr<LevelData> LoadLevelData(const std::string& fileName);

private:

	static void LoadObjectRecursive(const nlohmann::json& objectJson, LevelData* levelData);

};