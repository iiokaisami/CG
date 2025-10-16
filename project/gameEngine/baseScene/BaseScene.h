#pragma once

class SceneManager;

/// <summary>
/// シーンの基底クラス
/// 継承して使用する
/// </summary>
class BaseScene
{
public:
    // デストラクタ
    virtual ~BaseScene() = default;

    // 初期化
    virtual void Initialize();

    // 終了
    virtual void Finalize();

    // 更新処理
    virtual void Update();

    // 描画処理
    virtual void Draw();

public:

    virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:

    // シーンマネージャ
    SceneManager* sceneManager_ = nullptr;
};