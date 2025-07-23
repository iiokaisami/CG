#pragma once

#include "../../BaseObject/GameObject.h"
#include "Bullet/VignetteTrap.h"
#include "Bullet/TimeBomb.h"
#include "BehaviorState/TrapEnemyState/TrapEnemyBehaviorState.h"
#include"../../../gameEngine/Collider/ColliderManager.h"
#include "../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>
#include <Sprite.h>
#include <Framework.h>

class TrapEnemy : public GameObject
{
public:

	TrapEnemy() = default;
	~TrapEnemy() = default;
	
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

	// 罠設置
	void SetTrap(bool _isNextTrapTimeBomb);

	// 罠初期化
	void TrapInit(bool _isNextTrapTimeBomb);

	// 行動ステート切り替え
	void ChangeBehaviorState(std::unique_ptr<TrapEnemyBehaviorState> _pState);

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

	// デスフラグ
	bool IsDead() const { return isDead_; }
	// 被弾フラグ
	bool IsHit() const { return isHit_; }
	// 罠設置のクールタイム完了フラグ
	bool IsTrapCooldownComplete() const { return isTrapCooldownComplete_; }
	// プレイヤーとの距離が一定以上かどうか
	bool IsFarFromPlayer() const { return isFarFromPlayer_; }

public: // セッター

	Vector3 SetPlayerPosition(const Vector3& _playerPosition) { return playerPosition_ = _playerPosition; }

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

	// 罠設置完了フラグセット
	void SetIsTrapCooldownComplete(bool _isTrapCooldownComplete) { isTrapCooldownComplete_ = _isTrapCooldownComplete; }

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
	const float kTooCloseDistance = 3.0f;
	// 追尾開始距離
	const float kTooFarDistance = 20.0f;


	// 死亡
	bool isDead_ = false;

	// 罠
	std::vector<std::unique_ptr<TimeBomb>> pTimeBomb_ = {};
	std::vector<std::unique_ptr<VignetteTrap>> pVignetteTrap_ = {};
	
	// クールタイム完了フラグ
	bool isTrapCooldownComplete_ = false;

	// 壁に衝突したかどうか
	bool isWallCollision_ = false;

	// 衝突相手のaabb
	AABB collisionWallAABB_ = {};

	// 行動ステート
	std::unique_ptr<TrapEnemyBehaviorState> pBehaviorState_ = nullptr;

	// 離れる
	bool isEscape_ = false;
	// 近づく
	bool isApproach_ = false;
	// 罠を設置する
	bool isStopAndTrap_ = false;


	// 無敵フラグ(出現時等攻撃を受けなくさせる)
	bool isInvincible_ = true;

	// 被弾フラグ
	bool isHit_ = false;

	// プレイヤーとの距離が一定以上かどうか
	bool isFarFromPlayer_ = false;

};

