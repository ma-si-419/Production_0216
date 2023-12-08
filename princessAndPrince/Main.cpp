#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// このフレームの開始時刻を覚えておく
	LONGLONG start = GetNowHiPerformanceCount();
	Application& application = Application::GetInstance();
	if (!application.Init())
	{
		return -1;
	}
	// FPS60に固定する
	while (GetNowHiPerformanceCount() - start < 16667)
	{
		// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
	}
	application.Run();


	return 0;	//正常終了
}