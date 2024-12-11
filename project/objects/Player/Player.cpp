#include "Player.h"

#include <ModelManager.h>

#include "Input.h"

void Player::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("plane.obj");

    object_ = std::make_unique<Object3d>();
    object_->Initialize("plane.obj");

    position_ = { 0.0f,0.0f,0.0f };
    object_->SetPosition(position_);

    // 仮置き
    object_->SetScale({ 0.2f,0.2f,0.2f });
}

void Player::Finalize()
{
    // 各解放処理

    for (auto& bullet : bullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
    }

    bullets_.remove_if([](PlayerBullet* bullet) {
        if (bullet->IsDead()) {
            bullet->Finalize();
            delete bullet;
            return true;
        }
        return false;
        });
}

void Player::Update()
{

    object_->Update();

    //デスフラグの立った弾を削除
    bullets_.remove_if([](PlayerBullet* bullet) {
        if (bullet->IsDead()) {

            bullet->Finalize();
            delete bullet;

            return true;
        }
        return false;
        });

    moveVelocity_ = {};

    // 移動処理
    if (Input::GetInstance()->PushKey(DIK_W))
    {
        moveVelocity_.z += moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_S))
    {
        moveVelocity_.z += -moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_A))
    {
        moveVelocity_.x += -moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_D))
    {
        moveVelocity_.x += moveSpeed_;
    }

    position_ += moveVelocity_;

    // モデルに座標をセット
    object_->SetPosition(position_);

    CameraFollow();

    CalcCursorMove();

    Attack();

    // 弾更新
    for (auto& bullet : bullets_) {
        bullet->Update();
    }

}

void Player::Draw()
{
    object_->Draw();

    // 弾描画
    for (auto& bullet : bullets_)
    {
        bullet->Draw();
    }
}

void Player::Draw2d()
{
}

void Player::Attack()
{
    if (Input::GetInstance()->PushKey(DIK_SPACE))
    {
        /// プレイヤーの向きに合わせて弾の速度を変更
        Vector3 bulletVelocity = {
            std::cosf(rotation_.x) * std::sinf(rotation_.y),    // x
            std::sinf(-rotation_.x),                             // y
            std::cosf(rotation_.x) * std::cosf(rotation_.y)     // z
        };

        if (countCoolDownFrame_ <= 0)
        {
            // 弾を生成し、初期化
            PlayerBullet* newBullet = new PlayerBullet();

            newBullet->SetPosition(position_);
            newBullet->Initialize();
            newBullet->SetVelocity(bulletVelocity);

            newBullet->RunSetMask();
           // collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "PlayerBullet"));

            // 弾を登録する
            bullets_.push_back(newBullet);

            countCoolDownFrame_ = kShootCoolDownFrame_;
        }
    }
    countCoolDownFrame_--;
}

void Player::CameraFollow()
{
    Matrix4x4 playerWorld = Matrix4x4::AffineMatrix(scale_, rotation_, position_);
    Matrix4x4 cameraLocal = Matrix4x4::AffineMatrix({ 1.0f,1.0f,1.0f }, {}, cameraDistance_);

    Vector3 camPos_target = Transform(cameraDistance_, playerWorld);
    Vector3 camPos_current = camera_->GetPosition();

    cameraRotate_.x = rotation_.x;
    cameraRotate_.y = rotation_.y;

    camera_->SetRotate(cameraRotate_);

    Vector3 camPos_nextFrame = {};
    camPos_nextFrame.Lerp(camPos_current, camPos_target, camFollowMultiply_);
    camera_->SetPosition(camPos_nextFrame);
}

void Player::CalcCursorMove()
{
    POINT mousePosCurrent;
    // マウス座標取得
    GetCursorPos(&mousePosCurrent);
    mousePosDiff_ =
    {
        static_cast<float>(960 - mousePosCurrent.x),
        static_cast<float>(540 - mousePosCurrent.y)
    };
}
