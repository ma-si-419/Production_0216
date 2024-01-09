#pragma once
#include"Scene.h"
#include <vector>
#include<memory>
#include<stack>
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
	SceneMain(SceneManager& manager,int stageNum);
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
	//�V�[�����I������^�C�~���O
	void SetEnd() { m_isResult = true; }
private:
	// �O���t�B�b�N�̃n���h��
	int m_playerHandle;
	int m_princessHandle;
	int m_enemyHandle;
	int m_bgHandle;
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
	//�G�̏o���Ԋu
	int m_enemyPopTimeCount;
	//�G�̏o�������Ǘ�����\���̂������X�^�b�N
	stack<popEnemy> m_popEnemyList;
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
};