#pragma once

#include <Framework.h>
#include <vector>
#include <memory>
#include <MyMath.h>
#include <Object3d.h>

#include "../base/BaseScene.h"

class TitleScene : public BaseScene
{
public:

	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;

private:

	CameraManager& cameraManager = CameraManager::GetInstance();
	std::shared_ptr<Camera> camera_ = std::make_shared<Camera>();

	// 2Dスプライト
	std::vector<Sprite*>sprites = {};

	// 3Dオブジェクト
	std::vector<Object3d*> object3ds = {};

	Vector3 position_ = { 0.0f,0.0f,-10.0f };

	// 透明チェック
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	// サウンド
	SoundData soundData_;
	SoundData soundData2_;
};

