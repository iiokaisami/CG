#pragma once

#include "../GameObject/GameObject.h"
#include "./Bullet/PlayerBullet.h"
#include "../HPBar/HPBar.h"

#include "../Collider/Collider.h"

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

    // 更新処理
    void Update() override;

    // 描画処理
    void Draw() override;

    void Draw2d();

	void ImGuiDraw();

    // 攻撃
    void Attack();

    // 追尾カメラ
    void CameraFollow();

    // マウス視点
    void CalcCursorMove();

private: // 衝突判定

    void OnCollisionTrigger(const Collider* _other);

public: // セッター

    void SetCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }

public: // ゲッター

    Vector3 GetPosition() { return position_; }

	bool IsDead() const { return isDead_; }

private:

    std::shared_ptr<Camera> camera_ = nullptr;

    // 3Dオブジェクト
    std::unique_ptr<Object3d> object_ = nullptr;

	std::unique_ptr<HPBar> hpBar_ = nullptr;

    // 弾
    std::list<PlayerBullet*>    bullets_ = {};

    // 移動速度
    Vector3 moveVelocity_ = {};
    float moveSpeed_ = 0.08f;

    // 追尾カメラ
    Vector3 cameraRotate_ = { 0.3f, 0.0f, 0.0f };
    Vector3 cameraDistance_ = { 0.0f, 0.75f, -5.5f };
    float camFollowMultiply_ = 0.8f;

    // マウス視点
    Vector2 mousePosDiff_;
    bool cursorVisible_ = true;
    bool cursorLock_ = false;

	Vector2 rotateSpeed_ = { 0.015f, 0.015f };

    // 発射クールタイム
    const int kShootCoolDownFrame_ = 15;
    // 弾のクールタイム
    int countCoolDownFrame_ = 0; 

    // 当たり判定関係
    CollisionManager* collisionManager_ = nullptr;
    Collider collider_;
    AABB aabb_;

	
	bool isHit_ = false;
    uint32_t hitInterval_ = 0;
	bool isDead_ = false;
};

