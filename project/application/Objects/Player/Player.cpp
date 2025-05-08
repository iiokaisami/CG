#include "Player.h"

void Player::Initialize()
{
	// --- 3Dオブジェクト ---
	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");


	position_ = { 0.2f,0.7f,0.2f };
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
	colliderManager_->RegisterCollider(&collider_);

	// ステータス
	hp_ = 5;
	isDead_ = false;
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

	// 移動処理
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		moveVelocity_.z += moveSpeed_.z;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		moveVelocity_.z += -moveSpeed_.z;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		moveVelocity_.x += -moveSpeed_.x;
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		moveVelocity_.x += moveSpeed_.x;
	}

	// 移動ベクトルがゼロでない場合に回転を計算
	if (moveVelocity_.x != 0.0f or moveVelocity_.z != 0.0f)
	{
		// 移動方向の角度を計算 (atan2を使用)
		float targetAngle = std::atan2(moveVelocity_.x, moveVelocity_.z);

		// 現在の回転角度と目標角度を補間 (Lerp)
		const float rotationSpeed = 0.1f; // 回転速度 (0.0f～1.0f)
		rotation_.y = rotation_.y + (targetAngle - rotation_.y) * rotationSpeed;
	}

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
		isHitMoment_ = true;
	}
}
