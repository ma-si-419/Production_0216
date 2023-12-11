#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class Player;
class SceneMain;
class MagicBase;
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
	Enemy(SceneMain* pMain,Player* pPlayer);
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
	void HitMagic(MagicBase& magic);
	//プリンセスとぶつかった際のノックバック
	void HitPrincess(Vec2 knockBack) { m_knockBack = knockBack; }
	//状態を管理する
	Game::State m_nowState;
private:
	//SceneMainのポインタをもつ
	SceneMain* m_pMain;
	//プレイヤーのポインタを持つ
	Player* m_pPlayer;
	//ターゲットのポジション
	Vec2 m_targetPos;
	//敵の種類
	Kinds m_kind;
	//動く方向
	Vec2 m_moveVec;
	//左を向いているかフラグ
	bool m_isLeftFlag;
};

