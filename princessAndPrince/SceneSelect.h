#pragma once
#include "Scene.h"
#include "Game.h"
class Player;
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& sceneManager, DataManager& DataManager);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();

	//�V�[���I�����̉��o
	void MoveScene(bool up);
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
	//�V���b�v�̃A�j���[�V����
	int m_shopAnimFrame;
	//�V���b�v�̃A�j���[�V������؂�����W
	int m_shopSrcX;
	//bgm
	int m_bgm;
	//�w�i�摜��؂�����W
	int m_cutBgPosY;
	//���o�̊ԓ������Ȃ��悤�ɂ���
	bool m_isStaging;
	//�V�[�����オ���Ă��邩�������Ă��邩
	bool m_isSceneUp;
	//�V���b�v�̃O���t�B�b�N
	int m_shopGraph;
	//�I��ł���V�[���̃t���[��
	int m_selectSceneFrame;
	//�X�e�[�W���̕\�L
	int m_backBoxGraph;
	//�X�e�[�W�ύX���������ǂ���
	bool m_isChangeStage;
};
