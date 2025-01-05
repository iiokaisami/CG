#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	for (uint32_t i = 0; i < 1; ++i)
	{
		Sprite* sprite = new Sprite();
		if (i == 0 || i == 3) {
			sprite->Initialize("uvChecker.png", { 0,0 }, { 1,1,1,1 }, { 0,0 });
		}
		else {
			sprite->Initialize("monsterBall.png", { 10,10 }, { 1,1,1,1 }, { 0,0 });
		}
		sprites.push_back(sprite);

		Vector2 size = sprite->GetSize();
		size.x = 370.0f;
		size.y = 370.0f;
		sprite->SetSize(size);
	}

	// Player
	pPlayer_ = std::make_unique<Player>();
	pPlayer_->Initialize();
	pPlayer_->SetCamera(cameraManager.GetActiveCamera());

	// Enemy
	pEnemy_ = std::make_unique<Enemy>();
	pEnemy_->Initialize();
	pEnemy_->SetPlayerPosition(pPlayer_->GetPosition());

	// Skydome
	pSkydome_ = std::make_unique<Skydome>();
	pSkydome_->Initialize();


}

void GamePlayScene::Finalize()
{
	for (Sprite* sprite : sprites) 
	{
		delete sprite;
	}

	pPlayer_->Finalize();
	pEnemy_->Finalize();
	pSkydome_->Finalize();
}

void GamePlayScene::Update()
{
	for (Sprite* sprite : sprites)
	{
		sprite->Update();
		//sprite->SetPosition(position);
		//sprite->SetFlipX(isFilipX);
		//sprite->SetFlipY(isFilipY);
		//sprite->SetTextureLeftTop(textureLeftTop);
		//sprite->SetTextureSize(textureSize);
	}

	pPlayer_->Update();
	pEnemy_->SetPlayerPosition(pPlayer_->GetPosition());
	pEnemy_->Update();
	pSkydome_->Update();

	//pSkydome_->SetCamera(cameraManager.GetActiveCamera());


	if (Input::GetInstance()->TriggerKey(DIK_P))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}

	if (Input::GetInstance()->TriggerKey(DIK_O))
	{
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}

void GamePlayScene::Draw()
{
	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->CommonDrawSetting();

	// 描画前処理(Object)
	Object3dCommon::GetInstance()->CommonDrawSetting();

	//for (Sprite* sprite : sprites)
	//{
	//	//sprite->Draw(textureSrvHandleGPU);
	//}
	 
	
	// 天球描画
	pSkydome_->Draw();
	// プレーヤー描画
	pPlayer_->Draw();
	// 敵描画
	pEnemy_->Draw();
}
