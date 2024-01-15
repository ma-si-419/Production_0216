#include "Application.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "SceneTitle.h"

#include "Pad.h"

#include <DxLib.h>
#include <assert.h>

namespace
{
	constexpr int kScreenWidth = 1600;
	constexpr int kScreenHeight = 960;
}

void Application::Terminate()
{
	if (RemoveFontResourceEx(m_fontPath, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, "remove failure", "", MB_OK);
	}
	DxLib_End();
}

bool Application::Init()
{
	windowSize_ = { kScreenWidth,kScreenHeight };

	ChangeWindowMode(true);	//非スクリーン
	SetGraphMode(windowSize_.w, windowSize_.h, 32);

	if (DxLib_Init() == -1) {	//初期化に失敗したらアプリを落とす
		return false;	//異常終了
	}
	SetDrawScreen(DX_SCREEN_BACK);

	m_fontPath = "data/PixelMplus12-Regular.ttf";
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
	return true;
}

void Application::Run()
{
	{
		SceneManager sceneManager;
		DataManager DataManager;
		sceneManager.SetStartScene(std::make_shared<SceneTitle>(sceneManager,DataManager));

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
