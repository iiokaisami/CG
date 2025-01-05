#pragma once

#include "../GameObject/GameObject.h"
#include "./Bullet/PlayerBullet.h"

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

    // 攻撃
    void Attack();

    // 追尾カメラ
    void CameraFollow();

    // マウス視点
    void CalcCursorMove();

public: // セッター

    void SetCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }

public: // ゲッター

    Vector3 GetPosition() { return position_; }

private:

    std::shared_ptr<Camera> camera_ = nullptr;

    // 3Dオブジェクト
    std::unique_ptr<Object3d> object_ = nullptr;

    // 弾
    std::list<PlayerBullet*>    bullets_ = {};

    // 移動速度
    Vector3 moveVelocity_ = {};
    float moveSpeed_ = 0.05f;

    // 追尾カメラ
    Vector3 cameraRotate_ = { 0.3f, 0.0f, 0.0f };
    Vector3 cameraDistance_ = { 0.0f, 0.75f, -5.5f };
    float camFollowMultiply_ = 0.8f;

    // マウス視点
    Vector2 mousePosDiff_;
    bool cursorVisible_ = true;
    bool cursorLock_ = false;

    // 発射クールタイム
    const int                   kShootCoolDownFrame_ = 15;
    // 弾のクールタイム
    int                         countCoolDownFrame_ = 0; 
};

