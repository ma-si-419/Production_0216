#pragma once
#include "Scene.h"
#include "DxLib.h"
class SceneSave : public Scene
{
private:
	enum class dataKind
	{
		main,
		atk,
		def,
		spd,
		fire,
		wind,
		gold,
		exp,
		stage,
		dataNum
	};

public:
	SceneSave(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum);
	virtual ~SceneSave();

	void Init();
	void Update(Pad& pad);
	void Draw();

	//���[�h�����f�[�^�ɂ���ĕ\�����鐔�l��ς���
	int GetDrawData(int selectNum, dataKind kind);
	//���l���E�����ɂ���
	int AlignmentRight(int num);
	//�Z�[�u�f�[�^�̃t���[����\������
	void DrawFrame(int selectNum);
	//�Z�[�u�f�[�^���Z�b�g����
	void SetSaveData(int selectNum);
	//�Z�[�u�f�[�^�̃i���o�[��\������
	void DrawSaveDataNum();
	//�X�e�[�^�X��\������
	void DrawStatus();
	//�t�@�C�������[�h����
	void LoadFile();
	//�{�^�������͂���Ă��邩��Ԃ�
	bool GetPushButton(XINPUT_STATE& input);
	//�t���[���̊g�k������
	void ScalingFrame();
	//�J�[�\���𓮂���
	void MoveCursor(XINPUT_STATE& input);
	//�Z�[�u�f�[�^������
	void DeleteSaveData(int dataNum);

private:
	
	//�w�i�̃O���t�B�b�N
	int m_bgGraph;
	//�t���[���̃O���t�B�b�N
	int m_frameGraph;
	//�G�l�~�[�̊�摜
	int m_enemyHeadGraph;
	//�J�[�\���̉摜
	int m_cursorGraph;
	//�J�[�\���𓮂������Ƃ��̉�
	int m_cursorSe;
	//���肵�����̉�
	int m_appSe;
	//�L�����Z���������̉�
	int m_cancelSe;
	//�O�̃t���[���ɃL�[�������Ă�����
	bool m_isLastKey;
	//���ǂ̃Z�[�u�f�[�^��I�����Ă��邩
	int m_selectSaveNum;
	//�Z�[�u�f�[�^�̃E�B���h�E��\�����Ă��邩
	bool m_isShowSaveWindow;
	//�f�[�^�폜�̃E�B���h�E���o���Ă��邩�ǂ���
	bool m_isShowDeleteWindow;
	//�E�B���h�E�̂ǂ̍��ڂ�I�����Ă��邩
	int m_selectWindowNum;
	//�f�[�^�폜�̑I����
	int m_selectDeleteNum;
	//���[�h�����f�[�^(���)
	int m_firstLoadData[10];
	//���[�h�����f�[�^(���)
	int m_secondLoadData[10];
	//���[�h�����f�[�^(�O��)
	int m_thirdLoadData[10];
	//���[�h�����f�[�^(�l��)
	int m_fourthLoadData[10];
	//�t���[���g�k�̑傫��
	float m_frameRatioX;
	float m_frameRatioY;
	//�t���[�����g�債�Ă��邩�ǂ���
	bool m_isZoom;
	//BGM
	int m_bgm;
	//�V�[���ړ����邩�ǂ���
	bool m_isMoveScene;
	//�J�[�\���̗h�炷����
	float m_cursorShakeLange;
	//�J�[�\�����E�ɓ����Ă��邩�ǂ���
	bool m_isCursorMoveRight;
};

