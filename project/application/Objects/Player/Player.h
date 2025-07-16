#pragma once

#include "../../BaseObject/GameObject.h"
#include "Bullet/PlayerBullet.h"
#include"../../Collider/ColliderManager.h"

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

	// 回避
	void Evade();


private: // 衝突判定

	// 衝突時の処理
	void OnCollisionTrigger(const Collider* _other);
	
	// 衝突中の処理
	void OnCollision(const Collider* _other);

	// 壁に衝突したときの処理
	void CorrectOverlap();

public: // ゲッター

	// デスフラグ
	bool IsDead() const { return isDead_; }

	bool IsHitMoment() const { return isHitMoment_; }

	Vector3 GetVelocity() const { return moveVelocity_; }

public: // セッター

	void SetHitMoment(bool _isHitMoment) { isHitMoment_ = _isHitMoment; }

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// 弾
	std::list<PlayerBullet*> pBullets_ = {};

	// 発射クールタイム
	const int kShootCoolDownFrame_ = 15;
	// 弾のクールタイム
	int countCoolDownFrame_ = 0;

	// 移動速度
	Vector3 moveVelocity_{};
	Vector3 moveSpeed_ = { 0.1f,0.0f,0.1f };

	// 死亡フラグ
	bool isDead_ = false;

	// ヒットした瞬間のフラグ
	bool isHitMoment_ = false;

	// 壁に衝突したかどうか
	bool isWallCollision_ = false;

	// 衝突相手のAABB
	AABB collisionWallAABB_;

	// 回避フラグ
	bool isEvading_ = false;
	// 回避時間
	int evadeTime_ = 0;
	// 回避時間の最大値
	const int kEvadeTimeMax_ = 30;
	// 回避速度
	Vector3 evadeSpeed_ = { 0.2f,0.0f,0.2f };
	// 回避方向
	Vector3 evadeDirection_ = { 0.0f,0.0f,0.0f };
	// 回避中のフレーム数
	int evadeFrame_ = 0;
	const int kEvadeDuration_ = 30; // 回避の持続時間
	// 回避速度
	const float kEvadeSpeed_ = 0.2f;

	float evadeStartRotationX_ = 0.0f; // 回避開始時のx軸角度
	float evadeTargetRotationX_ = 0.0f; // 回避中の目標x軸角度
	const float kEvadeRotateAngle_ = 3.14f * 4.0f; // 1回転(360度) 

};

