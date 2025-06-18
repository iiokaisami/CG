#include "Enemy.h"

#include <Ease.h>

void Enemy::Initialize()
{
    // --- 3Dオブジェクト ---
    object_ = std::make_unique<Object3d>();
    object_->Initialize("cube.obj");

    moveVelocity_ = { 0.1f,0.1f,0.0f };

    object_->SetPosition(position_);
    object_->SetRotate(rotation_);

    // 仮置き
    scale_ = { 0.1f,0.1f,0.1f };
	object_->SetScale(scale_);

    colliderManager_ = ColliderManager::GetInstance();

    objectName_ = "Enemy";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollisionTrigger, this, std::placeholders::_1));
	collider_.SetOnCollision(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
    colliderManager_->RegisterCollider(&collider_);

    // ステータス
    hp_ = 3;
    isDead_ = false;
	popMotion_.isActive = true;
}

void Enemy::Finalize()
{
	for (auto& bullet : pBullets_)
	{
		bullet->SetIsDead(true);
		bullet->Finalize();
	}

    pBullets_.erase(
        std::remove_if(pBullets_.begin(), pBullets_.end(), [](std::unique_ptr<EnemyBullet>& bullet)
            {
                if (bullet->IsDead())
                {
                    bullet->Finalize();
                    return true;
                }
                return false;
            }),
        pBullets_.end()
    );

    colliderManager_->DeleteCollider(&collider_);
}

void Enemy::Update()
{
    object_->SetPosition(position_);
	object_->SetRotate(rotation_);
	object_->SetScale(scale_);
    object_->Update();

    ///////////////////////////////////////////////

    if (popMotion_.isActive)
    {
        PopMotion();
    }
    if (moveMotion_.isActive)
    {
        MoveMotion();
    }
    if (attackMotion_.isActive)
    {
        AttackMotion();
    }
    if (hitMotion_.isActive)
    {
        HitMotion();
    }
    if (deadMotion_.isActive)
    {
        DeadMotion();
    }

    ///////////////////////////////////////////////

    // 出現モーションが終わるまで動かない
	if (!popMotion_.isActive)
	{
        // 移動
        Move();
	
        // 攻撃
        Attack();
    }

	// 敵の弾の削除
    pBullets_.erase(
        std::remove_if(pBullets_.begin(), pBullets_.end(), [](std::unique_ptr<EnemyBullet>& bullet)
            {
                if (bullet->IsDead())
                {
                    bullet->Finalize();
                    return true;
                }
                return false;
            }),
        pBullets_.end()
    );

    // 弾の更新
    for (auto& bullet : pBullets_)
    {
        bullet->Update();
    }

	// aabbの更新
    aabb_.min = position_ - object_->GetScale();
    aabb_.max = position_ + object_->GetScale();
    aabb_.max.y += 1.0f;
    collider_.SetPosition(position_);

    // 壁に衝突した場合の処理
    if (isWallCollision_)
    {
		CorrectOverlap(collisionWallAABB_);
        isWallCollision_ = false;
    }

    
}

void Enemy::Draw()
{
    object_->Draw();

	// 弾描画
	for (auto& bullet : pBullets_)
	{
		bullet->Draw();
	}
}

void Enemy::Draw2D()
{
}

void Enemy::ImGuiDraw()
{
	ImGui::Begin("Enemy");

	ImGui::SliderFloat3("position", &position_.x, -30.0f, 30.0f);
	ImGui::SliderFloat3("rotation", &rotation_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("scale", &scale_.x, 0.0f, 10.0f);


    ImGui::End();

	for (auto& bullet : pBullets_)
	{
		bullet->ImGuiDraw();
	}
}

void Enemy::Move()
{
    // プレイヤーとの距離を計算
    float distanceToPlayer = position_.Distance(playerPosition_);

    if (distanceToPlayer <= kStopChasingDistance)
    {
        // プレイヤーとの距離が一定以下の場合、追尾を停止(0にすると色々まずかった)
        moveVelocity_ = { 0.000001f, 0.0f, 0.0f };

		// 移動モーションを無効にする
        moveMotion_.isActive = false;
        
        return;
    }
    else
    {
        // 追尾中は移動モーションを有効にする
		moveMotion_.isActive = true; 

        // 敵弾から自キャラへのベクトルを計算
        toPlayer_ = playerPosition_ - position_;

        // ベクトルを正規化する
        toPlayer_ = Normalize(toPlayer_);
        moveVelocity_ = Normalize(moveVelocity_);

        // 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
        moveVelocity_ = 1.0f * (Slerp(moveVelocity_, toPlayer_, 0.1f));

        // 進行方向に見た目の回転を合わせる
        // Y軸周り角度(θy)
        rotation_.y = std::atan2(moveVelocity_.x, moveVelocity_.z);
        rotation_.x = 0.0f;

        moveVelocity_ /= 20.0f;

        moveVelocity_.y = 0.0f;
        position_ += moveVelocity_;
    }
}

void Enemy::Attack()
{
    // プレイヤーとの距離を計算
    float distanceToPlayer = position_.Distance(playerPosition_);

    // 発射条件: プレイヤーとの距離が一定以下
    if (distanceToPlayer <= kStopChasingDistance)
    {
        // 一定間隔で弾を発射
        if (attackCooldown_ <= 0)
        {
            // 攻撃モーションを開始
			attackMotion_.isActive = true; 
        }

        if (isAttack_)
        {
            // 弾の数と間隔角度
            const int bulletCount = 36; 
            const float angleStep = 360.0f / bulletCount;

            for (int i = 0; i < bulletCount; ++i)
            {
                // 弾の角度を計算 (ラジアンに変換)
                float angle = DirectX::XMConvertToRadians(i * angleStep);

                // 弾の方向を計算
                Vector3 bulletDirection = 
                {
                    std::cos(angle), // X成分
                    0.0f,            // Y成分
                    std::sin(angle)  // Z成分
                };

                // 弾を生成
                auto bullet = std::make_unique<EnemyBullet>();
                bullet->Initialize();
                bullet->SetPosition({ position_.x,position_.y + 0.5f,position_.z }); // 敵の位置より少し上を初期位置に設定
                bullet->SetVelocity(bulletDirection * 0.2f);
                bullet->UpdateModel();

                // 弾をリストに追加
                pBullets_.push_back(std::move(bullet));
            }

            // クールダウンを設定
            attackCooldown_ = 60 * 4;
        } 
        else
        {
			if (attackCooldown_ >= 300)
			{
                attackCooldown_ = 60 * 4;
			}

            // クールダウンを減少
            --attackCooldown_;
        }
    }
}

void Enemy::PopMotion()
{
    float t = float(popMotion_.count) / popMotion_.maxCount;
    float scale = Ease::OutBack(t); // 0〜1 の範囲で膨らみつつ出現
	Vector3 one(1.0f, 1.0f, 1.0f);
    scale_ = one * scale;

	if (popMotion_.count < popMotion_.maxCount)
	{
		popMotion_.count++;
	}
    else
	{
        // モーション終了 カウントリセット
		popMotion_.isActive = false; 
        popMotion_.count = 0;
	}
}

void Enemy::MoveMotion()
{
    // 進行度（0〜1）
    float t = float(moveMotion_.count) / moveMotion_.maxCount;

    // 上下にゆっくり波打つ動き
    float wave = std::sin(t * std::numbers::pi_v<float>) * 0.3f;

   // position_ += Vector3(0.0f, wave, 0.0f);

	//SetPosition(position_ + Vector3(0.0f, wave, 0.0f));

	if (moveMotion_.count < moveMotion_.maxCount)
	{
		moveMotion_.count++;
	}
    else
	{
        // モーション終了 カウントリセット
		moveMotion_.isActive = false;
		moveMotion_.count = 0; 
	}
}

void Enemy::AttackMotion()
{
    // 溜め
    if (attackMotion_.count <= 20)
    {
        float t = float(attackMotion_.count) / 20.0f; // 0〜1
        float ease = (t <= 0.5f)
            ? Ease::OutSine(t * 2.0f)               // 0〜0.5  → 縮小
            : Ease::OutSine((1.0f - t) * 2.0f);     // 0.5〜1  → 元に戻す

        float scaleValue = 1.0f - ease * 0.3f;
        scale_ = Vector3(scaleValue, scaleValue, scaleValue);

        float shake = ((attackMotion_.count % 2 == 0) ? 1 : -1) * 0.05f;
        object_->SetPosition(position_ + Vector3(shake, 0, shake));
    }
    // 攻撃
    else
    {
        // 攻撃フラグをリセット
		isAttack_ = false;

        if (attackMotion_.count == 21)
        {
            // 攻撃フラグを立てる
            isAttack_ = true;
        }

        // Y軸回転
        rotation_.y += 0.3f;
        object_->SetRotate(rotation_);
    }


	if (attackMotion_.count < attackMotion_.maxCount)
	{
        attackMotion_.count++;
	}
    else
	{
		// モーション終了 カウントリセット
		attackMotion_.isActive = false;
        attackMotion_.count = 0; 
	}
}

void Enemy::HitMotion()
{
    Vector3 shakeOffset =
    {
    ((hitMotion_.count % 2 == 0) ? 1.0f : -1.0f) * 0.15f,
	0.0f, // Y軸は揺らさない
    ((hitMotion_.count % 3 == 0) ? 1.0f : -1.0f) * 0.1f
    };

    SetPosition(position_ + shakeOffset);

	if (hitMotion_.count < hitMotion_.maxCount)
	{
		hitMotion_.count++;
	}
    else
	{
        // モーション終了 カウントリセット
		hitMotion_.isActive = false;
		hitMotion_.count = 0;
	}
}

void Enemy::DeadMotion()
{
    float t = float(deadMotion_.count) / deadMotion_.maxCount;

    if (deadMotion_.count == 0) 
    {
        scale_ = Vector3(1.8f, 1.8f, 1.8f); // 初回だけ一気に膨らむ
    }

    // 徐々に縮む演出（1.8 → 0.0）
    float scale = Lerp(1.8f, 0.0f, Ease::InCubic(t));
    scale_ = (Vector3(scale, scale, scale));

    position_.y += Ease::OutQuad(t) * 0.1f;

    rotation_.y += 0.1f;
	rotation_.x += 0.1f;

	if (deadMotion_.count < deadMotion_.maxCount)
	{
		deadMotion_.count++;
	}
    else
	{
		// モーション終了&死亡処理
		deadMotion_.isActive = false; 
		isDead_ = true;
	}
}

void Enemy::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "PlayerBullet")
	{
		// プレイヤーの弾と衝突した場合
		if (hp_ > 0)
		{
            // HP減少
			hp_--;

            // 被弾モーションを開始
			hitMotion_.isActive = true;
		}
        else
		{
            // パーティクル
            ParticleEmitter::Emit("explosionGroup", position_, 6);

            // 死亡モーションを開始
			deadMotion_.isActive = true;
		}
	}
}

void Enemy::OnCollision(const Collider* _other)
{
    if (_other->GetColliderID() == "Enemy")
    {
        
        // 敵の位置
        Vector3 enemyPosition = _other->GetOwner()->GetPosition();

        // 敵同士が重ならないようにする
        Vector3 direction = position_ - enemyPosition;
        direction.Normalize();
        float distance = 2.5f; // 敵同士の間の距離を調整するための値

        // 互いに重ならないように少しずつ位置を調整
        if ((position_ - enemyPosition).Length() < distance) 
        {
            position_ += direction * 0.1f; // 微調整のための値
        }
    }

    if (_other->GetColliderID() == "Wall")
    {
        isWallCollision_ = true;
        collisionWallAABB_ = *_other->GetAABB();
    }
}

void Enemy::CorrectOverlap(const AABB _anyAABB)
{
    Vector3 penetrationVector{};

    // 壁との重なりを計算
    // x軸(左右)
    float overlapLeftX = _anyAABB.max.x - aabb_.min.x;
    float overlapRightX = aabb_.max.x - _anyAABB.min.x;
    float correctionX = 0.0f;

    if (overlapLeftX < overlapRightX)
    {
        correctionX = overlapLeftX;
    } else
    {
        correctionX = -overlapRightX;
    }

    // y軸(上下)
    float overlapBelowY = _anyAABB.max.y - aabb_.min.y;
    float overlapAboveY = aabb_.max.y - _anyAABB.min.y;
    float correctionY = 0.0f;

    if (overlapBelowY < overlapAboveY)
    {
        correctionY = overlapBelowY;
    } else
    {
        correctionY = -overlapAboveY;
    }

    // z軸(前後)
    float overlapBackZ = _anyAABB.max.z - aabb_.min.z;
    float overlapFrontZ = aabb_.max.z - _anyAABB.min.z;
    float correctionZ = 0.0f;

    if (overlapBackZ < overlapFrontZ)
    {
        correctionZ = overlapBackZ;
    } else
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
    } else if (absY <= absZ)
    {
        penetrationVector.y = correctionY;
    } else
    {
        penetrationVector.z = correctionZ;
    }

    // 位置を修正
    position_ += penetrationVector;
}