#pragma once

#include "BaseTransition.h"

#include <memory>
#include <functional>

class TransitionManager
{

public:

	// トランジション開始
    void Start(std::unique_ptr<BaseTransition> transition, std::function<void()> onSceneChange);

	// 更新
    void Update();
    
	// 描画
    void Draw();

public: // ゲッター

	// トランジション中か
    bool IsRunning() const;

private:

	// 現在のトランジション
    std::unique_ptr<BaseTransition> transition_;

};

