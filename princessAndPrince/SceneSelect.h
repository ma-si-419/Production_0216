#pragma once
#include "Scene.h"
#include "Game.h"
class Player;
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& sceneManager,DataManager& DataManager);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();
private:
	//���I�����Ă���V�[����ۑ�����
	int m_stageSelectNum;
	//���I�����Ă��鍀�ڂ�ۑ�����
	int m_pauseSelectNum;
	//�A���ŃL�[��������ĂȂ��悤��
	bool m_isKeyDown;
	//�J�[�\�����A���ňړ����Ȃ��悤��
	bool m_isSelectKeyDown;
	//�|�[�Y���J���Ă��邩�ǂ���
	bool m_isPause;
	//�X�e�[�^�X��ʂ��J���Ă��邩�ǂ���
	bool m_isStatus;
	//�X�e�[�W�����肵����ɘA�ł��Ȃ��悤��
	bool m_isSelectScene;
	//����{�^�����������Ƃ��̌��ʉ�
	int m_appSe;
	//�J�[�\���𓮂������Ƃ��̌��ʉ�
	int m_cursorSe;
	//�L�����Z���{�^�����������Ƃ��̌��ʉ�
	int m_cancelSe;
	//�v���C���[�̃O���t�B�b�N
	int m_playerGraph;
	//Princess�̃O���t�B�b�N
	int m_princessGraph;
	//�w�i�̃O���t�B�b�N
	int m_bgGraph;
	//�I��ł���V�[���̔w�i
	int m_selectSceneBgGraph;
	//�v���C���[�������Ă������
	Game::Dir m_dir;
	//�v���C���[�̃A�j���[�V����
	int m_animFrame;
	//bgm
	int m_bgm;
};

