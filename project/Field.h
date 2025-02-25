#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>

#include "Collider.h"
#include "GameObject.h"

class Field : public GameObject
{
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	
private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);

private:

	std::unique_ptr<Object3d> field_ = nullptr;
	// 当たり判定関係
	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

};