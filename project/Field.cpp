#include "Field.h"

#include "ColliderManager.h"

void Field::Initialize()
{
	field_ = std::make_unique<Object3d>();
	field_->Initialize("cube.obj");
	position_ = { 0.0f,-0.5f,0.0f };
	scale_ = { 1.0f,1.0f,1.0f };
	field_->SetPosition(position_);
	field_->SetScale(scale_);

	// 衝突判定
	collisionManager_ = CollisionManager::GetInstance();

	objectName_ = "Field";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&Field::OnCollisionTrigger, this, std::placeholders::_1));
	collisionManager_->RegisterCollider(&collider_);

}

void Field::Finalize()
{
	field_.reset();
}

void Field::Update()
{
	field_->Update();
	field_->SetPosition(position_);
	field_->SetScale(scale_);

	aabb_.min = position_ - field_->GetScale();
	aabb_.max = position_ + field_->GetScale();
	collider_.SetPosition(position_);

}

void Field::Draw()
{
	field_->Draw();
}

void Field::ImGuiDraw()
{
	ImGui::Begin("Field");

	ImGui::SliderFloat3("FieldSize", &scale_.x, -20.0f, 20.0f);
	
	ImGui::End();
}

void Field::OnCollisionTrigger(const Collider* _other)
{
	_other;
	// 何もしない
}
