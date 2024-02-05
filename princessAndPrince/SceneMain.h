#pragma once
#include"Scene.h"
#include <vector>
#include<memory>
#include<stack>
#include"UserData.h"
using namespace std;

class Player;
class Princess;
class Vec2;
class Enemy;
class ItemBase;
class UI;
class MagicBase;
class TreasureBox;
class CircleCol;
class Particle;
struct popEnemy
{
	int enemyKind;
	float popTime;
};
class SceneMain : public Scene
{
public:
	SceneMain(SceneManager& sceneManager, DataManager& DataManager, int stageNum);
	virtual ~SceneMain();

	void Init();

	void Update(Pad& pad);
	void Draw();

	/// <summary>
	/// SceneMain�ɂ���A�C�e���̔z���pItem��ǉ������ǉ�����
	/// </summary>
	/// <param name="pItem">�ǉ�����A�C�e��</param>
	/// <returns>���ɈӖ��͂Ȃ�</returns>
	bool AddItem(std::shared_ptr<ItemBase> pItem);
	//SceneMain�ɖ��@���o���֐�
	bool AddMagic(MagicBase* pMagic);
	//SceneMain�ɕ󔠂��o���֐�
	bool AddTreasure(TreasureBox* pTreasure);
	//SceneMain�Ƀp�[�e�B�N�����o���֐�
	bool AddParticle(Particle* pParticle);
	//�����蔻��𔻒肷��֐�
	bool IsCollision(CircleCol col1, CircleCol col2);
	//�G�𐶐�����֐�
	bool CreateEnemy(int enemyKind);
	//�|�����{�X�̐��𐔂���
	void CountKillBoss() { m_killBossCount++; };
	//�������[�h�̃Q�[�W�����߂�
	void AddSpecialGauge(float gauge);
	//�������[�h�̃Q�[�W�̂��܂��Ă��銄����Ԃ�
	float GetSpecialGaugeRate() { return 100 / m_specialGauge; }
	//�������[�h�𔭓����Ă��邩�ǂ�����Ԃ�
	bool GetSpecialMode() { return m_isSpecialMode; }
	//�|�[�Y��ʂ��ǂ������擾����
	bool GetPause() { return m_isPause; }
	//���o���I������^�C�~���O
	void SetEnd() { m_isResult = true; }
	//���U���g�̕�����\������^�C�~���O
	void ShowResult() { m_isClearString = true; }
	//�������擾����
	int GetDigits(int num);
	//���U���g��GOLD�̃��[�v���J�n����
	void StartExpLoop() { m_isExpLoop = true; }
	//���ʂ�ݒ肷��
	void ChangeSoundVol(int volume);
	//���̃��x���܂ł̌o���l�ʂ�Ԃ�
	int GetNextExp();
	//�R�C���̌��ʉ���Ԃ�
	int GetCoinSe() { return m_coinSe; };
	//�o���l�̌��ʉ���Ԃ�
	int GetExpSe() { return m_expSe; };
	//�����@�̌��ʉ���Ԃ�
	int GetWindMagicSe() { return m_windMagicSe; }
	//�����@�̌��ʉ���Ԃ�
	int GetFireMagicSe() { return m_fireMagicSe; }
	//�|�[�V�����̌��ʉ���Ԃ�
	int GetPortionSe() { return m_portionSe; }
	//�����E�������̌��ʉ���Ԃ�
	int GetBloodSe() { return m_bloodSe; }
	//����n�����Ƃ��̌��ʉ���Ԃ�
	int GetPassBloodSe() { return m_passBloodSe; }
	//�{�^���̃O���t�B�b�N��Ԃ�
	int GetButtonsGraph() { return m_buttonsGraph; }
	//�I�΂ꂽ�V�[���ɍ��킹�ăG�l�~�[�̏�������
	void SetEnemyInfo(int stageNum);
	//�I�΂ꂽ�V�[������ۑ�����
	int GetSceneNum() { return m_selectScene; }
	//Ui��\������w�i���擾����
	int GetUiBg() { return m_uiBgGraph; }
	//�o�Ă���{�X�̐���ݒ肷��
	void SetBossVol(int stageNum);

private:
	// �O���t�B�b�N�̃n���h��
	int m_playerHandle;
	int m_princessHandle;
	int m_enemyHandle;
	int m_bgHandle;
	int m_itemHandle;
	// �v���C���[
	Player* m_pPlayer;
	//���P�l
	Princess* m_pPrincess;
	//UI
	UI* m_pUi;
	//�p�[�e�B�N��
	Particle* m_pParticle;

	// �G
	vector<shared_ptr<Enemy>> m_pEnemy;
	//�A�C�e��
	vector<shared_ptr<ItemBase>> m_pItem;
	//���@
	vector<MagicBase*> m_pMagic;
	//��
	vector<TreasureBox*> m_pTreasure;
	//�p�[�e�B�N��
	vector<Particle*> m_pParticleArray;
	//���y����������Ȃ��悤��
	bool m_isMusicFlag;
	//�Z���N�g�V�[���őI�񂾃X�e�[�W
	int m_selectScene;
	//�G�̏o���Ԋu
	int m_enemyPopTimeCount;
	//�G�̏o�������Ǘ�����\���̂������X�^�b�N
	stack<popEnemy> m_popEnemyList;
	//�o���l�̏�񂪓����Ă���z��
	int m_expList[50];
	//�`���[�g���A���摜�������Ă���z��
	int m_tutorialGraph[6];
	//�{�X�̐�������ϐ�
	int m_bossCount;
	//�|�����{�X�̐�������ϐ�
	int m_killBossCount;
	//�{�X��|������̎��Ԃ��J�E���g����ϐ�
	int m_clearTime;
	//���Ƀp�[�e�B�N�����o�Ă���܂ŃJ�E���g����ϐ�
	int m_particleCount;
	//�N���A�������ǂ������f����ϐ�
	bool m_isClearFlag;
	//���Ƀ����X�^�[���o�Ă���܂ł̎���
	int m_nextEnemyPopTime;
	//���ɏo�Ă��郂���X�^�[�̎��
	int m_nextEnemyKind;
	//�������[�h�̃Q�[�W
	float m_specialGauge;
	//�������[�h�𔭓����Ă��邩�ǂ���
	bool m_isSpecialMode;
	//�|�[�Y�ɓ����Ă��邩�ǂ���
	bool m_isPause;
	//���o���ɉ�ʂ��~�߂�
	bool m_isStop;
	//���U���g�𗬂��^�C�~���O
	bool m_isResult;
	//���ׂĂ��I�������t���b�O
	bool m_isEnd;
	//�|�[�Y��ʂőI�����Ă��鍀�ڂ�ۑ�����
	int m_pauseSelectNum;
	//�A���ŏ㉺���͂�����Ȃ��悤��
	bool m_isSelectKeyDown;
	//�N���A���̉��o�Ɏg���ϐ�
	bool m_isGoldLoop;
	bool m_isExpLoop;
	//���[�v���n�܂�܂ł̎��Ԃ𑪂�
	int m_startLoopTimeCount;
	//�Q�[���N���A�������ɕ������o���^�C�~���O
	bool m_isClearString;
	//�Q�[���I�[�o�[�����Ƃ��ɕ������o���^�C�~���O
	bool m_isGameOverString;
	//�G�ƂԂ������Ƃ��̉�
	int m_attackSe;
	//��_�ɂԂ������Ƃ��̉�
	int m_weakAttackSe;
	//�{�X���o�Ă������ɗ��Ă�t���O
	bool m_isBossFlag;
	//�N���A���̃_���X�̉��y
	int m_danceMusic;
	//�N���A���̊l���S�[���h�𐔂��鉹
	int m_resultGold;
	//�N���A���̌o���l
	int m_resultExp;
	//�}�b�v��BGM
	int m_fieldBgm;
	//�{�X���o�Ă�������BGM
	int m_bossBgm;
	//�Ō�̃_���X�O�̉�
	int m_beforeDanceSe;
	//�J�[�\���𓮂������Ƃ��̉�
	int m_cursorSe;
	//����{�^�����������Ƃ��̉�
	int m_appSe;
	//B�{�^�����������Ƃ��̉�
	int m_cancelSe;
	//�R�C�����E�������̉�
	int m_coinSe;
	//�o���l���E�������̉�
	int m_expSe;
	//�����̏ꏊ�Ƀp�[�e�B�N�����o���������f����
	bool m_isWitchParticle;
	//���x���A�b�v�������ǂ������f����
	bool m_isUpLevel;
	//���x���A�b�v�̕������o��Y���W
	int m_levelUpPosY;
	//�N���A�ł��Ȃ�������
	bool m_isGameOver;
	//���U���g��ʂŌo���l�ƃS�[���h�𔼕��ɂ������ǂ���
	bool m_isHalfGold;
	bool m_isHalfExp;
	//�Z���N�g�V�[���ɖ߂�
	bool m_isBackSelectScene;
	//�������[�h���n�܂������ɃJ�E���g����
	int m_specialModeStartCount;
	//�O�̃t���[���X�y�[�X�������Ă�����
	bool m_lastSpace;
	//�����@�̌��ʉ�
	int m_windMagicSe;
	//�����@�̌��ʉ�
	int m_fireMagicSe;
	//�������[�h���n�߂�Ƃ��ɖ炷���ʉ�
	int m_specialModeSe;
	//�|�[�V�������Ƃ����ۂ̌��ʉ�
	int m_portionSe;
	//�����E�������̌��ʉ�
	int m_bloodSe;
	//����n�����Ƃ��̌��ʉ�
	int m_passBloodSe;
	//�|�[�Y���J�����Ƃ��ɏo�镶��
	int m_pauseGraph;
	//�{�^���̃O���t�B�b�N
	int m_buttonsGraph;
	//Ui��\������w�i
	int m_uiBgGraph;
	//Princess�ƓG���Ԃ��������̌��ʉ�
	int m_hitPrincessSe;
	//�{�胂�[�h���ɕ\������摜
	int m_angryMarkGraph;
	//�Q�[���X�^�[�g���ɕ\������READY
	int m_readyGraph;
	//ready��\�����Ă��邩�ǂ���
	bool m_isShowReady;
	//ready��\�����Ă��鎞�Ԃ��J�E���g����
	int m_readyCount;
	//�`���[�g���A����\�����Ă��邩�ǂ���
	bool m_isShowTutorial;
	//�G���^�[�L�[���������ςȂ��ɂȂ�Ȃ��悤��
	bool m_isLastKey;
	//�\������`���[�g���A���̖���
	int m_tutorialNum;
	//�`���[�g���A���������ڂ���\�����邩
	int m_startTutorialNum;
	//���\�����Ă���`���[�g���A���������ڂ�
	int m_nowShowTutorialNum;
	//���ʉ����������ǂ���
	bool m_isLastSe;

};