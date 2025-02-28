#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>

#include "Collider.h"
#include "GameObject.h"

class Enemy : public GameObject
{
public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	
	// 移動
	void Move();
	
	// 場外処理
	void OutOfField();
	
	// 攻撃
	void Attack();
	
	// ImGui
	void ImGuiDraw();

private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);

public: //セッター

	void SetPlayerPos(const Vector3& _pos) { playerPos_ = _pos; }

private:

	std::unique_ptr<Object3d> enemy_ = nullptr;

	// 当たり判定関係
	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;
	bool isHit_ = false;
	bool isGround_ = false;
	
	// 移動速度
	Vector3 moveSpeed_ = { 0.1f,0.0f,0.1f };
	Vector3 moveVel_ = { 0.01f,0.01f,0.01f };
	// 落下速度
	float fallSpeed_ = 0.3f;
	
	// プレイヤーの位置
	Vector3 playerPos_{};
	// プレイヤーとの距離
	Vector3 toPlayer_{};

	// 攻撃
	bool isAttack_ = false;
	// 攻撃時間
	const float attackTime_ = 12;
	float attackTimeCounter_ = attackTime_;
	// 攻撃距離
	const Vector3 attackRange_ = { 0.8f,0.0f,0.8f };

};

