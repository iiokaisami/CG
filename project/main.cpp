#include "scene/base/MyGame.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame game;

	// ゲームの初期化
	game.Initialize();

	//メインループ
	//ウィンドウのxボタンが押されるまでループ
	while (true)
	{
		// ゲームの更新
		game.Update();

		if (game.IsEndRequest())
		{
			break;
		}

		// 描画
		game.Draw();

	}

	// ゲームの終了
	game.Finalize();
	
	
	return 0;
}