#pragma once

#include <Framework.h>
#include <vector>

#include "../../scene/base/BaseScene.h"

class GameOverScene : public BaseScene
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

	// 2Dスプライト
	std::vector<Sprite*>sprites = {};

	// シーン切り替えフラグ
	bool isSceneStart_ = true;
	bool isSceneChange_ = false;
	float alpha_ = 0.0f;

	// サウンド
	SoundData soundData_;
};

