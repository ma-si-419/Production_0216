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
	/// SceneMainにあるアイテムの配列にpItemを追加するを追加する
	/// </summary>
	/// <param name="pItem">追加するアイテム</param>
	/// <returns>特に意味はない</returns>
	bool AddItem(std::shared_ptr<ItemBase> pItem);

	bool AddMagic(MagicBase* pMagic);

	bool AddTreasure(TreasureBox* pTreasure);

	bool IsCollision(CircleCol col1, CircleCol col2);

	bool createEnemy();

private:
	// グラフィックのハンドル
	int m_playerHandle;
	int m_princessHandle;
	int m_enemyHandle;
	int m_bgHandle;
	// プレイヤー
	Player* m_pPlayer;
	//お姫様
	Princess* m_pPrincess;
	//UI
	UI* m_pUi;
	// 背景

	// 敵
	vector<shared_ptr<Enemy>> m_pEnemy;
	//アイテム
	vector<shared_ptr<ItemBase>> m_pItem;
	//魔法
	vector<MagicBase*> m_pMagic;
	//宝箱
	vector<TreasureBox*> m_pTreasure;
	//音楽が複数個流れないように
	bool m_isMusicFlag;
	//敵の出現間隔
	int m_enemyPopTimeCount;
	//敵の出現情報を管理する構造体をいれるスタック
	stack<popEnemy> m_popEnemyList;

};
//
////出現する敵の種類の順番
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