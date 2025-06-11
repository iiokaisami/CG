#pragma once

#include <string>

class EnemyManager;

class EnemyWaveState
{
public:

	EnemyWaveState(const std::string& _wave, EnemyManager* _pEnemyManager) : wave_(_wave), pEnemyManager_(_pEnemyManager) {};
	virtual~EnemyWaveState();

	virtual void Update() = 0;

protected:

	std::string wave_;
	EnemyManager* pEnemyManager_ = nullptr;

};

