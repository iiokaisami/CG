#pragma once

#include "../application/BaseObject/GameObject.h"
#include"../../application/Collider/ColliderManager.h"

#include <Object3d.h>
#include <Sprite.h>

class EnemyBullet : public GameObject
{
public:
	EnemyBullet() = default;
	~EnemyBullet() = default;

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

private: // 衝突判定
	
	void OnCollisionTrigger(const Collider* _other);

public: // ゲッター

	bool IsDead() const { return isDead_; }
	
	void RunSetMask();
	
public: // セッター

	bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

	Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;
	
	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;
	
	// 速度
	Vector3 velocity_{};
	
	// 寿命
	static const uint32_t kLifeTime = 60 * 3;

	// デスタイマー
	uint32_t deathTimer_ = kLifeTime;

	// デスフラグ
	bool isDead_ = false;

};

