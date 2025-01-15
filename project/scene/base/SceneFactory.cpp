#include "SceneFactory.h"

#include "../../scene/scene/TitleScene.h"
#include "../../scene/scene/GamePlayScene.h"
#include "../../scene/scene/GameClearScene.h"
#include "../../scene/scene/GameOverScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") 
	{
		newScene = new TitleScene();
	}
	if (sceneName == "GAMEPLAY") 
	{
		newScene = new GamePlayScene();
	}
	if (sceneName == "GAMECLEAR") 
	{
		newScene = new GameClearScene();
	}
	if (sceneName == "GAMEOVER") 
	{
		newScene = new GameOverScene();
	}

	return newScene;
}
