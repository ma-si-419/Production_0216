#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ���̃t���[���̊J�n�������o���Ă���
	LONGLONG start = GetNowHiPerformanceCount();
	Application& application = Application::GetInstance();
	if (!application.Init())
	{
		return -1;
	}
	// FPS60�ɌŒ肷��
	while (GetNowHiPerformanceCount() - start < 16667)
	{
		// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
	}
	application.Run();


	return 0;	//����I��
}