#include "LevelDataLoader.h"

std::unique_ptr<LevelData> LevelDataLoader::LoadLevelData(const std::string& fileName)
{
	std::string fullPath = kDefaultBaseDirectory + fileName + kExtension;

	std::ifstream file(fullPath);
	if (file.fail())
	{
		assert(false && "Failed to open level data file.");
		return nullptr;
	}

	nlohmann::json deserialized;
	file >> deserialized;

	assert(deserialized.is_object());
	assert(deserialized.contains("name") && deserialized["name"].is_string());
	assert(deserialized["name"] == "scene");

	auto levelData = std::make_unique<LevelData>();

	for (const auto& object : deserialized["objects"])
	{
		LoadObjectRecursive(object, levelData.get());
	}

	return levelData;
}

void LevelDataLoader::LoadObjectRecursive(const nlohmann::json& objectJson, LevelData* levelData)
{
    assert(objectJson.contains("type") && objectJson["type"].is_string());

    if (objectJson.contains("disable_export"))
    {
        // 有効無効フラグ
        bool disabled = objectJson["disable_export"].get<bool>();
        if (!disabled)
        {
            // このオブジェクトは配置しない
            return;
        }
    }

    std::string type = objectJson["type"];

    if (type == "MESH")
    {
        LevelData::ObjectData data;
        data.type = type;

        if (objectJson.contains("file_name")) 
        {
            std::string fileName = objectJson["file_name"];
            // すでに .obj で終わっていなければ付与
			// 4文字以下の場合 or 最後の4文字が ".obj" でない場合
            if (fileName.length() < 4 or fileName.substr(fileName.length() - 4) != ".obj") 
            {
                fileName += ".obj";
            }
            data.fileName = objectJson["file_name"];
        }

        if (objectJson.contains("transform"))
        {
            const auto& t = objectJson["transform"];
            data.translation = { t["translation"][0], t["translation"][1], t["translation"][2] };
            data.rotation = { -1.0f * t["rotation"][0], -1.0f * t["rotation"][1], -1.0f * t["rotation"][2] };
            data.scale = { t["scaling"][0], t["scaling"][1], t["scaling"][2] };
        }

        levelData->objects.emplace_back(std::move(data));
    }

    // 再帰 子オブジェクトの走査
    if (objectJson.contains("children")) 
    {
        for (const auto& child : objectJson["children"]) 
        {
            LoadObjectRecursive(child, levelData);
        }
    }
}
