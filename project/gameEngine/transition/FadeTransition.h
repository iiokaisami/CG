#pragma once

#include <functional>
#include <memory>
#include <Sprite.h>

#include "BaseTransition.h"

/// <summary>
/// フェードトランジション
/// フェードインのみ or フェードアウト→シーン切り替え→フェードイン
/// </summary>
class FadeTransition : public BaseTransition
{
public:

	enum class Mode { FadeInOnly, Full };

	FadeTransition(Mode mode = Mode::Full);

	//  開始(シーン切り替え時に呼び出す)
	void Start(std::function<void()> onSceneChange)override;
	
	//  更新
	void Update() override;
	
	//  描画
	void Draw() override;

public: // ゲッター

	bool IsFinished() const override;

private:

	std::unique_ptr<Sprite> fadeSprite_ = nullptr;

	Mode mode_;

	enum class State { FadeOut, Hold, FadeIn, Done };
	State state_ = State::FadeOut;
	float alpha_ = 0.0f;
	float speed_ = 0.02f;
	std::function<void()> onSceneChange_;

};

