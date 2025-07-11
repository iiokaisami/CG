#pragma once

#include "../../../BaseObject/GameObject.h"
#include "../../../Collider/ColliderManager.h"
#include "../../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>
#include <Sprite.h>

class PlayerBullet : public GameObject
{
public:

	PlayerBullet() = default;
	~PlayerBullet() = default;
	
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

	Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

public: // セッター

	bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// 速度
	Vector3 velocity_{};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;

};