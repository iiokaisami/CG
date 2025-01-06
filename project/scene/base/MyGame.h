#pragma once

#include "../base/Framework.h"

#include "../scene/base/BaseScene.h"
#include "../scene/scene/GamePlayScene.h"
#include "../scene/scene/TitleScene.h"


class MyGame : public Framework
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


   /* CameraManager& cameraManager = CameraManager::GetInstance();
    std::shared_ptr<Camera> camera1 = std::make_shared<Camera>();
    std::shared_ptr<Camera> camera2 = std::make_shared<Camera>();
	uint32_t activeIndex = 0;
	Vector3 camera1Rotate{};
	Vector3 camera1Position{};
	Vector3 camera2Rotate{};
	Vector3 camera2Position{};*/

	

	/*std::vector<Object3d*>object3ds;

	std::vector<Sprite*> sprites;*/

	Vector2 position{};


	bool isFilipX = false;
	bool isFilipY = false;

	Vector2 textureLeftTop{};
	Vector2 textureSize{ 500.0f ,500.0f };
	
};

