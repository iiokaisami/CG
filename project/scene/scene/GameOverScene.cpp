#include "GameOverScene.h"

void GameOverScene::Initialize()
{
	for (uint32_t i = 0; i < 3; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0) {
			sprite->Initialize("Gameover/BG.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else if(i == 1){
			sprite->Initialize("Gameover/goTitle.png", { 10,10 }, { 1,1,1,1 }, { 0,0 });
		}
		else {
			sprite->Initialize("black.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		sprites.push_back(sprite);

		Vector2 size = sprite->GetSize();
		
		sprite->SetSize(size);
	}

	isSceneStart_ = true;
	isSceneChange_ = false;
	alpha_ = 1.0f;

	// --- サウンド ---
	soundData_ = Audio::GetInstance()->LoadWav("gameOver.wav");
	Audio::GetInstance()->PlayWave(soundData_, false, 0.6f);
}

void GameOverScene::Finalize()
{
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}

	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData_);
}

void GameOverScene::Update()
{
	for (Sprite* sprite : sprites)
	{
		sprite->Update();
	}

	// シーンスタート
	if (isSceneStart_ && alpha_ > 0.1f)
	{
		alpha_ -= 0.01f;

		if (alpha_ <= 0.1f)
		{
			isSceneStart_ = false;
		}
	}
	// フェードスタート
	if (Input::GetInstance()->TriggerKey(DIK_RETURN) && !isSceneStart_)
	{
		isSceneChange_ = true;
	}
	// フェード
	if (isSceneChange_)
	{
		alpha_ += 0.01f;

		// 画面隠れたらシーン切り替え
		if (alpha_ >= 1.1f)
		{
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}

	sprites[2]->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
}

void GameOverScene::Draw()
{
	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	for (Sprite* sprite : sprites)
	{
		sprite->Draw();
	}
}
