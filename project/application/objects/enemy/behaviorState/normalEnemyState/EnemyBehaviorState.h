#pragma once

#include <sstream>
#include <Vector3.h>

class NormalEnemy;

/// <summary>
/// 敵の行動ステート基底クラス
/// </summary>
class EnemyBehaviorState
{
public:

	EnemyBehaviorState(const std::string& _state, NormalEnemy* _pNormalEnemy) :stateName_(_state), pNormalEnemy_(_pNormalEnemy) {};
	virtual ~EnemyBehaviorState();
	
	// 初期化
	virtual void Initialize() = 0;
	
	// 更新
	virtual void Update() = 0;

	// モーションのカウント等をリセット
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
	void TransformUpdate(NormalEnemy* _pEnemy);

	std::string stateName_;
	NormalEnemy* pNormalEnemy_ = nullptr;

	Motion motion_;

};

