#pragma once

#include "../../../BaseObject/GameObject.h"
#include"../../../../gameEngine/Collider/ColliderManager.h"

#include <Object3d.h>

class VignetteTrap : public GameObject
{
public:

	VignetteTrap() = default;
	~VignetteTrap() = default;
	
	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;
	
	// ImGui
	void ImGuiDraw();
	
	// モデル更新
	void UpdateModel();

	// 放物線上に発射
	void LaunchTrap();

private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);

public: // ゲッター

	bool IsDead() const { return isDead_; }

public: // セッター

	bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

	Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

	// プレイヤーの周囲の座標を設定
	void SetTrapLandingPosition(const Vector3& _playerPosition);

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;
	
	// 寿命
	//static const uint32_t kLifeTime = 120; // 2秒 いらない？

	// 死亡フラグ
	bool isDead_ = false;

	// 着弾地点
	Vector3 landingPosition_{};
	// 速度
	Vector3 velocity_{};
	// 発射中フラグ
	bool isLaunchingTrap_ = false;
	// 着弾までの時間
	float flightTime_ = 1.0f;
	
};

