#include "Enemy.h"

#include <ModelManager.h>

void Enemy::Initialize()
{
    // --- 3Dオブジェクト ---
    ModelManager::GetInstance()->LoadModel("enemy/Enemy.obj");

    object_ = std::make_unique<Object3d>();
    object_->Initialize("Enemy.obj");

    moveVelocity_ = { 0.1f,0.1f,0.0f };
    
	object_->SetPosition(position_);
    object_->SetRotate(rotation_);

    // 仮置き
	scale_ = { 0.0f,0.0f,0.0f };
	object_->SetScale(scale_);

    collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Enemy";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
    collisionManager_->RegisterCollider(&collider_);

    hp_ = 5;
	isDead_ = false;

	// hpBar
	hpBar_ = std::make_unique<HPBar>();
    hpBar_->LoadBar("EnemyHPBar.obj", { position_.x,position_.y - 0.1f,position_.z });
	hpBar_->Initialize();
	hpBar_->SetScale({ 1.0f,0.1f,0.1f });

    // --- サウンド ---
    soundData_ = Audio::GetInstance()->LoadWav("enemyDamage.wav");
}

void Enemy::Finalize()
{
    // 各解放処理
    Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData_);

    collisionManager_->DeleteCollider(&collider_);
}

void Enemy::Update()
{
	hpBar_->Update();
    object_->Update();

    // 敵弾から自キャラへのベクトルを計算
    toPlayer_ = playerPosition_ - position_;

    // ノックバックする間移動出来ない
    if (!(hitInterval_ > 0))
    {
        // ベクトルを正規化する
        toPlayer_ = Normalize(toPlayer_);
        moveVelocity_ = Normalize(moveVelocity_);
        // 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
        moveVelocity_ = 1.0f * (Slerp(moveVelocity_, toPlayer_, 0.1f));

        // 進行方向に見た目の回転を合わせる
        // Y軸周り角度(θy)
        if (!isDeadMotion_)
        {
            rotation_.y = std::atan2(moveVelocity_.x, moveVelocity_.z);
            rotation_.x = 0.0f;
        }

        moveVelocity_ /= 20.0f;
    }
    else if (!isDeadMotion_)
    {
        rotation_.y = std::atan2(-moveVelocity_.x, -moveVelocity_.z);
    }

  

    // 座標を移動させる(フレーム分の移動量を足しこむ)
    if (!isPopMotion_)
    {
		moveVelocity_.y = 0.0f;
        position_ += moveVelocity_;
    }

    if (hitInterval_ > 0)
    {
        hitInterval_--;
    }

	// ポップモーション
    if (isPopMotion_)
    {
		scale_ = Lerp(scale_, popScale_, 0.1f);

        if (scale_.y >= 0.7f)
        {
            isPopMotion_ = false;
        }
	}

	// デスモーション
    if (isDeadMotion_)
    {
        scale_ = Lerp(scale_, deadScale_, 0.1f);
		rotation_.y += 0.3f;

        //デス
        if (scale_.y <= 0.1f)
        {
            isDead_ = true;
        }
    }
   

    // モデルに座標をセット
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);
    object_->SetScale(scale_);

	// hpBarの位置をセット
	hpBar_->SetRatio((float)(hp_ + 0.1f) / 10.0f);
    hpBar_->SetPosition({ position_.x,position_.y + 1.5f,position_.z });
    hpBar_->SetRotation(rotation_);

    aabb_.min = position_ - object_->GetScale();
    aabb_.max = position_ + object_->GetScale();
    aabb_.max.y += 1.0f;
    collider_.SetPosition(position_);
}

void Enemy::Draw()
{
    object_->Draw();

    if (!isPopMotion_ && !isDeadMotion_)
    {
        hpBar_->Draw();
    }
}

void Enemy::Draw2d()
{
}

void Enemy::ImGuiDraw()
{
	ImGui::Begin("Enemy");

	ImGui::SliderFloat3("rotate", &rotation_.x, -10.0f, 10.0f);
    ImGui::SliderFloat3(" scale", &scale_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("pos", &position_.x, -200.0f,200.0f);

	ImGui::End();

	hpBar_->ImGuiDraw();
}

void Enemy::Attack()
{
}

void Enemy::EnemyCollision(Vector3 _position)
{
	// 自分の位置から相手の位置のベクトル
	Vector3 toEnemy = _position - position_;
	
    if (toEnemy.x > toEnemy.z)
    {
		moveVelocity_.x = -toEnemy.x / 5;
        position_ += moveVelocity_;
		object_->SetPosition(position_);
    }
    else
    {
		moveVelocity_.z = -toEnemy.z / 5;
        position_ += moveVelocity_;
		object_->SetPosition(position_);
    }

	object_->SetPosition(position_);
}

void Enemy::OnCollision(const Collider* _other)
{
    if (_other->GetColliderID() == "Enemy")
    {
        EnemyCollision(_other->GetPosition());
    }
	else if (_other->GetColliderID() == "Player")
	{
        moveVelocity_ = -toPlayer_;
	}
    else if (hp_ > 0 && _other->GetColliderID() != "Player")
    {
		isHit_ = true;
        moveVelocity_.x = -toPlayer_.x;
        moveVelocity_.z = -toPlayer_.z;
		hitInterval_ = 7;
        hp_ -= 1;

        Audio::GetInstance()->PlayWave(soundData_, false, 0.2f);
    }
	else if (hp_ <= 0)
    {  
		isDeadMotion_ = true;
    }
}
