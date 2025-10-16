#pragma once

#include "BaseScene.h"
#include <string>

/// <summary>
/// シーン生成の抽象クラス
/// 継承先でCreateSceneを実装してシーンを生成する
/// </summary>
class AbstractSceneFactory
{
public:
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;

	// シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};