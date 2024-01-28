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
	SceneMain(SceneManager& sceneManager,DataManager& DataManager,int stageNum);
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
	//演出が終わったタイミング
	void SetEnd() { m_isResult = true; }
	//リザルトの文字を表示するタイミング
	void ShowResult() { m_isClearString = true; }
	//桁数を取得する
	int GetDigits(int num);
	//リザルトのGOLDのループを開始する
	void StartExpLoop() { m_isExpLoop = true; }
	//音量を設定する
	void ChangeSoundVol(int volume);
	//次のレベルまでの経験値量を返す
	int GetNextExp();
	//コインの効果音を返す
	int GetCoinSe() { return m_coinSe; };
	//経験値の効果音を返す
	int GetExpSe() { return m_expSe; };

private:
	// グラフィックのハンドル
	int m_playerHandle;
	int m_princessHandle;
	int m_enemyHandle;
	int m_bgHandle;
	int m_itemHandle;
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
	//経験値の情報が入っている配列
	int m_expList[50];
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
	//ゲームクリアした時に文字を出すタイミング
	bool m_isClearString;
	//ゲームオーバーしたときに文字を出すタイミング
	bool m_isGameOverString;
	//敵とぶつかったときの音
	int m_attackSe;
	//弱点にぶつかったときの音
	int m_weakAttackSe;
	//ボスが出てきた時に立てるフラグ
	bool m_isBossFlag;
	//クリア時のダンスの音楽
	int m_danceMusic;
	//クリア時の獲得ゴールドを数える音
	int m_resultGold;
	//クリア時の経験値
	int m_resultExp;
	//マップのBGM
	int m_fieldBgm;
	//ボスが出てきた時のBGM
	int m_bossBgm;
	//最後のダンス前の音
	int m_beforeDanceSe;
	//カーソルを動かしたときの音
	int m_cursorSe;
	//決定ボタンを押したときの音
	int m_appSe;
	//Bボタンを押したときの音
	int m_cancelSe;
	//コインを拾った時の音
	int m_coinSe;
	//経験値を拾った時の音
	int m_expSe;
	//魔女の場所にパーティクルを出したか判断する
	bool m_isWitchParticle;
	//レベルアップしたかどうか判断する
	bool m_isUpLevel;
	//レベルアップの文字を出すY座標
	int m_levelUpPosY;
	//クリアできなかった時
	bool m_isGameOver;
	//リザルト画面で経験値とゴールドを半分にしたかどうか
	bool m_isHalfGold;
	bool m_isHalfExp;
	//セレクトシーンに戻る
	bool m_isBackSelectScene;
};