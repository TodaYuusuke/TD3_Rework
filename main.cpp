#include "Game/Scenes/Test/TestScene.h"
#include "Game/Scenes/GamePlay/GamePlayScene.h"
#include "Game/Scenes/Title/TitleScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp("TD3");
#ifdef DEMO
#else
	LWP::Window::ChangeFullScreenMode();
#endif
	LWP::System::Run(new TestScene());
	return 0;
}