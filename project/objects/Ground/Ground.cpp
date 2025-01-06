#include "Ground.h"

#include <ModelManager.h>

void Ground::Initialize()
{
    /// モデルの読み込みと登録
    ModelManager::GetInstance()->LoadModel("ground/ground.obj");

    /// Transformの初期化
    scale_ = { 1.0f, 1.0f, 1.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    object_->Initialize("ground.obj");

    object_->SetScale(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);
}

void Ground::Update()
{
    object_->Update();
}

void Ground::Draw()
{
    object_->Draw();
}

void Ground::Finalize()
{
}
