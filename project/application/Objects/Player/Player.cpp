#include "Player.h"

void Player::Initialize()
{
	// --- 3Dオブジェクト ---
	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");


	position_ = { 0.2f,0.7f,-1.2f };
	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	
	// 仮置き
	scale_ = { 1.0f,1.5f,1.0f };
	object_->SetScale(scale_);

	// 衝突判定
	colliderManager_ = ColliderManager::GetInstance();

	objectName_ = "Player";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetShape(Shape::AABB);
	collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&Player::OnCollisionTrigger, this, std::placeholders::_1));
	collider_.SetOnCollision(std::bind(&Player::OnCollision, this, std::placeholders::_1));
	colliderManager_->RegisterCollider(&collider_);

	// ステータス
	hp_ = 3;
	isDead_ = false;

	// パーティクル

}

void Player::Finalize()
{
	for (auto& bullet : pBullets_)
	{
		bullet->SetIsDead(true);
		bullet->Finalize();
	}

	pBullets_.remove_if([](PlayerBullet* bullet)
		{
			if (bullet->IsDead())
			{
				bullet->Finalize();
				delete bullet;
				return true;
			}
			return false;
		});

	colliderManager_->DeleteCollider(&collider_);
}

void Player::Update()
{
	object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);
	object_->Update();

	// 移動
	Move();
	
	// 攻撃
	Attack();

	// 弾の削除
	pBullets_.remove_if([](PlayerBullet* bullet)
		{
		if (bullet->IsDead()) {
			bullet->Finalize();
			delete bullet;
			return true;
		}
		return false;
		});

	// 弾更新
	for (auto& bullet : pBullets_)
	{
		bullet->Update();
	}

	if (isWallCollision_)
	{
		// 壁に衝突した場合の処理
		WallCollision();
		isWallCollision_ = false;
	}

	// AABBの更新
	aabb_.min = position_ - object_->GetScale();
	aabb_.max = position_ + object_->GetScale();
	aabb_.max.y += 1.0f;
	collider_.SetPosition(position_);
}

void Player::Draw()
{
	object_->Draw();

	// 弾描画
	for (auto& bullet : pBullets_)
	{
		bullet->Draw();
	}
}

void Player::Draw2D()
{
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");

	ImGui::SliderFloat3("pos", &position_.x, -290.0f, 290.0f);
	ImGui::SliderFloat3("rot", &rotation_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("scale", &scale_.x, 0.0f, 10.0f);

	// HitMomentオン
	if (ImGui::Button("HitMoment"))
	{
		isHitMoment_ = true;
	}

	ImGui::End();

	for (auto& bullet : pBullets_)
	{
		bullet->ImGuiDraw();
	}
}

void Player::Move()
{
	moveVelocity_ = {};

	// 画面上の見た目通りに移動（カメラ回転なし）
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		moveVelocity_.z += moveSpeed_.z;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		moveVelocity_.z -= moveSpeed_.z;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		moveVelocity_.x -= moveSpeed_.x;
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		moveVelocity_.x += moveSpeed_.x;
	}

	// 移動ベクトルがゼロでない場合にプレイヤーの向きを補間で更新
	if (moveVelocity_.x != 0.0f || moveVelocity_.z != 0.0f)
	{
		float targetAngle = std::atan2(moveVelocity_.x, moveVelocity_.z);
		const float rotationSpeed = 0.1f;
		rotation_.y += (targetAngle - rotation_.y) * rotationSpeed;
	}

	// 位置更新
	position_ += moveVelocity_;
}

void Player::Attack()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		/// プレイヤーの向きに合わせて弾の速度を変更
		Vector3 bulletVelocity = {
			std::cosf(rotation_.x) * std::sinf(rotation_.y),    // x
			std::sinf(-rotation_.x),                             // y
			std::cosf(rotation_.x) * std::cosf(rotation_.y)     // z
		};

		if (countCoolDownFrame_ <= 0)
		{
			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();

			newBullet->SetPosition(position_);
			newBullet->Initialize();
			newBullet->SetVelocity(bulletVelocity);

			newBullet->RunSetMask();
			collider_.SetMask(colliderManager_->GetNewMask(collider_.GetColliderID(), "PlayerBullet"));

			// 弾を登録する
			pBullets_.push_back(newBullet);

			countCoolDownFrame_ = kShootCoolDownFrame_;
		}
	}
	countCoolDownFrame_--;
}

void Player::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "EnemyBullet")
	{
		// プレイヤーのHPを減少
		if (hp_ > 0)
		{
			hp_--;
		}
		else
		{
			//isDead_ = true;
		}

		isHitMoment_ = true;
	} 
	
	if (_other->GetColliderID() == "Enemy")
	{
		// プレイヤーのHPを減少
		if (hp_ > 0)
		{
			hp_--;
		} 
		else
		{
			//isDead_ = true;
		}
		isHitMoment_ = true;
	}

}

void Player::OnCollision(const Collider* _other)
{

	if (_other->GetColliderID() == "Wall")
	{
		isWallCollision_ = true;
		collisionWallAABB_ = *_other->GetAABB();
	}
}

void Player::WallCollision()
{
	Vector3 penetrationVector{};

	// 壁との重なりを計算

	// x軸(左右)
	float overlapLeftX = collisionWallAABB_.max.x - aabb_.min.x;
	float overlapRightX = aabb_.max.x - collisionWallAABB_.min.x;
	float correctionX = 0.0f;

	if (overlapLeftX < overlapRightX)
	{
		correctionX = overlapLeftX;
	} 
	else
	{
		correctionX = -overlapRightX;
	}

	// y軸(上下)
	float overlapBelowY = collisionWallAABB_.max.y - aabb_.min.y;
	float overlapAboveY = aabb_.max.y - collisionWallAABB_.min.y;
	float correctionY = 0.0f;

	if (overlapBelowY < overlapAboveY)
	{
		correctionY = overlapBelowY;
	}
	else
	{
		correctionY = -overlapAboveY;
	}

	// z軸(前後)
	float overlapBackZ = collisionWallAABB_.max.z - aabb_.min.z;
	float overlapFrontZ = aabb_.max.z - collisionWallAABB_.min.z;
	float correctionZ = 0.0f;

	if (overlapBackZ < overlapFrontZ)
	{
		correctionZ = overlapBackZ;
	}
	else
	{
		correctionZ = -overlapFrontZ;
	}

	// 最小の重なりを持つ軸を選択
	float absX = std::abs(correctionX);
	float absY = std::abs(correctionY);
	float absZ = std::abs(correctionZ);

	if (absX <= absY && absX <= absZ)
	{
		penetrationVector.x = correctionX;
	} 
	else if (absY <= absZ)
	{
		penetrationVector.y = correctionY;
	}
	else
	{
		penetrationVector.z = correctionZ;
	}

	// 位置を修正
	position_ += penetrationVector;
}
