#pragma once
#include"Scene.h"
#include <vector>
#include<memory>
using namespace std;

class Player;
class Princess;
class Vec2;
class Enemy;
class ItemBase;
class UI;
class SceneManager;
class MagicBase;

class SceneMain : public Scene
{
public:
	SceneMain(SceneManager& manager);
	virtual ~SceneMain();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();

	/// <summary>
	/// SceneMain�ɂ���A�C�e���̔z���pItem��ǉ������ǉ�����
	/// </summary>
	/// <param name="pItem">�ǉ�����A�C�e��</param>
	/// <returns>���ɈӖ��͂Ȃ�</returns>
	bool AddItem(std::shared_ptr<ItemBase> pItem);

	bool AddMagic(MagicBase* pMagic);

	bool createEnemy();
private:
	// �G�L�����N�^�[�̐���
//	void createEnemyLeft();
//	void createEnemyRight();
//	void createEnemyToPlayer();

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
	// �z��̃T�C�Y�͎��s���Ɍ��߂�(����̓R���X�g���N�^)
//	vector<EnemyBase*> m_pEnemy;
	//���y����������Ȃ��悤��
	bool m_isMusicFlag;
	//�����ɐG��āA������o��܂łɉ��x���������������Ȃ����߂�
	bool m_isTouch;
	//�G�̏o���Ԋu
	int m_enemyPopTimeCount;
};