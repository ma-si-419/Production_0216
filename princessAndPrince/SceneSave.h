#pragma once
#include "Scene.h"
class SceneSave : public Scene
{
private:
	enum dataKind
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

	//���[�h�����f�[�^�ɂ���ĕ\�����镶����ς���
	void DrawSceneName(int sceneNum);
	//���[�h�����f�[�^�ɂ���ĕ\�����鐔�l��ς���
	int GetDrawData(int selectNum, dataKind kind);
	//���l���E�����ɂ���
	int AlignmentRight(int num);
	//�Z�[�u�f�[�^�̃t���[����\������
	void DrawFrame(int selectNum);
	//�Z�[�u�f�[�^���Z�b�g����
	void SetSaveData(int selectNum);
	

private:
	
	//�w�i�̃O���t�B�b�N
	int m_bgGraph;
	//�t���[���̃O���t�B�b�N
	int m_frameGraph;
	//�O�̃t���[���ɃL�[�������Ă�����
	bool m_isLastKey;
	//���ǂ̃Z�[�u�f�[�^��I�����Ă��邩
	int m_selectSaveNum;
	//�Z�[�u�f�[�^�̃E�B���h�E��\�����Ă��邩
	bool m_isShowSaveWindow;
	//�E�B���h�E�̂ǂ̍��ڂ�I�����Ă��邩
	int m_selectWindowNum;
	//�f�[�^�폜��I�񂾂��ǂ���
	bool m_isDeleteData;
	//�f�[�^�폜�̑I����
	int m_selectDeleteNum;
	//���[�h�����f�[�^(���)
	int m_firstLoadData[10];
	//���[�h�����f�[�^(���)
	int m_secondLoadData[10];
	//���[�h�����f�[�^(�O��)
	int m_thirdLoadData[10];
	//�t���[���g�k�̑傫��
	float m_frameRatioX;
	float m_frameRatioY;
	//�t���[�����g�債�Ă��邩�ǂ���
	bool m_isZoom;

};

