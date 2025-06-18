#pragma once

#include <sstream>

class Enemy;

class EnemyBehaviorState
{
public:

	EnemyBehaviorState(const std::string& _state, Enemy* _pEnemy) : pEnemy_(_pEnemy) {};
	virtual ~EnemyBehaviorState();
	
	// 初期化
	virtual void Initialize() = 0;
	
	// 更新
	virtual void Update() = 0;

protected:

	struct Motion
	{
		// モーションがアクティブかどうか
		bool isActive = false;
		// カウント
		uint32_t count = 0;
		// Maxカウント
		uint32_t maxCount = 30;
	};

	void MotionCount(Motion _motion);

	std::string stateName_;
	Enemy* pEnemy_ = nullptr;

	Motion motion_;

};

