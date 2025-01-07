#include "Player.h"

#include <ModelManager.h>

#include "../collider/CollisionManager.h"
#include "Input.h"
using namespace std;

void Player::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("player/Player.obj");

    object_ = std::make_unique<Object3d>();
    object_->Initialize("Player.obj");

    position_ = { 0.0f,0.5f,0.0f };
    object_->SetPosition(position_);

    // 仮置き
    object_->SetScale({ 0.2f,0.2f,0.2f });

	// 衝突判定
    collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Player";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Player::OnCollisionTrigger, this, std::placeholders::_1));
    collisionManager_->RegisterCollider(&collider_);

    hp_ = 5;
    isDead_ = false;

	hpBar_ = std::make_unique<HPBar>();
    hpBar_->LoadBar("HPBar.obj", { position_.x,position_.y  + 0.3f ,position_.z });
	hpBar_->Initialize();
    hpBar_->SetScale({ 0.3f,0.05f,0.05f });
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

    collisionManager_->DeleteCollider(&collider_);
}

void Player::Update()
{
	hpBar_->Update();
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


    Vector3 playerForward = { std::sinf(rotation_.y), 0.f, std::cosf(rotation_.y) };
    Vector3 playerRight = { std::cosf(rotation_.y), 0.f, -std::sinf(rotation_.y) };

    moveVelocity_ = {};

    // 移動処理
    if (Input::GetInstance()->PushKey(DIK_W))
    {
        moveVelocity_ += playerForward * moveSpeed_;
       // moveVelocity_.z += moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_S))
    {
        moveVelocity_ += -playerForward * moveSpeed_;
        //moveVelocity_.z += -moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_A))
    {
        moveVelocity_ += -playerRight * moveSpeed_;
        //moveVelocity_.x += -moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_D))
    {
        moveVelocity_ += playerRight * moveSpeed_;
        //moveVelocity_.x += moveSpeed_;
    }

    position_ += moveVelocity_;

    // 移動制限
    if (position_.x <= 0)
    {
        position_.x = max(position_.x, -290.0f);
    }
    else if (position_.x >= 0)
    {
        position_.x = min(position_.x, 290.0f);
    }
    if (position_.z <= 0)
    {
        position_.z = max(position_.z, -290.0f);
    }
    else if (position_.z >= 0)
    {
        position_.z = min(position_.z, 290.0f);
    }

    // モデルに座標をセット
    object_->SetPosition(position_);
    object_->SetRotate(rotation_);

	// hpBarの位置をセット
	hpBar_->SetRatio((hp_ + 0.1f) / 5.0f);
    hpBar_->SetPosition({ position_.x,position_.y + 0.8f ,position_.z });
    hpBar_->SetRotation(rotation_);

    // マウス移動
    //rotation_.x -= mousePosDiff_.y * 0.001f;
    //rotation_.y -= mousePosDiff_.x * 0.001f;
    if (rotation_.x > 1.57f) rotation_.x = 1.57f;
    if (rotation_.x < -1.57f) rotation_.x = -1.57f;

    
    if (Input::GetInstance()->PushKey(DIK_RIGHT))
    {
        //rotation_.y -= mousePosDiff_.x * 0.00005f;
		rotation_.y += rotateSpeed_.y;
    }
    if (Input::GetInstance()->PushKey(DIK_LEFT))
    {
       //rotation_.y += mousePosDiff_.x * 0.00005f;
		rotation_.y -= rotateSpeed_.y;
    }

    CameraFollow();
    camera_->Update();

    //CalcCursorMove();


    Attack();

	if (hitInterval_ > 0)
	{
		hitInterval_--;
	}

    aabb_.min = position_ - object_->GetScale();
    aabb_.max = position_ + object_->GetScale();
    collider_.SetPosition(position_);

    // 弾更新
    for (auto& bullet : bullets_) {
        bullet->Update();
    }

}

void Player::Draw()
{
    if (!((hitInterval_ <= 29 && hitInterval_ >= 20) or ( hitInterval_ <= 9 && hitInterval_ >= 1 )))
    {
        object_->Draw();
    }

    // 弾描画
    for (auto& bullet : bullets_)
    {
        bullet->Draw();
    }

	hpBar_->Draw();
}

void Player::Draw2d()
{
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");
	
    ImGui::SliderFloat3("pos", &position_.x, -290.0f, 290.0f);

	ImGui::End();
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
            collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "PlayerBullet"));

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

void Player::OnCollisionTrigger(const Collider* _other)
{
    _other;
    if (/*_other->GetColliderID() != "BossMoon" && !isHit_ &&*/ hp_ > 0 && hitInterval_ == 0)
    {
        hp_ -= 1;
		hitInterval_ = 30;
    }
    if (hp_ <= 0)
    {
        isDead_ = true;
    }
}
