#pragma once

#include "AbstractSceneFactory.h"

/// <summary>
/// シーンファクトリ
/// シーンの生成を行う
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:
	// シーン生成
	BaseScene* CreateScene(const std::string& sceneName) override;
};