#include "Enemy.h"

void Enemy::Initialize()
{
    // --- 3Dオブジェクト ---
    object_ = std::make_unique<Object3d>();
    object_->Initialize("cube.obj");

    moveVelocity_ = { 0.1f,0.1f,0.0f };

    object_->SetPosition(position_);
    object_->SetRotate(rotation_);

    // 仮置き
    scale_ = { 1.0f,1.0f,1.0f };
    object_->SetScale(scale_);

    colliderManager_ = ColliderManager::GetInstance();

    objectName_ = "Enemy";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(colliderManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollisionTrigger, this, std::placeholders::_1));
    colliderManager_->RegisterCollider(&collider_);

    // ステータス
    hp_ = 5;
    isDead_ = false;

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

    // 移動
	Move();

    // 攻撃
	Attack();

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

	// 敵同士の衝突判定
	//EnemyCollision();


    aabb_.min = position_ - object_->GetScale();
    aabb_.max = position_ + object_->GetScale();
    aabb_.max.y += 1.0f;
    collider_.SetPosition(position_);
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
        return;
    }
    else
    {
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
        static int attackCooldown = 0;
        if (attackCooldown <= 0)
        {
            const int bulletCount = 36; // 弾の数
            const float angleStep = 360.0f / bulletCount; // 弾の間隔角度

            for (int i = 0; i < bulletCount; ++i)
            {
                // 弾の角度を計算 (ラジアンに変換)
                float angle = DirectX::XMConvertToRadians(i * angleStep);

                // 弾の方向を計算
                Vector3 bulletDirection = {
                    std::cos(angle), // X成分
                    0.0f,            // Y成分
                    std::sin(angle)  // Z成分
                };

                // 弾を生成
                auto bullet = std::make_unique<EnemyBullet>();
                bullet->Initialize();
                bullet->SetPosition({ position_.x,position_.y + 0.5f,position_.z }); // 敵の位置より少し上を初期位置に設定
                bullet->SetVelocity(bulletDirection * 0.2f); // 弾の速度を設定

				bullet->RunSetMask();
				collider_.SetMask(colliderManager_->GetNewMask(collider_.GetColliderID(), "EnemyBullet"));

                // 弾をリストに追加
                pBullets_.push_back(std::move(bullet));
            }

            // クールダウンを設定
            attackCooldown = 60 * 4;
        } 
        else
        {
            --attackCooldown; // クールダウンを減少
        }
    }
}

void Enemy::OnCollisionTrigger(const Collider* _other)
{
}

void Enemy::EnemyCollision(Vector3 _position)
{
}
