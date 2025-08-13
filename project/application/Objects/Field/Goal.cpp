#include "Goal.h"

void Goal::Initialize()
{
	// --- 3Dオブジェクト ---
	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");
	position_ = { 0.0f,0.0f,-3.5f };
	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

	// 仮置き
	scale_ = { 0.0f,0.0f,0.0f };
	object_->SetScale(scale_);

	// 当たり判定
	colliderManager_ = ColliderManager::GetInstance();

	objectName_ = "Goal";
	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&Goal::OnCollisionTrigger, this, std::placeholders::_1));
	colliderManager_->RegisterCollider(&collider_);
}

void Goal::Finalize()
{
	colliderManager_->DeleteCollider(&collider_);
}

void Goal::Update()
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

	// ゴール出現処理
	if (isGoalVisible_)
	{
		scale_ = { 2.5f,3.0f,2.0f };
	}
}

void Goal::Draw()
{
	if (isGoalVisible_)
	{
		object_->Draw();
	}
}

void Goal::ImGuiDraw()
{
}

void Goal::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "Player" && isGoalVisible_)
	{
		// プレイヤーがゴールに到達した場合クリア
		isCleared_ = true;
	}
}
