#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "BaseTransition.h"
#include "Sprite.h"

/// <summary>
/// ブロックが下から上にせり上がるトランジション
/// </summary>
class BlockRiseTransition : public BaseTransition
{
public:

    enum class Mode { Full, DropOnly};

    BlockRiseTransition(Mode mode = Mode::Full);

	//  開始(シーン切り替え時に呼び出す)
    void Start(std::function<void()> onSceneChange) override;
    
	//  更新
    void Update() override;
    
	//  描画
    void Draw() override;
    
public: // ゲッター

	// トランジションが終了したか
    bool IsFinished() const override;

private:

    struct Block
    {
        std::unique_ptr<Sprite> sprite;
        Vector2 position;
        Vector2 scale;
        float rotation;
        float delay;
        float dropDelay;
        bool active;
        float fallSpeed;
        float verticalOffset;
        bool isRising;
        bool isDropped;
        float riseTimer;
        float riseDuration;
    };

    std::vector<Block> blocks_;

    enum class State { Fill, Hold, Drop, Done };
    State state_ = State::Fill;

    float timer_ = 0.0f;
    float fillSpeed_ = 4.0f;
    float blockInterval_ = 0.04f;
    uint32_t rows_ = 8;
    uint32_t cols_ = 14;
    float blockSize_ = 1280.0f / 14.0f;

    Mode mode_;
    std::function<void()> onSceneChange_;
    bool sceneChanged_ = false;

};

