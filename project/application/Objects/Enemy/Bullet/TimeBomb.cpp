#include "TimeBomb.h"

#include <numbers>

void TimeBomb::Initialize()
{
	// --- 3Dオブジェクト ---
	object_ = std::make_unique<Object3d>();
	object_->Initialize("bomb.obj");

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	
	// 仮置き
	scale_ = { 0.7f, 0.7f, 0.7f };
	object_->SetScale(scale_);
	
	// 当たり判定
	colliderManager_ = ColliderManager::GetInstance();
	objectName_ = "TimeBomb";
	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&TimeBomb::OnCollisionTrigger, this, std::placeholders::_1));
	colliderManager_->RegisterCollider(&collider_);
}

void TimeBomb::Finalize()
{
	colliderManager_->DeleteCollider(&collider_);
}

void TimeBomb::Update()
{
	// 物理挙動（放物線運動）
	if (isLaunchingTrap_) 
	{
		// 重力加速度
		const float gravity = -9.8f;
		// 1フレームの時間（例: 1/60秒）
		const float deltaTime = 1.0f / 60.0f;

		// 速度に重力を加算
		velocity_.y += gravity * deltaTime;

		// 位置を速度で更新
		position_ += velocity_ * deltaTime;
	}

	if ((position_ - landingPosition_).Length() < 0.1f)
	{
		isLaunchingTrap_ = false;
	}

	UpdateModel();
	
	rotation_ += {0.1f, 0.1f, 0.0f};
	
	aabb_.min = position_ - object_->GetScale();
	aabb_.max = position_ + object_->GetScale();
	collider_.SetPosition(position_);
	
}

void TimeBomb::Draw()
{
	object_->Draw();
}

void TimeBomb::ImGuiDraw()
{
}

void TimeBomb::UpdateModel()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);
}

void TimeBomb::LaunchTrap()
{
	isLaunchingTrap_ = true;

	// 重力加速度
	const float gravity = -9.8f;

	// XYの差分
	Vector3 diff = landingPosition_ - position_;

	// XZ方向の初速度
	velocity_.x = diff.x / flightTime_;
	velocity_.z = diff.z / flightTime_;

	// Y方向の初速度（放物線のために加速度を考慮）
	velocity_.y = (diff.y - 0.5f * gravity * flightTime_ * flightTime_) / flightTime_;
}

void TimeBomb::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "PlayerBullet")
	{
		// プレイヤーの弾と衝突した場合
		isDead_ = true; // デスフラグを立てる
	}
	if (_other->GetColliderID() == "Player")
	{
		// プレイヤーと衝突した場合
		isDead_ = true; // デスフラグを立てる
	}
}

void TimeBomb::SetTrapLandingPosition(const Vector3& _playerPosition)
{
	// ランダムな角度（0〜2π）
	float angle = static_cast<float>(rand()) / RAND_MAX * std::numbers::pi_v<float> *2.0f;

	// ランダムな半径（2.5〜4.0）
	float radius = 2.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;

	// オフセットを計算（XZ平面）
	Vector3 offset;
	offset.x = std::cos(angle) * radius;
	offset.z = std::sin(angle) * radius;
	offset.y = 0.0f;

	// プレイヤーの位置にオフセットを加え、Y座標を0.5に
	landingPosition_ = _playerPosition + offset;
	landingPosition_.y = 0.5f;
}
