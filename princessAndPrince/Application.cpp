#include "Application.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "Pad.h"

#include <DxLib.h>
#include <assert.h>

namespace
{
	constexpr int screen_width = 960;
	constexpr int screen_height = 800;
}

void Application::Terminate()
{
	DxLib_End();
}

bool Application::Init()
{
	windowSize_ = { screen_width,screen_height };

	ChangeWindowMode(true);	//非スクリーン
	SetGraphMode(windowSize_.w, windowSize_.h, 32);

	if (DxLib_Init() == -1) {	//初期化に失敗したらアプリを落とす
		return false;	//異常終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

void Application::Run()
{
	{
		SceneManager sceneManager;
		sceneManager.ChangeScene(std::make_shared<SceneTitle>(sceneManager));

		Pad pad;

		while (ProcessMessage() != -1) 
		{
			// このフレームの開始時刻を覚えておく
			LONGLONG start = GetNowHiPerformanceCount();
			ClearDrawScreen();

			pad.Update();
			sceneManager.Update(pad);
			sceneManager.Draw();
			// FPS60に固定する
			while (GetNowHiPerformanceCount() - start < 16667)
			{
				// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
			}
			ScreenFlip();
		}
	}
	Terminate();
}

const Size& Application::GetWindowSize() const
{
	return windowSize_;
}
