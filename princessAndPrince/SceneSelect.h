#pragma once
#include "Scene.h"
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& manager);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();
private:
	//���I�����Ă���V�[����ۑ�����
	int m_selectNum;
	//�A���ŃL�[��������ĂȂ��悤��
	bool m_isKeyDown;
	//�J�[�\�����A���ňړ����Ȃ��悤��
	bool m_isSelectKeyDown;
};

