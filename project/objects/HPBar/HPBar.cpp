#include "HPBar.h"

#include <ModelManager.h>

void HPBar::Initialize()
{
	barObject_ = std::make_unique<Object3d>();
	barObject_->Initialize(textureFilePath_);

	barObject_->SetPosition(position_);
}

void HPBar::Finalize()
{
}

void HPBar::Update()
{
	Vector3 newScale = scale_;
	newScale.x *= ratio_;
	barObject_->SetScale(newScale);
	barObject_->SetPosition(position_);
	barObject_->SetRotate(rotation_);
	barObject_->Update();
}

void HPBar::Draw()
{
	barObject_->Draw();
}

void HPBar::ImGuiDraw()
{
	ImGui::Begin("HPBar");
	ImGui::SliderFloat3("position", &position_.x, -100.0f, 100.0f);
	ImGui::End();
}

void HPBar::LoadBar(const std::string _textureFilePath, Vector3 _position)
{
	ModelManager::GetInstance()->LoadModel(_textureFilePath);

	textureFilePath_ = _textureFilePath;
	position_ = _position;
}
