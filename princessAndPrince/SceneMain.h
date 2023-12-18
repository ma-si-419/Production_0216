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
struct popEnemy
{
	int enemyKind;
	float popTime;
};
class SceneMain : public Scene
{
public:
	SceneMain(SceneManager& manager);
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
	//�����蔻��𔻒肷��֐�
	bool IsCollision(CircleCol col1, CircleCol col2);
	//�G�𐶐�����֐�
	bool CreateEnemy(int enemyKind);
	//�|�����{�X�̐��𐔂���
	void CountKillBoss() { m_killBossCount++; };
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
	// �w�i

	// �G
	vector<shared_ptr<Enemy>> m_pEnemy;
	//�A�C�e��
	vector<shared_ptr<ItemBase>> m_pItem;
	//���@
	vector<MagicBase*> m_pMagic;
	//��
	vector<TreasureBox*> m_pTreasure;
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
	//�N���A�������ǂ������f����ϐ�
	bool m_clearFlag;
	//���Ƀ����X�^�[���o�Ă���܂ł̎���
	int m_nextEnemyPopTime;
	//���ɏo�Ă��郂���X�^�[�̎��
	int m_nextEnemyKind;
};