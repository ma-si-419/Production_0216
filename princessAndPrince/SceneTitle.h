#pragma once
#include "Scene.h"
class SceneManager;
class SceneTitle : public Scene 
{
public:
	//�R���X�g���N�^
	SceneTitle(SceneManager& manager);
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
};

