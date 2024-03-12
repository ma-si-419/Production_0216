#pragma once
#include "Scene.h"
class SceneManager;
class SceneTitle : public Scene 
{
public:
	//�R���X�g���N�^
	SceneTitle(SceneManager& sceneManager,DataManager& DataManager,int selectSceneNum);
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
	//bgm
	int m_bgm;
	//�w�i�̃O���t�B�b�N
	int m_bgGraph;
	//�^�C�g���̃O���t�B�b�N
	int m_titleGraph;
	//�_�ł�����Ƃ��ɃJ�E���g����
	int m_count;
	//�������o�����ǂ���
	bool m_isString;
	//�Z���N�g�V�[���őI��ł����V�[����ۑ�����
	int m_selectSceneNum;
	//�v���C���[�̃O���t�B�b�N
	int m_playerGraph;
	//�G�l�~�[�̃O���t�B�b�N
	int m_enemyGraph;
	//�v�����Z�X�̃O���t�B�b�N
	int m_princessGraph;
	int m_changeWindowModeGraph;
};

