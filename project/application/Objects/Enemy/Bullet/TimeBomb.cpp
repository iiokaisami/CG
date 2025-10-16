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
	// 設置判定用
	objectName_ = "SetTimeBomb";
	setDesc =
	{
		//ここに設定
		.owner = this,
		.colliderID = objectName_,
		.shape = Shape::AABB,
		.shapeData = &setAABB_,
		.attribute = colliderManager_->GetNewAttribute(objectName_),
		.onCollision = std::bind(&TimeBomb::OnSetCollision, this, std::placeholders::_1),
		.onCollisionTrigger = std::bind(&TimeBomb::OnSetCollisionTrigger, this, std::placeholders::_1),
	};
	setCollider_.MakeAABBDesc(setDesc);
	colliderManager_->RegisterCollider(&setCollider_);
	// 爆発判定用
	explosionObjectName_ = "ExplosionTimeBomb";
	explosionDesc =
	{
		//ここに設定
		.owner = this,
		.colliderID = objectName_,
		.shape = Shape::AABB,
		.shapeData = &explosionAABB_,
		.attribute = colliderManager_->GetNewAttribute(objectName_),
		.onCollisionTrigger = std::bind(&TimeBomb::OnExplosionTrigger, this, std::placeholders::_1),
	};
	explosionCollider_.MakeAABBDesc(explosionDesc);
	colliderManager_->RegisterCollider(&explosionCollider_);

}

void TimeBomb::Finalize()
{
	colliderManager_->DeleteCollider(&setCollider_);
	colliderManager_->DeleteCollider(&explosionCollider_);
}

void TimeBomb::Update()
{
	// 着弾していなければ判定を付けない
	isActive_ = !isLaunchingTrap_;

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

	if ((position_ - landingPosition_).Length() < 0.1f or position_.y <= 0.5f)
	{
		isLaunchingTrap_ = false;
	}
	else
	{
		// 地面に埋まらないようにする
		if (position_.y < 0.5f)
		{
			position_.y = 0.5f;
			velocity_.y = 0.0f; // Y方向の速度をリセット
		}
	}

	// 壁との反射クールタイム
	if (wallCollisionCooldown_ > 0)
	{
		wallCollisionCooldown_--;
	}

	if (wallCollisionCooldown_ <= 0 && isWallCollision_)
	{
		// 壁に衝突した場合の処理
		ReflectOnWallCollision();
		wallCollisionCooldown_ = 1;
		isWallCollision_ = false;
	}

	UpdateModel();
	
	rotation_ += {0.1f, 0.1f, 0.0f};
	
	// 設置判定の更新
	setAABB_.min = position_ - object_->GetScale();
	setAABB_.max = position_ + object_->GetScale();
	setCollider_.SetPosition(position_);
	// 爆発判定の更新
	explosionAABB_.min = position_ - object_->GetScale();
	explosionAABB_.max = position_ + object_->GetScale();
	explosionCollider_.SetPosition(position_);
	
}

void TimeBomb::Draw()
{
	if (!isExploded_)
	{
		object_->Draw();
	}
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

void TimeBomb::OnSetCollisionTrigger(const Collider* _other)
{
	// プレイヤー、ノーマルエネミー、プレイヤーの弾と衝突した場合
	if (isActive_ && (_other->GetColliderID() == "Player" or 
		_other->GetColliderID() == "NormalEnemy" or 
		_other->GetColliderID() == "PlayerBullet"))
	{
		// 爆発状態へ
		isExploded_ = true;
	}

}

void TimeBomb::OnSetCollision(const Collider* _other)
{
	if (_other->GetColliderID() == "Wall" or
		_other->GetColliderID() == "Barrie")
	{
		collisionWallAABB_ = *_other->GetAABB();
		isWallCollision_ = true;
	}

	if (_other->GetColliderID() == "TrapEnemy" or
		_other->GetColliderID() == "VignetteTrap" or
		_other->GetColliderID() == "SetTimeBomb")
	{
		const AABB* otherAABB = _other->GetAABB();

		if (otherAABB && IsAABBOverlap(setAABB_, *otherAABB))
		{
			CorrectOverlap(*otherAABB, setAABB_, position_);
		}
	}

}

void TimeBomb::OnExplosionTrigger(const Collider* _other)
{
	if (isExploded_ &&
		(_other->GetColliderID() == "Player" or 
			_other->GetColliderID() == "NormalEnemy" or
			_other->GetColliderID() == "PlayerBullet"))
	{
		isDead_ = true;
		// パーティクル起動
		ParticleEmitter::Emit("explosionGroup", position_, 6);
	}
}

void TimeBomb::Explode()
{
	// 爆発処理
	if (isExploded_)
	{

		// scaleを徐々に大きくして判定を広げる
		elapsedTime += 1.0f/60.0f;

		// 経過割合（0.0〜1.0）
		float t = std::clamp(elapsedTime / duration, 0.0f, 1.0f);

		// Lerpでスケール補間
		currentScale = Lerp(startScale, endScale, t);

		// スケールを適用
		SetScale(currentScale); // あなたのオブジェクト用関数

		// 爆発後はオブジェクトを削除
		isDead_ = true;
	}
}

void TimeBomb::ReflectOnWallCollision()
{

	// 重なり補正と同じく、軸ごとの重なり量を調べる
	float overlapLeftX = collisionWallAABB_.max.x - setAABB_.min.x;
	float overlapRightX = setAABB_.max.x - collisionWallAABB_.min.x;
	float correctionX = (overlapLeftX < overlapRightX) ? overlapLeftX : -overlapRightX;

	float overlapBackZ = collisionWallAABB_.max.z - setAABB_.min.z;
	float overlapFrontZ = setAABB_.max.z - collisionWallAABB_.min.z;
	float correctionZ = (overlapBackZ < overlapFrontZ) ? overlapBackZ : -overlapFrontZ;

	float absX = std::abs(correctionX);
	float absZ = std::abs(correctionZ);

	// 目標地点を反転
	if (absX <= absZ)
	{
		landingPosition_.x = position_.x + (position_.x - landingPosition_.x);
		// 壁の外側に十分押し出す
		position_.x += (correctionX + (correctionX > 0 ? 0.5f : -0.5f));
	} else
	{
		landingPosition_.z = position_.z + (position_.z - landingPosition_.z);
		// 壁の外側に十分押し出す
		position_.z += (correctionZ + (correctionZ > 0 ? 0.5f : -0.5f));
	}

	// ここで新しい放物線運動を再計算
	LaunchTrap();
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
