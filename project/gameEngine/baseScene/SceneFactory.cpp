#include "SceneFactory.h"

#include "../../application/scene/TitleScene.h"
#include "../../application/scene/GamePlayScene.h"
#include "../../application/scene/GameOverScene.h"
#include "../../application/scene/ClearScene.h"

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
	else if (sceneName == "GAMEOVER")
	{
		newScene = new GameOverScene();
	}
	else if (sceneName == "CLEAR")
	{
		newScene = new ClearScene();
	}

	return newScene;
}
