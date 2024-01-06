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
	//持っているゴールドを減らす
	void SubGold() { m_gold--; }
	//持っている経験値の量を取得する
	int GetExp() { return m_exp; }
	//持っている経験値を減らす
	void SubExp() { m_exp--; }
	//防御力を取得する
	float GetDef() { return m_def; }
	//アイテムを取得した時の処理
	void PickUpItem(std::shared_ptr<ItemBase> item);
	//持っている血を渡す関数
	void GiveBlood(Princess* princess);
	//死んだときの処理を作成する
	void DeathMove();

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
};