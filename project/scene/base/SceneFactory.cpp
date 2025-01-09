#include "SceneFactory.h"

#include "../../scene/scene/TitleScene.h"
#include "../../scene/scene/GamePlayScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	BaseScene* newScene = nullptr;
	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlayScene();
	}

	return newScene;
}
