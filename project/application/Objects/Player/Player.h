#pragma once

#include "../application/BaseObject/GameObject.h"
#include "Bullet/PlayerBullet.h"

#include <Object3d.h>
#include <Sprite.h>
#include <Framework.h>

class Player : public GameObject
{
public:

	Player() = default;
	~Player() = default;
	
	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;
	
	// スプライト描画
	void Draw2D();
	
	// ImGui
	void ImGuiDraw();
	
	// 移動
	void Move();
	
	// 攻撃
	void Attack();

private: // 衝突判定

	//void OnCollisionTrigger(const Collider* _other);
	
public: // ゲッター

	// デスフラグ
	bool IsDead() const { return isDead_; }

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	/*CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;*/

	// 弾
	std::list<PlayerBullet*> bullets_ = {};

	// 発射クールタイム
	const int kShootCoolDownFrame_ = 15;
	// 弾のクールタイム
	int countCoolDownFrame_ = 0;

	// 移動速度
	Vector3 moveVelocity_{};
	Vector3 moveSpeed_ = { 0.05f,0.0f,0.05f };

	// 死亡フラグ
	bool isDead_ = false;

};

