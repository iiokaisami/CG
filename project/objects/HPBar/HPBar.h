#pragma once

#include "../GameObject/GameObject.h"

#include <Object3d.h>
#include <string>
#include <Vector3.h>
#include <memory>

class HPBar : public GameObject
{
public:

	HPBar() = default;
	~HPBar() = default;
	
	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新処理
	void Update() override;
	
	// 描画処理
	void Draw() override;

	void ImGuiDraw();

	void LoadBar(const std::string _textureFilePath,Vector3 _position);

public:

	void SetRatio(float _ratio) { ratio_ = _ratio; }

	void SetPosition(Vector3 _position) { position_ = _position; }

private:

	std::unique_ptr<Object3d> barObject_ = nullptr;
	float ratio_ = 1.0f;

	std::string textureFilePath_;
};

