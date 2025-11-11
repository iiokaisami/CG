#include "Corruptor.h"

// BehaviorState
#include "behaviorState/corruptorState/CorruptorBehaviorSpawn.h"
#include "behaviorState/corruptorState/CorruptorBehaviorMove.h"
#include "behaviorState/corruptorState/CorruptorBehaviorSelfDestruct.h"

void Corruptor::Initialize()
{
	// 3Dオブジェクト生成
	object_ = std::make_unique<Object3d>();
	object_->Initialize("bomb.obj");

	moveVelocity_ = { 0.1f,0.1f,0.1f };

	object_->SetRotate(rotation_);
	scale_ = { 1.0f,1.0f,1.0f };
	object_->SetScale(scale_);

	// コライダー設定
	colliderManager_ = ColliderManager::GetInstance();

	objectName_ = "Corruptor";

	desc = 
	{
		.owner = this,
		.colliderID = objectName_,
		.shape = Shape::AABB,
		.shapeData = &aabb_,
		.attribute = colliderManager_->GetNewAttribute(objectName_),
		.onCollisionTrigger = std::bind(&Corruptor::OnCollisionTrigger, this, std::placeholders::_1),
	};
	collider_.MakeAABBDesc(desc);
	colliderManager_->RegisterCollider(&collider_);

	// ステータス
	isDead_ = false;

	// 行動ステート
	pBehaviorState_ = std::make_unique<CorruptorBehaviorSpawn>(this);

	// 出現時は無敵状態
	isInvincible_ = true;

}

void Corruptor::Finalize()
{
	colliderManager_->DeleteCollider(&collider_);
}

void Corruptor::Update()
{
	// アクティブフラグ
	isActive_ = !isInvincible_;

	// 行動ステート更新
	pBehaviorState_->Update();

	object_->Update();

	// プレイヤーとの距離更新
	float distanceToPlayer = position_.Distance(playerPosition_);

	// プレイヤーとの距離が一定以上かどうかのフラグ更新
	isFarFromPlayer_ = (distanceToPlayer > 2.0f);

	// aabbの更新
	aabb_.min = position_ - object_->GetScale();
	aabb_.max = position_ + object_->GetScale();
	aabb_.max.y += 1.0f;
	collider_.SetPosition(position_);

}

void Corruptor::Draw()
{
	object_->Draw();
}

void Corruptor::Draw2D()
{
}

void Corruptor::ImGuiDraw()
{
	ImGui::Begin("Corruptor");
	ImGui::Text("Position: (%.2f, %.2f, %.2f)", position_.x, position_.y, position_.z);
	ImGui::Text("Rotation: (%.2f, %.2f, %.2f)", rotation_.x, rotation_.y, rotation_.z);
	ImGui::Text("Scale: (%.2f, %.2f, %.2f)", scale_.x, scale_.y, scale_.z);
	ImGui::Text("HP: %d", hp_);
	ImGui::Text("Is Dead: %s", isDead_ ? "Yes" : "No");
	ImGui::End();
}

void Corruptor::Move()
{
	// プレイヤー方向ベクトル計算
	toPlayer_ = playerPosition_ - position_;
	Vector3 direction = Normalize(toPlayer_);
	moveVelocity_ = Normalize(moveVelocity_);

	// 近づく
	moveVelocity_ = Slerp(moveVelocity_, direction, 0.1f);

	// Y軸回転を進行方向に合わせる
	rotation_.y = std::atan2(moveVelocity_.x, moveVelocity_.z);
	rotation_.x = 0.0f;

	moveVelocity_ /= 30.0f;
	moveVelocity_.y = 0.0f;
	position_ += moveVelocity_;

	ObjectTransformSet(position_, rotation_, scale_);

}

void Corruptor::ChangeBehaviorState(std::unique_ptr<CorruptorBehaviorState> _pState)
{
	pBehaviorState_ = std::move(_pState);
	pBehaviorState_->Initialize();
}

void Corruptor::ObjectTransformSet(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale)
{
	object_->SetPosition(_position);
	object_->SetRotate(_rotation);
	object_->SetScale(_scale);
}

void Corruptor::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "PlayerBullet" && !isInvincible_)
	{
		// プレイヤーの弾と衝突した場合
		if (hp_ > 1)
		{
			// HP減少
			hp_--;

			isHit_ = true;
		}
	}
}