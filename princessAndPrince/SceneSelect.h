#pragma once
#include "Scene.h"
#include "Game.h"
class Player;
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& sceneManager, DataManager& DataManager,int selectSceneNum);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();

	//�V�[���I�����̉��o
	void MoveScene(bool up);
	//�V�[����I��ł��鎞�ɏo�Ă���O�p�`
	void DrawSceneSrideTriangle();
private:
	//���I�����Ă���V�[����ۑ�����
	int m_stageSelectNum;
	//�A���ŃL�[��������ĂȂ��悤��
	bool m_isKeyDown;
	//�J�[�\�����A���ňړ����Ȃ��悤��
	bool m_isSelectKeyDown;
	//�X�e�[�W�����肵����ɘA�ł��Ȃ��悤��
	bool m_isSelectScene;
	//����{�^�����������Ƃ��̌��ʉ�
	int m_appSe;
	//���C���V�[���Ɉړ�����Ƃ��̉�
	int m_moveMainSceneSe;
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
	int m_shopSrcY;
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
	//�{�^����UI
	int m_buttonsUiGraph;
	//�V���b�v�{�^�������������ǂ���
	bool m_isShopButton;
	//�V���b�v�̕\�����W(�n�_�ƏI�_)
	float m_shopStartPosX;
	float m_shopStartPosY;
	int m_shopEndPosX;
	int m_shopEndPosY;
	//���C���V�[���Ɉړ����邩�ǂ���
	bool m_isMoveMainScene;
};
