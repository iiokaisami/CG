#pragma once

#include "GamePlayState.h"

class PlayStateSecond : public GamePlayState
{
public:
	
	PlayStateSecond(GamePlayScene* _pScene);
	
	void Update();
};

