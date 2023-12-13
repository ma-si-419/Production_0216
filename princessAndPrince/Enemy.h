#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class SceneMain;
class MagicBase;
class Player;
class TreasureBox;
class Enemy : public Character
{
public:
	enum Kinds
	{
		goblin,
		boar,
		doragon,
		skeleton,
		snowman
	};

public:
	Enemy(SceneMain* pMain);
	virtual ~Enemy();
	//初期化処理
	void Init(int kinds);
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//プレイヤーとぶつかった際の処理
	void HitPlayer(Player& player);
	//魔法とぶつかったときの処理
	void HitMagic(MagicBase* magic);
	//プリンセスとぶつかった際のノックバック
	void HitPrincess(Vec2 knockBack) { m_knockBack = knockBack; }
private:
	//SceneMainのポインタをもつ
	SceneMain* m_pMain;
	//ターゲットのポジション
	Vec2 m_targetPos;
	//敵の種類
	Kinds m_kind;
	//動く方向
	Vec2 m_moveVec;
	//左を向いているかフラグ
	bool m_isLeftFlag;
	//魔法に連続でヒットしないためにカウントを用意する
	int m_hitMagicCount;
	//宝箱のポインタ
	TreasureBox* m_pTreasureBox;
};

