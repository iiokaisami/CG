#pragma once

#include <functional>

/// <summary>
/// シーン切り替え時のトランジション処理を管理するための基底クラス
/// 継承して使用する
/// </summary>
class BaseTransition
{
public:

	virtual ~BaseTransition() = default;

	//  開始(シーン切り替え時に呼び出す)
	virtual void Start(std::function<void()> onSceneChange) = 0;

	//  更新
	virtual void Update() = 0;

	//  描画
	virtual void Draw() = 0;

public: // ゲッター

	// トランジションが終了したか
	virtual bool IsFinished() const = 0;

};

