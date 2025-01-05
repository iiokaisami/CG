#include "Enemy.h"

#include <ModelManager.h>

void Enemy::Initialize()
{
    // --- 3Dオブジェクト ---
    ModelManager::GetInstance()->LoadModel("cube.obj");

    object_ = std::make_unique<Object3d>();
    object_->Initialize("cube.obj");

    moveVelocity_ = { 0.1f,0.1f,0.0f };
    object_->SetRotate(rotation_);

    // 仮置き
    object_->SetScale({ 0.5f,1.0f,0.5f });

    collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Enemy";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollision, this));
    collisionManager_->RegisterCollider(&collider_);
}

void Enemy::Finalize()
{
    // 各解放処理

    collisionManager_->DeleteCollider(&collider_);
}

void Enemy::Update()
{
    object_->Update();

    // 敵弾から自キャラへのベクトルを計算
    Vector3 toPlayer = playerPosition_ - position_;

    // ベクトルを正規化する
    toPlayer = Normalize(toPlayer);
    moveVelocity_ = Normalize(moveVelocity_);
    // 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
    moveVelocity_ = 1.0f * (Slerp(moveVelocity_, toPlayer, 0.1f));


    // 進行方向に見た目の回転を合わせる
    // Y軸周り角度(θy)
    rotation_.y = std::atan2(moveVelocity_.x, moveVelocity_.z);

    // 座標を移動させる(フレーム分の移動量を足しこむ)
    moveVelocity_ /= 20.0f;
    position_ += moveVelocity_;

    // モデルに座標をセット
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);

    aabb_.min = position_ - object_->GetScale();
    aabb_.max = position_ + object_->GetScale();
    aabb_.max.y += 1.0f;
    collider_.SetPosition(position_);
}

void Enemy::Draw()
{
    object_->Draw();
}

void Enemy::Draw2d()
{
}

void Enemy::Attack()
{
}

void Enemy::OnCollision()
{
   /* if (hp_ > 0)
    {
        hp_ -= 1;
    }
    else
    {
        if (isDead_ == false)
        {
            isBossDeadMoment_ = true;
        }
        isDead_ = true;
    }*/
}
