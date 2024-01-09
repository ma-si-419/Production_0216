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
	/// SceneMainにあるアイテムの配列にpItemを追加するを追加する
	/// </summary>
	/// <param name="pItem">追加するアイテム</param>
	/// <returns>特に意味はない</returns>
	bool AddItem(std::shared_ptr<ItemBase> pItem);
	//SceneMainに魔法を出す関数
	bool AddMagic(MagicBase* pMagic);
	//SceneMainに宝箱を出す関数
	bool AddTreasure(TreasureBox* pTreasure);
	//SceneMainにパーティクルを出す関数
	bool AddParticle(Particle* pParticle);
	//当たり判定を判定する関数
	bool IsCollision(CircleCol col1, CircleCol col2);
	//敵を生成する関数
	bool CreateEnemy(int enemyKind);
	//倒したボスの数を数える
	void CountKillBoss() { m_killBossCount++; };
	//聖剣モードのゲージをためる
	void AddSpecialGauge(float gauge);
	//聖剣モードのゲージのたまっている割合を返す
	float GetSpecialGaugeRate() { return 100 / m_specialGauge; }
	//聖剣モードを発動しているかどうかを返す
	bool GetSpecialMode() { return m_isSpecialMode; }
	//ポーズ画面かどうかを取得する
	bool GetPause() { return m_isPause; }
	//シーンが終わったタイミング
	void SetEnd() { m_isResult = true; }
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
	//パーティクル
	Particle* m_pParticle;

	// 敵
	vector<shared_ptr<Enemy>> m_pEnemy;
	//アイテム
	vector<shared_ptr<ItemBase>> m_pItem;
	//魔法
	vector<MagicBase*> m_pMagic;
	//宝箱
	vector<TreasureBox*> m_pTreasure;
	//パーティクル
	vector<Particle*> m_pParticleArray;
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
	//次にパーティクルが出てくるまでカウントする変数
	int m_particleCount;
	//クリアしたかどうか判断する変数
	bool m_isClearFlag;
	//次にモンスターが出てくるまでの時間
	int m_nextEnemyPopTime;
	//次に出てくるモンスターの種類
	int m_nextEnemyKind;
	//聖剣モードのゲージ
	float m_specialGauge;
	//聖剣モードを発動しているかどうか
	bool m_isSpecialMode;
	//ポーズに入っているかどうか
	bool m_isPause;
	//演出時に画面を止める
	bool m_isStop;
	//リザルトを流すタイミング
	bool m_isResult;
	//すべてが終了したフラッグ
	bool m_isEnd;
	//ポーズ画面で選択している項目を保存する
	int m_pauseSelectNum;
	//連続で上下入力がされないように
	bool m_isSelectKeyDown;
	//クリア時の演出に使う変数
	bool m_isGoldLoop;
	bool m_isExpLoop;
	//ループが始まるまでの時間を測る
	int m_startLoopTimeCount;
};