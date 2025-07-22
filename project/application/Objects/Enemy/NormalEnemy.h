#pragma once

#include "../../BaseObject/GameObject.h"
#include "Bullet/EnemyBullet.h"
#include "BehaviorState/NormalEmemyState/EnemyBehaviorState.h"
#include"../../../gameEngine/Collider/ColliderManager.h"
#include "../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>
#include <Sprite.h>
#include <Framework.h>

class NormalEnemy : public GameObject
{
public:

	NormalEnemy() = default;
	~NormalEnemy() = default;

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

	// 行動ステート切り替え
	void ChangeBehaviorState(std::unique_ptr<EnemyBehaviorState> _pState);

	// objectのtransformをセット
	void ObjectTransformSet(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale);

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

	// 被弾フラグ
	bool IsHit() const { return isHit_; }

	// プレイヤーとの距離が一定以上かどうか
	bool IsFarFromPlayer() const { return isFarFromPlayer_; }

public: // セッター

	// プレイヤーの位置をセット
	void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

	// エネミーの位置をセット
	void SetEnemyPosition(Vector3 _enemyPosition) { position_ = _enemyPosition; }

	// 無敵フラグをセット
	void SetIsInvincible(bool _isInvincible) { isInvincible_ = _isInvincible; }

	// 被弾フラグをセット
	void SetIsHit(bool _isHit) { isHit_ = _isHit; }

	// 死亡フラグをセット
	void SetIsDead(bool _isDead) { isDead_ = _isDead; }

	// オブジェクトのtransformをセット
	void SetObjectPosition(const Vector3& _position) { object_->SetPosition(_position); }
	void SetObjectRotation(const Vector3& _rotation) { object_->SetRotate(_rotation); }
	void SetObjectScale(const Vector3& _scale) { object_->SetScale(_scale); }

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


	// 死亡
	bool isDead_ = false;

	// 弾
	std::vector<std::unique_ptr<EnemyBullet>> pBullets_ = {};

	// 壁に衝突したかどうか
	bool isWallCollision_ = false;
	
	// 衝突相手のaabb
	AABB collisionWallAABB_ = {};

	// 行動ステート
	std::unique_ptr<EnemyBehaviorState> pBehaviorState_ = nullptr;

	// 無敵フラグ(出現時等攻撃を受けなくさせる)
	bool isInvincible_ = true;

	// 被弾フラグ
	bool isHit_ = false;

	// プレイヤーとの距離が一定以上かどうか
	bool isFarFromPlayer_ = false;

};

