#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
class SceneMain;
class Player;
class Particle;
class TreasureBox
{
public:
	//コンストラクタ
	TreasureBox(SceneMain* sceneMain);
	//デストラクタ
	~TreasureBox();
	//初期化処理
	void Init(Vec2 pos);
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//宝箱の体力の減らす
	void SubHp() { m_hp--; }
	//宝箱の体力を減らす
	int GetHp() { return m_hp; }
	//Playerとぶつかったとき
	void HitPlayer(Player* player);
	//魔法とぶつかったとき
	void HitMagic();
	//自分の場所を返す
	Vec2 GetPos() { return m_pos; }
	//当たり判定を返す
	CircleCol GetColCircle() { return m_circleCol; }
	//ノックバックの大きさを返す
	float GetKnockBackPow() { return m_knockBackPow; }
	//グラフィックハンドルを設定する
	void SetHandle(int handle) { m_handle = handle; }
	//経験値とお金の量を設定する
	void SerGoldAndExp(int gold, int exp) { m_gold = gold; m_exp = exp; }
	//状態を管理する
	Game::State m_nowState;


private:
	enum Dropkind  
	{
		kPortion,
		kExp,
		kGold,
		kBlood
	};
	//SceneMainのポインタ
	SceneMain* m_pMain;
	//表示位置
	Vec2 m_pos;
	//プレイヤーと衝突した点の座標
	Vec2 m_hitPos;
	//パーティクルのポインタ
	Particle* m_pParticle;
	//グラフィックハンドル
	int m_handle;
	//当たり判定
	CircleCol m_circleCol;
	//存在しているかどうか
	bool m_isExist;
	//宝箱の体力
	int m_hp;
	//当たり判定の大きさ
	int m_colScale;
	//魔法に当たったときにカウントする変数
	int m_magicCount;
	//ノックバックのベクトル
	Vec2 m_knockBackVec;
	//ノックバックする時間をカウントする変数
	int m_knockBackTime;
	//ノックバックの大きさを決める変数
	float m_knockBackPow;
	//出てくるお金の金額
	int m_gold;
	//出てくる経験値の量
	int m_exp;
};

