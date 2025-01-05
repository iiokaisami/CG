#include "GameClearScene.h"

void GameClearScene::Initialize()
{
	for (uint32_t i = 0; i < 1; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0 || i == 3) {
			sprite->Initialize("monsterBall.png", { 0,0 }, { 1,1,1,1 }, { 0,0 }); 
		}
		else {
			sprite->Initialize("uvChecker.png", { 10,10 }, { 1,1,1,1 }, { 0,0 });
		}
		sprites.push_back(sprite);

		Vector2 size = sprite->GetSize();
		size.x = 370.0f;
		size.y = 370.0f;
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

	if (Input::GetInstance()->TriggerKey(DIK_P))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{
	for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}
}
