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
	m_isScreen = true;
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	ChangeWindowMode(m_isScreen);	//非スクリーン

	m_isWindowSize = { kScreenWidth,kScreenHeight };

	m_isPush = true;

	SetGraphMode(m_isWindowSize.w, m_isWindowSize.h, 32);

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
	ChangeFont("PixelMplus12");
	SetFontSize(48);
	return true;
}

void Application::Run()
{
	{
		SceneManager sceneManager;
		DataManager DataManager;
		DataManager.Init();
		sceneManager.SetStartScene(std::make_shared<SceneTitle>(sceneManager, DataManager));

		Pad pad;

		while (ProcessMessage() != -1)
		{
			// このフレームの開始時刻を覚えておく
			LONGLONG start = GetNowHiPerformanceCount();
			ClearDrawScreen();
			if (CheckHitKey(KEY_INPUT_F1) && m_isPush)
			{
				m_isScreen = !m_isScreen;
				ChangeWindowMode(m_isScreen);
				DataManager.Init();
				m_isPush = false;
			}
			else if (!CheckHitKey(KEY_INPUT_F1))
			{
				m_isPush = true;
			}
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
	return m_isWindowSize;
}
