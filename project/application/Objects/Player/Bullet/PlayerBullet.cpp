#include "PlayerBullet.h"

void PlayerBullet::Initialize()
{
	// --- 3Dオブジェクト ---
  	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	
	// 仮置き
	scale_ = { 0.5f,0.5f,0.5f };
	object_->SetScale(scale_);

	// 当たり判定
	colliderManager_ = ColliderManager::GetInstance();

	objectName_ = "PlayerBullet";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&PlayerBullet::OnCollisionTrigger, this, std::placeholders::_1));
	colliderManager_->RegisterCollider(&collider_);

}

void PlayerBullet::Finalize()
{
	colliderManager_->DeleteCollider(&collider_);
}

void PlayerBullet::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);

	rotation_.y += 1.0f;
	position_ += velocity_;

	aabb_.min = position_ - object_->GetScale();
	aabb_.max = position_ + object_->GetScale();
	collider_.SetPosition(position_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	object_->Draw();
}

void PlayerBullet::Draw2D()
{
}

void PlayerBullet::ImGuiDraw()
{
	ImGui::Begin("PlayerBullet");
	
	ImGui::SliderFloat3("position", &position_.x, -30.0f, 30.0f);
	ImGui::SliderFloat3("rotation", &rotation_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("scale", &scale_.x, 0.0f, 10.0f);
	
	ImGui::End();
}

void PlayerBullet::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "Enemy")
	{
		isDead_ = true;
	}  
	if (_other->GetColliderID() == "EnemyBullet")
	{
		isDead_ = true;
	}
	if (_other->GetColliderID() == "Wall")
	{
		isDead_ = true;
	}
}

void PlayerBullet::RunSetMask()
{
	collider_.SetMask(colliderManager_->GetNewMask(collider_.GetColliderID(), "Player"));
}