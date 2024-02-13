#pragma once
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
	//�\������{�X�����߂�
	int GetShowBossKind(int num);
	//�ς���A�C�e�������邩�ǂ���
	bool GetCanBuyItem();
	//�Z�[�u�V�[���ɖ߂�Ƃ��ɃZ�[�u����
	void SetSaveData(int num);
	//�X�e�[�W�̖��O��\������
	void DrawStageName(int num);
	//�V���b�v���g�傷��
	void ZoomShop();
	//�V�[���ƃL�����𓮂���
	void MoveCharcter();
	//�r�b�N���}�[�N�𓮂���
	void ZoomMark();
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
	//�X�g�[���[�̉摜�������Ă���z��
	int m_storyGraph[8];
	//�ǂ̃X�e�[�W�܂ŃN���A������
	int m_clearStageNum;
	//�v���C���[�ƃv�����Z�X�̍��W
	int m_charPosY;
	//�v���C���[�ƃv�����Z�X���X�e�[�W�̏�܂ōs�������ǂ���
	bool m_isCharArrTopEnd;
	//�v���C���[�ƃv�����Z�X���X�e�[�W�̉��܂ōs�������ǂ���
	bool m_isCharArrBottomEnd;
	//�v���C���[�ƃv�����Z�X�����ɓ����Ă��邩�ǂ���
	bool m_isCharMoveDown;
	//�e�X�e�[�W�̃{�X�̃O���t�B�b�N
	int m_enemyGraph;
	//�\������{�X
	int m_bossSrcX;
	int m_bossSrcY;
	//�{�X�̃A�j���[�V����
	int m_bossAnimFrame;
	//�{�X���o��������Ƃ��t�F�[�h�C���ŏo��������
	int m_bossAlpha;
	//�r�b�N���}�[�N
	int m_exclamationMarkGraph;
	//�����{�b�N�X
	int m_boxGraph;
	//�����{�b�N�X�̊g�嗦
	float m_boxRatio;
	//�����{�b�N�X�̊p�x
	float m_boxAngle;
	//�v���C���[�̃A�C�e���̒l�i
	int m_playerItemPriceList[21];
	//�v�����Z�X�̃A�C�e���̒l�i
	int m_princessItemPriceList[21];
	//�ς���A�C�e�������邩�ǂ���
	bool m_isBuy;
	//�r�b�N���}�[�N�̃|�W�V����
	float m_exclamationMarkRatio;
	//�r�b�N���}�[�N�𓮂�������
	float m_exclamationMarkZoomSpeed;
	//�O�p�`�𓮂������W
	float m_shakeTrianglePosY;
	//�O�p�`����ɓ������Ă��邩�ǂ���
	bool m_isMoveUpTriangle;
};
