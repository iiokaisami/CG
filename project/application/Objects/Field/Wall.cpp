#include "Wall.h"

void Wall::Initialize()
{
	// --- 3Dオブジェクト ---
	object_ = std::make_unique<Object3d>();
	object_->Initialize("wall.obj");
	position_ = { 0.0f,0.0f,-0.5f };
	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	
	// 仮置き
	scale_ = { 1.5f,3.0f,1.0f };
	object_->SetScale(scale_);
	
	// 当たり判定
	colliderManager_ = ColliderManager::GetInstance();

	objectName_ = "Wall";
	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&Wall::OnCollisionTrigger, this));
	colliderManager_->RegisterCollider(&collider_);
}

void Wall::Finalize()
{
	colliderManager_->DeleteCollider(&collider_);
}

void Wall::Update()
{
	// オブジェクトの更新
	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);
	object_->Update();

	// 当たり判定更新
	aabb_.min = position_ - object_->GetScale();
	aabb_.max = position_ + object_->GetScale();
	aabb_.max.y += 1.0f; // 見た目よりも縦に大きく
	collider_.SetPosition(position_);
}

void Wall::Draw()
{
	object_->Draw();
}

void Wall::ImGuiDraw()
{
	ImGui::Begin("Wall");

	ImGui::SliderFloat3("pos", &position_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("rot", &rotation_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("scale", &scale_.x, 0.0f, 10.0f);
	
	ImGui::End();
}

void Wall::OnCollisionTrigger()
{
	// とりあえず何もしない
}
