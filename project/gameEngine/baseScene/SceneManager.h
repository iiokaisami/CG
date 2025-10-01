#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager
{
#pragma region シングルトンインスタンス
private:
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator = (SceneManager&) = delete;

public:
	// シングルトンインスタンスの取得
	static SceneManager* GetInstance();
	// 終了
	void Finalize();
#pragma endregion シングルトンインスタンス
public:

	// 更新
	void Update();

	// 描画
	void Draw();

	//　次シーン予約
	void ChangeScene(const std::string& sceneName);

	// シーンファクトリーを設定
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:

	// 今のシーン
	BaseScene* scene_ = nullptr;

	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

