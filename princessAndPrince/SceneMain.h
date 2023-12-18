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
	//SceneMainに魔法を出す関数
	bool AddMagic(MagicBase* pMagic);
	//SceneMainに宝箱を出す関数
	bool AddTreasure(TreasureBox* pTreasure);
	//当たり判定を判定する関数
	bool IsCollision(CircleCol col1, CircleCol col2);
	//敵を生成する関数
	bool CreateEnemy(int enemyKind);
	//倒したボスの数を数える
	void CountKillBoss() { m_killBossCount++; };
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
	//ボスの数を入れる変数
	int m_bossCount;
	//倒したボスの数を入れる変数
	int m_killBossCount;
	//ボスを倒した後の時間をカウントする変数
	int m_clearTime;
	//クリアしたかどうか判断する変数
	bool m_clearFlag;
	//次にモンスターが出てくるまでの時間
	int m_nextEnemyPopTime;
	//次に出てくるモンスターの種類
	int m_nextEnemyKind;
};