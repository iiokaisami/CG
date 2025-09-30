#include "SceneFactory.h"

#include "../scene/TitleScene.h"
#include "../scene/GamePlayScene.h"
#include "../scene/GameOverScene.h"
#include "../scene/ClearScene.h"

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
