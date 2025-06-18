#pragma once

#include "../../BaseObject/GameObject.h"
#include "Bullet/EnemyBullet.h"
#include "../../../gameEngine/math/MyMath.h"
#include"../../Collider/ColliderManager.h"
#include "../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>
#include <Sprite.h>
#include <Framework.h>

class Enemy : public GameObject
{
public:

	Enemy() = default;
	~Enemy() = default;

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

	// 出現モーション
	void PopMotion();

	// 移動モーション
	void MoveMotion();

	// 攻撃モーション
	void AttackMotion();

	// 被弾モーション
	void HitMotion();

	// 死亡モーション
	void DeadMotion();

private: // 衝突判定

	// 衝突時の処理
	void OnCollisionTrigger(const Collider* _other);

	// 衝突中の処理
	void OnCollision(const Collider* _other);

	// 衝突時の押し出し処理
	void CorrectOverlap(const AABB _anyAABB);

public: // ゲッター

	// 死亡フラグ
	bool IsDead() const { return isDead_; }

	// プレイヤーとの距離
	Vector3 GetToPlayer() const { return toPlayer_; }

public: // セッター

	// プレイヤーの位置をセット
	void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

	// エネミーの位置をセット
	void SetEnemyPosition(Vector3 _enemyPosition) { position_ = _enemyPosition; }

public: // 構造体

	struct Motion
	{
		// モーションがアクティブかどうか
		bool isActive = false;
		// カウント
		uint32_t count = 0;
		// Maxカウント
		uint32_t maxCount = 30;

	};

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// 移動速度
	Vector3 moveVelocity_{};
	float moveSpeed_ = 0.05f;

	// プレイヤーの位置
	Vector3 playerPosition_{};
	Vector3 toPlayer_{};
	// 追尾停止距離
	const float kStopChasingDistance = 15.0f;

	// 出現モーション
	Motion popMotion_ = {false,0,30};
	// 移動モーション
	Motion moveMotion_ = { false,0,30 };
	// 攻撃モーション
	Motion attackMotion_ = { false,0,30 };
	// 被弾モーション
	Motion hitMotion_ = { false,0,30 };
	// 死亡モーション
	Motion deadMotion_ = { false,0,30 };

	// 死亡
	bool isDead_ = false;

	// 弾
	std::vector<std::unique_ptr<EnemyBullet>> pBullets_ = {};

	// 攻撃フラグ
	bool isAttack_ = false;
	// 攻撃クールタイム
	uint32_t attackCooldown_ = 0;

	// 壁に衝突したかどうか
	bool isWallCollision_ = false;
	
	// 衝突相手のaabb
	AABB collisionWallAABB_ = {};

};

