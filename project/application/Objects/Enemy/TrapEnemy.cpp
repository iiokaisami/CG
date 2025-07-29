#include "TrapEnemy.h"

// BehaviorState
#include "BehaviorState/TrapEnemyState/TrapEnemyBehaviorSpawn.h"
#include "BehaviorState/TrapEnemyState/TrapEnemyBehaviorMove.h"
#include "BehaviorState/TrapEnemyState/TrapEnemyBehaviorSetTrap.h"
#include "BehaviorState/TrapEnemyState/TrapEnemyBehaviorHitReact.h"
#include "BehaviorState/TrapEnemyState/TrapEnemyBehaviorDead.h"


void TrapEnemy::Initialize()
{
    // --- 3Dオブジェクト ---
    object_ = std::make_unique<Object3d>();
    object_->Initialize("trapEnemy.obj");

    moveVelocity_ = { 0.1f,0.1f,0.0f };

    object_->SetPosition(position_);
    object_->SetRotate(rotation_);
    // 仮置き
    scale_ = { 1.0f,1.0f,1.0f };
    object_->SetScale(scale_);


    colliderManager_ = ColliderManager::GetInstance();

    objectName_ = "TrapEnemy";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&TrapEnemy::OnCollisionTrigger, this, std::placeholders::_1));
    collider_.SetOnCollision(std::bind(&TrapEnemy::OnCollision, this, std::placeholders::_1));
    colliderManager_->RegisterCollider(&collider_);

    // ステータス
    hp_ = 3;
    isDead_ = false;

    // 行動ステート
    ChangeBehaviorState(std::make_unique<TrapEnemyBehaviorSpawn>(this));

    // 出現時は無敵状態
    isInvincible_ = true;
}

void TrapEnemy::Finalize()
{
    colliderManager_->DeleteCollider(&collider_);

    // 罠
	for (auto& trap : pTimeBomb_)
	{
		trap->SetIsDead(true);
		trap->Finalize();
	}

	pTimeBomb_.erase(
		std::remove_if(pTimeBomb_.begin(), pTimeBomb_.end(), [](std::unique_ptr<TimeBomb>& trap)
			{
				if (trap->IsDead())
				{
					trap->Finalize();
					return true;
				}
				return false;
			}),
		pTimeBomb_.end()
	);

    for (auto& trap : pVignetteTrap_)
    {
		trap->SetIsDead(true);
		trap->Finalize();
    }

	pVignetteTrap_.erase(
		std::remove_if(pVignetteTrap_.begin(), pVignetteTrap_.end(), [](std::unique_ptr<VignetteTrap>& trap)
			{
				if (trap->IsDead())
				{
					trap->Finalize();
					return true;
				}
				return false;
			}),
		pVignetteTrap_.end()
	);

}

void TrapEnemy::Update()
{
    // アクティブフラグに代入
    isActive_ = !isInvincible_;

    // 各行動ステートの更新
    pBehaviorState_->Update();

    object_->Update();

    // プレイヤーとの距離を計算
    float distanceToPlayer = position_.Distance(playerPosition_);

	isEscape_ = false;
	isApproach_ = false;
	isStopAndTrap_ = false;

    // 距離に応じてフラグを更新
    if (distanceToPlayer < kTooCloseDistance) 
    {
        // 離れる
		isEscape_ = true;
    }
    else if (distanceToPlayer > kTooFarDistance)
    {
        // 近づく
		isApproach_ = true;
	}
    else
    {
		// 罠を設置する
		isStopAndTrap_ = true;
    }

    // 罠の削除
	pTimeBomb_.erase(
		std::remove_if(pTimeBomb_.begin(), pTimeBomb_.end(), [](std::unique_ptr<TimeBomb>& trap)
			{
				if (trap->IsDead())
				{
					trap->Finalize();
					return true;
				}
				return false;
			}),
		pTimeBomb_.end()
	);

	pVignetteTrap_.erase(
		std::remove_if(pVignetteTrap_.begin(), pVignetteTrap_.end(), [](std::unique_ptr<VignetteTrap>& trap)
			{
				if (trap->IsDead())
				{
					trap->Finalize();
					return true;
				}
				return false;
			}),
		pVignetteTrap_.end()
	);

    // 罠の更新
	for (auto& trap : pTimeBomb_)
	{
		trap->Update();
	}

	for (auto& trap : pVignetteTrap_)
	{
		trap->Update();
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

void TrapEnemy::Draw()
{
    object_->Draw();

	// 罠の描画
	for (auto& trap : pTimeBomb_)
	{
		trap->Draw();
	}
	for (auto& trap : pVignetteTrap_)
	{
		trap->Draw();
	}
}

void TrapEnemy::Draw2D()
{
}

void TrapEnemy::ImGuiDraw()
{
	ImGui::Begin("TrapEnemy");
	ImGui::Text("Position: (%.2f, %.2f, %.2f)", position_.x, position_.y, position_.z);
	ImGui::Text("Rotation: (%.2f, %.2f, %.2f)", rotation_.x, rotation_.y, rotation_.z);
	ImGui::Text("Scale: (%.2f, %.2f, %.2f)", scale_.x, scale_.y, scale_.z);
	ImGui::Text("HP: %d", hp_);
	ImGui::Text("Is Dead: %s", isDead_ ? "Yes" : "No");
	ImGui::End();
}

void TrapEnemy::Move()
{
    toPlayer_ = playerPosition_ - position_;
    Vector3 direction = Normalize(toPlayer_);
    moveVelocity_ = Normalize(moveVelocity_);

	if (isEscape_)
	{
		// 離れる
		moveVelocity_ = Slerp(moveVelocity_, -direction, 0.1f);
	}
    else if (isApproach_)
	{
		// 近づく
		moveVelocity_ = Slerp(moveVelocity_, direction, 0.1f);
	}
    // 距離が適切かつ罠のクールタイムが無かったら疑似停止
    else if (isStopAndTrap_ && isTrapCooldownComplete_)
	{
		// 罠を設置するため停止
		moveVelocity_ = { 0.0f, 0.0f, 0.00001f };
	}
    

    // Y軸回転を進行方向に合わせる
    rotation_.y = std::atan2(moveVelocity_.x, moveVelocity_.z);
    rotation_.x = 0.0f;

    moveVelocity_ /= 30.0f;
    moveVelocity_.y = 0.0f;
    position_ += moveVelocity_;

    ObjectTransformSet(position_, rotation_, scale_);
}


void TrapEnemy::TrapInit()
{
    if (isNextTrapTimeBomb_ && isTrapCooldownComplete_)
    {
        // TimeBomb発射
        auto timeBomb = std::make_unique<TimeBomb>();
        timeBomb->Initialize();
        timeBomb->SetPosition(position_);
        timeBomb->SetTrapLandingPosition(playerPosition_);
        timeBomb->LaunchTrap();
        timeBomb->UpdateModel();
        pTimeBomb_.push_back(std::move(timeBomb));
    } 
	else if (!isNextTrapTimeBomb_ && isTrapCooldownComplete_)
    {
        // VignetteTrap発射
        auto vignetteTrap = std::make_unique<VignetteTrap>();
        vignetteTrap->Initialize();
        vignetteTrap->SetPosition(position_);
        vignetteTrap->SetTrapLandingPosition(playerPosition_);
        vignetteTrap->LaunchTrap();
        vignetteTrap->UpdateModel();
        pVignetteTrap_.push_back(std::move(vignetteTrap));
    }
}

void TrapEnemy::ChangeBehaviorState(std::unique_ptr<TrapEnemyBehaviorState> _pState)
{
    pBehaviorState_ = std::move(_pState);
    pBehaviorState_->Initialize();
}

void TrapEnemy::ObjectTransformSet(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale)
{
    object_->SetPosition(_position);
    object_->SetRotate(_rotation);
    object_->SetScale(_scale);
}

void TrapEnemy::OnCollisionTrigger(const Collider* _other)
{
    if (_other->GetColliderID() == "PlayerBullet" && !isInvincible_)
    {
        // プレイヤーの弾と衝突した場合
        if (hp_ > 0)
        {
            // HP減少
            hp_--;

            isHit_ = true;
        }
    }
}

void TrapEnemy::OnCollision(const Collider* _other)
{
    if (_other->GetColliderID() == "NormalEnemy" or _other->GetColliderID() == "TrapEnemy")
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

void TrapEnemy::CorrectOverlap(const AABB _anyAABB)
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
