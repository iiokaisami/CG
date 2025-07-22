#pragma once

#include "../../BaseObject/GameObject.h"
// ここに弾的なやつ
#include "BehaviorState/NormalEmemyState/EnemyBehaviorState.h"
#include"../../../gameEngine/Collider/ColliderManager.h"

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
	void SetTrap();

	// objectのtransformをセット
	void ObjectTransformSet(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale);

private: // 衝突判定

	// 衝突時の処理
	void OnCollisionTrigger(const Collider* _other);

	// 衝突中の処理
	void OnCollision(const Collider* _other);

	// 衝突時の押し出し処理
	void CorrectOverlap(const AABB _anyAABB);

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
	// 追尾停止・開始距離
	const float kTooCloseDistance = 15.0f;
	const float kTooFarDistance = 30.0f;


	// 死亡
	bool isDead_ = false;

	// 罠
	
	// 罠設置のクールタイム	
	float trapCooldown_ = 0.0f;
	// 罠設置のクールタイムの最大値
	const float kMaxTrapCooldown = 60.0f * 4;


	// 壁に衝突したかどうか
	bool isWallCollision_ = false;

	// 衝突相手のaabb
	AABB collisionWallAABB_ = {};

	// 行動ステート


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

