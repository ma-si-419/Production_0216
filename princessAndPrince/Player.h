#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class Princess;
class Enemy;
class TreasureBox;
class Player : public Character
{

public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw() const;

	//敵にぶつかった際の処理
	void HitEnemy(Enemy enemy);
	//宝箱にぶつかったときの処理
	void HitTreasure(TreasureBox* treasureBox);
	//現在のHpの割合計算する関数
	float GetHpRate() { return m_nowHp / m_hp; }
	//回復量を計算する
	float GetHealRate() { return m_hp / 300; }
	//現在持っている血の量の割合を計算する
	float GetBloodRate() { return m_nowBlood / m_maxBlood; }
	//倒れているかどうかを取得する
	bool GetDeathFlag() { return m_deathFlag; }
	//持っているゴールドの量を取得する
	int GetGold() { return m_gold; }
	//持っている経験値の量を取得する
	int GetExp() { return m_exp; }
	//アイテムを取得した時の処理
	void PickUpItem(Game::ItemKinds kind);
	//持っている血を渡す関数
	void GiveBlood(Princess* princess);
private:
	//歩いているかどうか
	bool m_isMove;
	//Hpバーの位置
	Vec2 m_hpBarPos;
	//血の量を表すバーの位置
	Vec2 m_bloodBarPos;
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
	bool m_deathFlag;
	//前のフレーム度のキーを押していたか保存する変数
	int m_lastPad;
	//倒れた時の座標を入れる変数
	Vec2 m_deathPos;
	//入手したお金を保存する
	int m_gold;
	//入手した経験値を保存する
	int m_exp;
};