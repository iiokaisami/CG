#pragma once

#include <Framework.h>
#include <vector>
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
	//std::vector<Object3d*> object3ds = {};

	// 透明チェック
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	// サウンド
	SoundData soundData_;
	SoundData soundData2_;

	// シーン切り替えフラグ
	bool isSceneChange_ = false;
	bool isScreenHide_ = false;
	Vector3 startPosition_ = { -1280.0f,0.0f,0.0f };
	Vector3 endPosition_ = { 0.0f,0.0f,0.0f };
	Vector3 movePosition_{};
	float t = 0.0f;
};

