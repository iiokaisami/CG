#include "Player.h"

#include "ColliderManager.h"

void Player::Initialize()
{
	player_ = std::make_unique<Object3d>();
	player_->Initialize("cube.obj");
	player_->SetPosition(position_);
	player_->SetScale({ 1.0f,1.0f,1.0f });

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

}

void Player::Finalize()
{
	player_.reset();
}

void Player::Update()
{
	player_->Update();
	player_->SetPosition(position_);

	Move();
}

void Player::Draw()
{
	player_->Draw();
}

void Player::Move()
{
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		position_.z -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		position_.z += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		position_.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		position_.x += 0.1f;
	}
}
