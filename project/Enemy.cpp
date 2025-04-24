#include "Enemy.h"

void Enemy::Initialize()
{
    // --- 3Dオブジェクト ---
    object_ = std::make_unique<Object3d>();
    object_->Initialize("cube.obj");

    moveVelocity_ = { 0.1f,0.1f,0.0f };

    object_->SetPosition(position_);
    object_->SetRotate(rotation_);

    // 仮置き
    scale_ = { 1.0f,1.0f,1.0f };
    object_->SetScale(scale_);

    /*collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Enemy";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
    collisionManager_->RegisterCollider(&collider_);*/

    // ステータス
    hp_ = 5;
    isDead_ = false;

}

void Enemy::Finalize()
{
    //collisionManager_->DeleteCollider(&collider_);
}

void Enemy::Update()
{
    object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);
    object_->Update();

    // 移動
	Move();

    // 攻撃
	Attack();

	// 敵同士の衝突判定
	//EnemyCollision();


    /*aabb_.min = position_ - object_->GetScale();
    aabb_.max = position_ + object_->GetScale();
    aabb_.max.y += 1.0f;
    collider_.SetPosition(position_);*/
}

void Enemy::Draw()
{
    object_->Draw();
}

void Enemy::Draw2D()
{
}

void Enemy::ImGuiDraw()
{
	ImGui::Begin("Enemy");

	ImGui::SliderFloat3("position", &position_.x, -30.0f, 30.0f);
	ImGui::SliderFloat3("rotation", &rotation_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("scale", &scale_.x, 0.0f, 10.0f);

    ImGui::End();
}

void Enemy::Move()
{
    // 敵弾から自キャラへのベクトルを計算
    toPlayer_ = playerPosition_ - position_;

    // ベクトルを正規化する
    toPlayer_ = Normalize(toPlayer_);
    moveVelocity_ = Normalize(moveVelocity_);
    // 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
    moveVelocity_ = 1.0f * (Slerp(moveVelocity_, toPlayer_, 0.1f));

    // 進行方向に見た目の回転を合わせる
    // Y軸周り角度(θy)
    rotation_.y = std::atan2(moveVelocity_.x, moveVelocity_.z);
    rotation_.x = 0.0f;
    

    moveVelocity_ /= 20.0f;


    moveVelocity_.y = 0.0f;
    position_ += moveVelocity_;
}

void Enemy::Attack()
{
}

void Enemy::EnemyCollision(Vector3 _position)
{
}
