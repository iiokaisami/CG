#include "Player.h"

#include "ColliderManager.h"

void Player::Initialize()
{
	player_ = std::make_unique<Object3d>();
	player_->Initialize("cube.obj");
	position_ = { 0.0f,3.0f,0.0f };
	scale_ = { 1.0f,1.0f,1.0f };
	player_->SetPosition(position_);
	player_->SetScale(scale_);

	// 衝突判定
	collisionManager_ = CollisionManager::GetInstance();

	objectName_ = "Player";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	//collider_.SetOnCollisionTrigger(std::bind(&Player::OnCollisionTrigger, this, std::placeholders::_1));
	collider_.SetOnCollision(std::bind(&Player::OnCollisionTrigger, this, std::placeholders::_1));
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

	moveVel_ = { 0.0f,0.0f,0.0f };

	// 当たり判定関係
	aabb_.min = position_ - player_->GetScale();
	aabb_.max = position_ + player_->GetScale();
	collider_.SetPosition(position_);

	// 移動
	Move();

	// 場外処理
	OutOfField();

	// 攻撃
	Attack();

	
	position_ += moveVel_;
}

void Player::Draw()
{
	player_->Draw();
}

void Player::Move()
{
	moveVel_ = { 0.0f,0.0f,0.0f };

	if (Input::GetInstance()->PushKey(DIK_W))
	{
		moveVel_.z = moveSpeed_.z;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		moveVel_.z = -moveSpeed_.z;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		moveVel_.x = -moveSpeed_.x;
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		moveVel_.x = moveSpeed_.x;
	}
}

void Player::OutOfField()
{
	if (isGround_ == false)
	{
		position_.y -= fallSpeed_;
	}

	isGround_ = false;
}

void Player::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		isAttack_ = true;
	}

	if (isAttack_)
	{
		moveVel_ *= 3.5f;

		attackTimeCounter_ -= 1.0f;
	}

	if (attackTimeCounter_ <= 0.0f)
	{
		isAttack_ = false;
		attackTimeCounter_ = attackTime_;
	}
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");

	ImGui::SliderFloat3("PlayerPos", &position_.x, -20.0f, 20.0f);

	ImGui::SliderFloat3("PlayerSpeed", &moveVel_.x, -5.0f, 5.0f);

	ImGui::Text("isGround_ : %s", isGround_ ? "true" : "false");

	if (ImGui::Button("ReSetPos"))
	{
		position_ = { 0.0f,3.0f,0.0f };
	}

	ImGui::End();
}

void Player::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "Field")
	{
		isGround_ = true;
	} 
}
