#include "Enemy.h"

#include "ColliderManager.h"

void Enemy::Initialize()
{
	enemy_ = std::make_unique<Object3d>();
	enemy_->Initialize("cube.obj");
	position_ = { 3.0f,8.0f,0.0f };
	scale_ = { 0.8f, 0.8f,0.8f };
	enemy_->SetPosition(position_);
	enemy_->SetScale(scale_);

	// 衝突判定
	collisionManager_ = CollisionManager::GetInstance();
	
	objectName_ = "Enemy";
	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	//collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollisionTrigger, this, std::placeholders::_1));
	collider_.SetOnCollision(std::bind(&Enemy::OnCollisionTrigger, this, std::placeholders::_1));
	collisionManager_->RegisterCollider(&collider_);
}

void Enemy::Finalize()
{
	enemy_.reset();
}

void Enemy::Update()
{
	enemy_->Update();
	enemy_->SetPosition(position_);
	enemy_->SetRotate(rotation_);

	// 当たり判定関係
	aabb_.min = position_ - enemy_->GetScale();
	aabb_.max = position_ + enemy_->GetScale();
	collider_.SetPosition(position_);
	
	// 移動
	Move();
	

	// 場外処理
	OutOfField();
	
	// 攻撃
	Attack();
	
	position_ += moveVel_;
}

void Enemy::Draw()
{
	enemy_->Draw();
}

void Enemy::Move()
{
	// 敵から自キャラへのベクトルを計算
	toPlayer_ = playerPos_ - position_;

	// ベクトルを正規化する
	toPlayer_ = Normalize(toPlayer_);
	moveVel_ = Normalize(moveVel_);
	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	moveVel_ = 1.0f * (Slerp(moveVel_, toPlayer_, 0.1f));

	// 進行方向に見た目の回転を合わせる
	// Y軸周り角度(θy)
	
	rotation_.y = std::atan2(moveVel_.x, moveVel_.z);
	rotation_.x = 0.0f;
	

	moveVel_ /= 20.0f;

	moveVel_.y = 0.0f;
	position_ += moveVel_;
}

void Enemy::OutOfField()
{
	if (isGround_ == false)
	{
		position_.y -= fallSpeed_;
	}

	isGround_ = false;
}

void Enemy::Attack()
{
	//if()
}

void Enemy::ImGuiDraw()
{
	ImGui::Begin("Enemy");

	ImGui::SliderFloat3("EnemyPos", &position_.x, -20.0f, 20.0f);
	
	ImGui::SliderFloat3("EnemySpeed", &moveVel_.x, -5.0f, 5.0f);

	ImGui::Text("isGround_ : %s", isGround_ ? "true" : "false");

	if (ImGui::Button("ReSetPos"))
	{
		position_ = { 1.0f,3.0f,0.0f };
	}

	ImGui::End();
}

void Enemy::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "Field")
	{
		isGround_ = true;
	}
}
