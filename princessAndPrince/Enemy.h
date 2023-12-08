#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class Player;
class SceneMain;
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
	//敵が存在しているかどうかを判断する
	bool IsExist() { return m_isExist; }
	//プレイヤーとぶつかった際の処理
	void OnDamage(Player& player);
	//プリンセスとぶつかった際のノックバック
	void HitPrincess(Vec2 knockBack) { m_knockBack = knockBack; }
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
	//存在しているかフラグ
	bool m_isExist;
	//左を向いているかフラグ
	bool m_isLeftFlag;
};

