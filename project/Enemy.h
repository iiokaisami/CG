#pragma once

#include "GameObject.h"
#include "gameEngine/math/MyMath.h"

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

private: // 衝突判定

	//void OnCollisionTrigger(const Collider* _other);

	// 敵同士が衝突したときの処理
	void EnemyCollision(Vector3 _position);

public: // ゲッター

	bool IsDead() const { return isDead_; }

public: // セッター

	// プレイヤーの位置をセット
	void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

	// エネミーの位置をセット
	void SetEnemyPosition(Vector3 _enemyPosition) { position_ = _enemyPosition; }

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	/*CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;*/

	// 移動速度
	Vector3 moveVelocity_{};
	float moveSpeed_ = 0.05f;

	// プレイヤーの位置
	Vector3 playerPosition_{};
	Vector3 toPlayer_{};

	// 死亡
	bool isDead_ = false;

	

};

