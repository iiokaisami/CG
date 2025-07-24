#pragma once

#include "../../../BaseObject/GameObject.h"
#include"../../../../gameEngine/Collider/ColliderManager.h"
#include "../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>

class TimeBomb : public GameObject
{
public:

	TimeBomb() = default;
	~TimeBomb() = default;

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

private:

	// 衝突処理
	void OnSetCollisionTrigger(const Collider* _other);
	void OnExplosionTrigger(const Collider* _other);


	// 爆発処理
	void Explode();

public: // ゲッター

public: // セッター

	Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

	// プレイヤーの周囲の座標を設定
	void SetTrapLandingPosition(const Vector3& _playerPosition);

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	// 設置判定
	Collider setCollider_;
	AABB setAABB_;
	// 爆発判定
	std::string explosionObjectName_;
	Collider explosionCollider_;
	AABB explosionAABB_;


	// 爆発フラグ
	bool isExploded_ = false;
	// 初期スケールと目標スケール
	Vector3 startScale = { 1.0f, 1.0f, 1.0f };
	Vector3 endScale = { 2.0f, 2.0f, 2.0f };
	// 経過時間とスケール
	float elapsedTime = 0.0f;
	float duration = 1.0f; // 1秒間
	Vector3 currentScale;

	// 相手のHP
	float anyHP_ = 0.0f;

	// 着弾地点
	Vector3 landingPosition_{};
	// 速度
	Vector3 velocity_{};
	// 発射中フラグ
	bool isLaunchingTrap_ = false; 
	// 着弾までの時間
	float flightTime_ = 1.0f;

};

