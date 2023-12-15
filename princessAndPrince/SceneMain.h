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

	bool AddMagic(MagicBase* pMagic);

	bool AddTreasure(TreasureBox* pTreasure);

	bool IsCollision(CircleCol col1, CircleCol col2);

	bool createEnemy();

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

};
//
////�o������G�̎�ނ̏���
//struct popEnemy
//{
//	int kind;
//	float popTime;
//};
//
//class Map1
//{
//	int temp;
//	stack<popEnemy> m_popEnemyList;
//
//	Map1()
//	{
//		//1
//		temp.e = new Enemy();
//		temp.popTime = 2.1f;
//		//push
//		m_popEnemyList.push(temp);
//		//2
//		temp.e = new Enemy();
//		temp.popTime = 2.1f;
//		//push
//		m_popEnemyList.push(temp);
//
//		//pos
//		temp = m_popEnemyList.pop();
//		temp.popTime;
//		temp.e;
//	}
//
//
//};