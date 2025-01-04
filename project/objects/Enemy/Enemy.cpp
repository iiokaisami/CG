#include "Enemy.h"

#include <ModelManager.h>

void Enemy::Initialize()
{
    // --- 3Dオブジェクト ---
    ModelManager::GetInstance()->LoadModel("cube.obj");

    object_ = std::make_unique<Object3d>();
    object_->Initialize("cube.obj");

    position_ = { 0.0f,0.0f,10.0f };
    object_->SetPosition(position_);

    moveVelocity_ = { 0.1f,0.1f,0.0f };
    object_->SetRotate(rotation_);

    // 仮置き
    object_->SetScale({ 0.5f,1.0f,0.5f });
}

void Enemy::Finalize()
{
    // 各解放処理


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
