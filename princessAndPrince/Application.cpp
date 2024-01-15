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
	DxLib_End();
}

bool Application::Init()
{
	windowSize_ = { kScreenWidth,kScreenHeight };

	ChangeWindowMode(true);	//��X�N���[��
	SetGraphMode(windowSize_.w, windowSize_.h, 32);

	if (DxLib_Init() == -1) {	//�������Ɏ��s������A�v���𗎂Ƃ�
		return false;	//�ُ�I��
	}
	SetDrawScreen(DX_SCREEN_BACK);
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
	Terminate();
}

const Size& Application::GetWindowSize() const
{
	return windowSize_;
}
