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
};

