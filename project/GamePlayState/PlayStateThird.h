#pragma once

#include "GamePlayState.h"

class PlayStateThird : public GamePlayState
{
public:

	PlayStateThird(GamePlayScene* _pScene);

	void Update();
};