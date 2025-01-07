#include "GameClearScene.h"

void GameClearScene::Initialize()
{
	for (uint32_t i = 0; i < 2; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0 || i == 3) {
			sprite->Initialize("Clear/BG.png", { 0,0 }, { 1,1,1,1 }, { 0,0 }); 
		}
		else {
			sprite->Initialize("Clear/goTitle.png", { 10,10 }, { 1,1,1,1 }, { 0,0 });
		}
		sprites.push_back(sprite);

		Vector2 size = sprite->GetSize();
		
		sprite->SetSize(size);
	}
}

void GameClearScene::Finalize()
{
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}
}

void GameClearScene::Update()
{
	for (Sprite* sprite : sprites)
	{
		sprite->Update();
	}

	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}
}
