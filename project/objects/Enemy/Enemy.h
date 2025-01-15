#pragma once

#include "../GameObject/GameObject.h"
#include "../math/MyMath.h"
#include "../../collider/Collider.h"
#include "../../collider/CollisionManager.h"
#include "../HPBar/HPBar.h"

#include <Object3d.h>
#include <Sprite.h>

class Enemy : public GameObject
{
public:

	Enemy() = default;
	~Enemy() = default;

    // 初期化
    void Initialize() override;

    // 終了
    void Finalize() override;

    // 更新処理
    void Update() override;

    // 描画処理
    void Draw() override;

    void Draw2d();

	void ImGuiDraw();

    // 攻撃
    void Attack();

	// 敵同士が衝突したときの処理
	void EnemyCollision(Vector3 _position);

private: // 衝突判定

    void OnCollision(const Collider* _other);

public: // ゲッター
    
	bool IsDead() const { return isDead_; }

public: // セッター

    void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

	void SetPosition(Vector3 _position) { position_ = _position; }

private:

    // 3Dオブジェクト
    std::unique_ptr<Object3d> object_ = nullptr;

	std::unique_ptr<HPBar> hpBar_ = nullptr;

    CollisionManager* collisionManager_ = nullptr;
    Collider collider_;
    AABB aabb_;

    // 移動速度
    Vector3 moveVelocity_{};
    float moveSpeed_ = 0.05f;
    
	// プレイヤーの位置
    Vector3 playerPosition_{};
	Vector3 toPlayer_{};
	
    // 死亡
	bool isDead_ = false;
	bool isDeadMotion_ = false;
	Vector3 deadScale_ = { 0.0f,0.0f,0.0f };

    // 出現
	bool isPopMotion_ = true;
	Vector3 popScale_ = { 0.5f,0.8f,0.5f };
};

