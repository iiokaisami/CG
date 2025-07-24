#include "VignetteTrap.h"

#include <numbers>

void VignetteTrap::Initialize()
{
	// --- 3Dオブジェクト ---
	object_ = std::make_unique<Object3d>();
	object_->Initialize("vignette.obj");

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

	// 仮置き
	scale_ = { 0.7f,0.7f,0.7f };
	object_->SetScale(scale_);

	// 当たり判定
	colliderManager_ = ColliderManager::GetInstance();
	objectName_ = "VignetteTrap";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&VignetteTrap::OnCollisionTrigger, this, std::placeholders::_1));
	colliderManager_->RegisterCollider(&collider_);
}

void VignetteTrap::Finalize()
{
	colliderManager_->DeleteCollider(&collider_);
}

void VignetteTrap::Update()
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

void VignetteTrap::Draw()
{
	object_->Draw();
}

void VignetteTrap::ImGuiDraw()
{
}

void VignetteTrap::UpdateModel()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);
}

void VignetteTrap::LaunchTrap()
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

void VignetteTrap::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() != "Field" && 
		_other->GetColliderID() != "Wall" && 
		_other->GetColliderID() != "EnemyBullet" &&
		_other->GetColliderID() != "TrapEnemy")
	{
		// 床と壁、敵の弾以外に当たると死亡
		isDead_ = true;
	}

	// プレイヤーとの衝突
	if (_other->GetColliderID() == "Player")
	{
		// 暗闇効果を適用
	}

	// ノーマルエネミーとの衝突
	if (_other->GetColliderID() == "NormalEnemy")
	{
		// 敵に行動不能を適用
	}
}

void VignetteTrap::SetTrapLandingPosition(const Vector3& _playerPosition)
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
