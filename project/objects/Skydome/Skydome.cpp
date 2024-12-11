#include "Skydome.h"

#include <ModelManager.h>

void Skydome::Initialize()
{
    /// モデルの読み込みと登録
    ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");

    /// Transformの初期化
    scale_ = { 300.0f, 300.0f, 300.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    object_->Initialize("skydome.obj");

    object_->SetScale(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);
}

void Skydome::Update()
{
    object_->Update();
}

void Skydome::Draw()
{
    object_->Draw();
}

void Skydome::Finalize()
{

}