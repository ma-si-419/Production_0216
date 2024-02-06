#include "Application.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "SceneTitle.h"
#include "UserData.h"
#include "MyString.h"

#include "Pad.h"

#include <DxLib.h>
#include <assert.h>

namespace
{
	constexpr int kScreenWidth = 1600;
	constexpr int kScreenHeight = 960;
	constexpr int kArraySize = 81;
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
	SetWindowText("姫と王子");
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
		sceneManager.SetStartScene(std::make_shared<SceneTitle>(sceneManager, DataManager, 0));
		int tempArray[8] = { 0,0,0,0,0,0,0,0 };
		//ファイルを開く
		std::ifstream ifs("./data/saveData.txt");
		int loopCount = 0;
		//帰ってきた値を返す配列
		vector<string> tempS;
		//配列を作成
		char str[kArraySize];
		//成功したら一行ずつ読み込む
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			tempArray[loopCount] = temp;
			loopCount++;
		}
		UserData::userGold = tempArray[0];
		UserData::userExp = tempArray[1];
		UserData::userMainLevel = tempArray[2];
		UserData::userAtkLevel = tempArray[3];
		UserData::userDefLevel = tempArray[4];
		UserData::userSpdLevel = tempArray[5];
		UserData::userFireLevel = tempArray[6];
		UserData::userWindLevel = tempArray[7];

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
	std::string tempS;
	ofstream outputfile("data/saveData.txt");
	tempS = to_string(UserData::userGold);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userExp);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userMainLevel);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userAtkLevel);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userDefLevel);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userSpdLevel);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userFireLevel);
	outputfile << tempS + "\n";
	tempS = to_string(UserData::userWindLevel);
	outputfile << tempS;

	Terminate();

}

const Size& Application::GetWindowSize() const
{
	return m_isWindowSize;
}
