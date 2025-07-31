#pragma once

#include <sstream>
#include <Vector3.h>

class TrapEnemy;

class TrapEnemyBehaviorState
{
public:
	TrapEnemyBehaviorState(const std::string& _name, TrapEnemy* _trapEnemy) : stateName_(_name), pTrapEnemy_(_trapEnemy) {};
	virtual ~TrapEnemyBehaviorState();
	
	// 初期化
	virtual void Initialize() = 0;
	
	// 更新
	virtual void Update() = 0;
	
	// モーションのリセット
	virtual void ResetMotion() = 0;

protected:

	struct Transform
	{
		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Vector3 rotation = { 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
	};

	struct Motion
	{
		// モーションがアクティブかどうか
		bool isActive = false;
		// カウント
		uint32_t count = 0;
		// Maxカウント
		uint32_t maxCount = 30;
		// トランスフォーム
		Transform transform;
	};

	// モーションのカウント
	void MotionCount(Motion& _motion);

	// 敵のトランスフォームをmotion_.transformにセット
	void TransformUpdate(TrapEnemy* _pEnemy);

	std::string stateName_;
	TrapEnemy* pTrapEnemy_ = nullptr;

	Motion motion_;

};

