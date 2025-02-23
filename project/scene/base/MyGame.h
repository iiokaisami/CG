#pragma once

#include "Framework.h"

#include "SceneFactory.h"

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

	// モデル読み込み
	void LoadModel();

	// サウンド読み込み
	void LoadSound();

	// サウンド
	SoundData soundData_;
	SoundData soundData2_;

	bool useExampleGroup_ = true;
};