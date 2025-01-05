#pragma once

#include <string>

class GamePlayScene;

class GamePlayState
{
public:

	GamePlayState(const std::string& _scene, GamePlayScene* _pPlayScene) : scene_(_scene), pPlayScene_(_pPlayScene) {};
	virtual ~GamePlayState();
	
	virtual void Update() = 0;

protected:

	std::string scene_;
	GamePlayScene* pPlayScene_ = nullptr;

};

