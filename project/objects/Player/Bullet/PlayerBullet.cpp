#include "PlayerBullet.h"

#include <ModelManager.h>

void PlayerBullet::Initialize()
{

	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("cube.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");


	// 仮置き
	object_->SetScale({ 0.2f,0.2f,0.2f });

	object_->SetPosition(position_);

}

void PlayerBullet::Finalize()
{
	// 各解放処理

}

void PlayerBullet::Update()
{
	object_->Update();

	object_->SetPosition(position_);

	position_ += velocity_;


	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	object_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

void PlayerBullet::RunSetMask()
{
	//collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "Player"));
}
