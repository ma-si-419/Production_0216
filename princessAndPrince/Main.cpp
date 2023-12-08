#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	Application& application = Application::GetInstance();
	if (!application.Init())
	{
		return -1;
	}
	application.Run();



	return 0;	//ê≥èÌèIóπ
}