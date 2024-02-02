#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
#include <memory>
class Princess;
class Enemy;
class TreasureBox;
class SceneMain;
class ItemBase;
class Player : public Character
{

public:
	Player(SceneMain* pMain);
	~Player();

	void Init();
	void Update();
	void Draw() const;

	//敵にぶつかった際の処理
	void HitEnemy(Enemy enemy,bool weak);
	//宝箱にぶつかったときの処理
	void HitTreasure(TreasureBox* treasureBox);
	//現在のHpの割合計算する関数
	float GetHpRate() { return m_nowHp / m_hp; }
	//回復量を計算する
	float GetHealRate() { return m_hp / 300; }
	//現在持っている血の量の割合を計算する
	float GetBloodRate() { return m_nowBlood / m_maxBlood; }
	//倒れているかどうかを取得する
	bool GetDeathFlag() { return m_isDeathFlag; }
	//持っているゴールドの量を取得する
	int GetGold() { return m_gold; }
	//ゲームオーバーの時は持っているゴールドを半分減らす
	void SubGoldGameOver() { m_gold *= 0.5; }
	//ゲームオーバーの時は持っている経験値を半分減らす
	void SubExpGameOver() { m_exp *= 0.5; }
	//グラフハンドルを返す
	int GetHandle() { return m_handle; }
	//持っているゴールドを減らす
	void SubGold(int subNum) { m_gold -= subNum; }
	//持っている経験値の量を取得する
	int GetExp() { return m_exp; }
	//持っている経験値を減らす
	void SubExp(int subNum) { m_exp-= subNum; }
	//防御力を取得する
	float GetDef() { return m_def; }
	//アイテムを取得した時の処理
	void PickUpItem(std::shared_ptr<ItemBase> item);
	//経験値を増やす関数
	void PickUpExp(int exp) { m_exp += exp; }
	//ゴールドを増やす関数
	void PickUpGold(int gold) { m_gold += gold; }
	//持っている血を渡す関数
	void GiveBlood(Princess* princess);
	//死んだときの処理を作成する
	void DeathMove();
	//クリアした時の演出
	void ClearDance();
	//向いている方向を前方にする
	void TurnFront() { m_dir = Game::kDirDown; }
	//今動いているベクトルを取得する
	Vec2 GetMoveVec() { return m_moveVec; }
	//怒りモードを始めるときによぶ
	void SetSpecialMode() { m_dir = Game::kDirDeath; m_animFrame = 24; }
	void StartSpecialMode() { m_dir = Game::kDirDown; m_animFrame = 0; }
	//怒りモード時のグラフィック
	void SetPlayerAngryGraph(int graph) { m_playerAngryGraph = graph; }
	void SetAngryFireGraph(int graph) { m_angryFireGraph = graph; }
private:
	//シーンメインのポインタを入れる
	SceneMain* m_pMain;
	//歩いているかどうか
	bool m_isMove;
	//Hpバーの位置
	Vec2 m_hpBarPos;
	//血の量を表すバーの位置
	Vec2 m_bloodBarPos;
	//移動ベクトル
	Vec2 m_moveVec;
	//Hpバーの長さ
	float m_hpBarWidth;
	//現在のHPの量
	float m_nowHp;
	//血の量バーの長さ
	float m_bloodBarWidth;
	//現在持っている血の量
	float m_nowBlood;
	//持てる血の量
	float m_maxBlood;
	//現在倒れているかどうかフラグ
	bool m_isDeathFlag;
	//前のフレーム度のキーを押していたか保存する変数
	int m_lastPad;
	//倒れた時の座標を入れる変数
	Vec2 m_deathPos;
	//入手したお金を保存する
	int m_gold;
	//入手した経験値を保存する
	int m_exp;
	//復活する時間をカウントする変数
	int m_revivalCount;
	//クリア時の演出の時にカウントする変数
	int m_danceCount;
	//クリア時の演出で回った数をカウントする
	int m_turnCount;
	//コインを拾った時の効果音
	int m_coinSe;
	//経験値を拾った時の効果音
	int m_expSe;
	//ポーションをとった時の効果音
	int m_portionSe;
	//血を拾った時の効果音
	int m_bloodSe;
	//怒りモード時のグラフィック
	int m_playerAngryGraph;
	//怒りモード時に後ろに出すグラフィック
	int m_angryFireGraph;
	//炎を反転させるかどうか
	bool m_isAngryFireReverseFlag;
};