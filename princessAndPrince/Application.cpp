#include "Application.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "SceneTitle.h"
#include "UserData.h"
#include "MyString.h"
#include "resource.h"

#include "Pad.h"

#include <DxLib.h>
#include <assert.h>

namespace
{
	constexpr int kScreenWidth = 1600;
	constexpr int kScreenHeight = 960;
	constexpr int kArraySize = 81;
	constexpr int kColorBiteDepth = 32;
	constexpr int kFontSize = 48;
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
	SetWindowText("�P�ƃT��");
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	ChangeWindowMode(false);	

	m_isWindowSize = { kScreenWidth,kScreenHeight };


	SetGraphMode(m_isWindowSize.w, m_isWindowSize.h, kColorBiteDepth);
	SetWindowIconID(111);

	if (DxLib_Init() == -1) {	//�������Ɏ��s������A�v���𗎂Ƃ�
		return false;	//�ُ�I��
	}
	SetDrawScreen(DX_SCREEN_BACK);

	m_fontPath = "data/PixelMplus12-Regular.ttf";
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}
	ChangeFont("PixelMplus12");
	SetFontSize(kFontSize);
	return true;
}

void Application::Run()
{
	{
		SceneManager sceneManager;
		DataManager DataManager;
		DataManager.Init();
		sceneManager.SetStartScene(std::make_shared<SceneTitle>(sceneManager, DataManager, 0));

		Pad pad;

		while (ProcessMessage() != -1)
		{
			// ���̃t���[���̊J�n�������o���Ă���
			LONGLONG start = GetNowHiPerformanceCount();
			ClearDrawScreen();
			pad.Update();
			sceneManager.Update(pad);
			sceneManager.Draw();
			// FPS60�ɌŒ肷��
			while (GetNowHiPerformanceCount() - start < 16667)
			{
				// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
			}
			ScreenFlip();
		}
	}

	SetSaveData(UserData::userSaveDataNum);

	Terminate();

}

const Size& Application::GetWindowSize() const
{
	return m_isWindowSize;
}

void Application::SetSaveData(int num)
{
	if (num == 1)
	{
		std::string tempS;
		ofstream outputfile("./data/save/firstSaveData.txt");
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
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
	if (num == 2)
	{
		std::string tempS;
		ofstream outputfile("./data/save/secondSaveData.txt");
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
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
	if (num == 3)
	{
		std::string tempS;
		ofstream outputfile("./data/save/thirdSaveData.txt");
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
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
	if (num == 4)
	{
		std::string tempS;
		ofstream outputfile("./data/save/fourthSaveData.txt");
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
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
}
