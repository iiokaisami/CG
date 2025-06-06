#include "LevelDataLoader.h"

LevelData* LevelDataLoader::LoadLevelData(const std::string& fileName)
{
	std::string fullPath = kDefaultBaseDirectory + fileName + kExtension;
	std::ifstream file(fullPath);
	assert(file && "Failed to open level data file.");

	nlohmann::json deserialized;
	file >> deserialized;

	assert(deserialized.is_object() && "Invalid level data file format.");
	assert(deserialized.contains("name") && deserialized["name"].is_string());
	assert(deserialized["name"] == "scene" && "Level data file must be 'scene'.");

	LevelData* levelData = new LevelData();
	LoadObjectsRecursive(deserialized["objects"], *levelData);
	return levelData;
}

void LevelDataLoader::LoadObjectsRecursive(const nlohmann::json& objectsJson, LevelData& levelData)
{
	for (const auto& object : objectsJson) {
		assert(object.contains("type"));
		std::string type = object["type"].get<std::string>();

		if (type == "MESH") {
			LevelData::ObjectData objData;
			if (object.contains("file_name")) {
				objData.fileName = object["file_name"].get<std::string>();
			}

			nlohmann::json transform = object["transform"];
			objData.translation = Vector3(
				(float)transform["translation"][0],
				(float)transform["translation"][1],
				(float)transform["translation"][2]);
			objData.rotation = Vector3(
				-(float)transform["rotation"][0],
				-(float)transform["rotation"][1],
				-(float)transform["rotation"][2]);
			objData.scale = Vector3(
				(float)transform["scale"][0],
				(float)transform["scale"][1],
				(float)transform["scale"][2]);

			levelData.objects.push_back(objData);
		}

		if (object.contains("children")) {
			LoadObjectsRecursive(object["children"], levelData);
		}
	}
}
