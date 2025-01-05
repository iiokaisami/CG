#pragma once

#include "GamePlayState.h"

class PlayStateFirst : public GamePlayState
{
public:

	PlayStateFirst(GamePlayScene* _pScene);
	
	void Update();
};

