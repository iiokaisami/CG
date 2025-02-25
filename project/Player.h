#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>

#include "Collider.h"

class Player
{
public:
	
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	// 移動
	void Move();

private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);



private:

	std::unique_ptr<Object3d> player_ = nullptr;

	Vector3 position_ = { 0.0f,0.0f,-10.0f };

	// 当たり判定関係
	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;
	bool isHit_ = false;
	bool isGround_ = true;

};

