#include "scene/base/MyGame.h"

//class ResourceObject
//{
//public:
//	ResourceObject(ID3D12Resource* resource)
//		:resource_(resource)
//	{}
//
//	~ResourceObject()
//	{
//		if (resource_)
//		{
//			resource_->Release();
//		}
//	}
//
//	ID3D12Resource* Get() { return resource_; }
//
//private:
//	ID3D12Resource* resource_;
//};


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Framework* game = new MyGame();	

	game->Run();

	delete game;

	return 0;
}