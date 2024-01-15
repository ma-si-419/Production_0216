#pragma once
#include "Scene.h"
class SceneManager;
class SceneTitle : public Scene 
{
public:
	//�R���X�g���N�^
	SceneTitle(SceneManager& sceneManager,DataManager& DataManager);
	//�f�X�g���N�^
	virtual ~SceneTitle();
	//����������
	void Init();
	//�X�V����
	void Update(Pad& pad);
	//�`�揈��
	void Draw();
private:
	//�^�C�g���摜
	int m_handle;
	//�A���ŃL�[��������Ȃ��悤�ɂ��邽�߂̕ϐ�
	bool m_isKeyDown;
	//�{�^�����������Ƃ��̌��ʉ�
	int m_appSe;
};

