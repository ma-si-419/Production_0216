#pragma once
#include <memory>
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class SceneMain;
class MagicBase;
class Player;
class TreasureBox;
class Particle;
class Enemy : public Character
{
public:
	enum Kinds
	{
		goblin,
		boar,
		doragon,
		blackSkeleton,
		snowman,
		redGoblin,
		skeleton,
		bossGoblin,
		bossBoar,
		bossDoragon,
		bossBlackSkeleton,
		bossSnowman,
		bossRedGoblin,
		bossSkeleton,
		bossDemonKing
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
	void HitPlayer(Player& player,bool weak);
	//魔法とぶつかったときの処理
	void HitMagic(MagicBase* magic);
	//プリンセスとぶつかった際のノックバック
	void HitPrincess(Vec2 knockBack) { m_knockBack = knockBack; }
	//弱点の当たり判定を返す
	CircleCol GetWeakCircle() { return m_weakCircle; }
	//動いているベクトルを返す
	Vec2 GetMoveVec() { return m_moveVec; }
	//移動速度を返す
	float GetKnockBackPow() { return m_knockBackPow; }
	//アイテムのハンドルを設定する
	void SetItemHandle(int handle) { m_itemHandle = handle; }
	//移動速度を取得する
	float GetSpd() { return m_spd; }
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
	//魔法やプレイヤーに連続でヒットしないためにカウントを用意する
	int m_hitCount;
	//宝箱のポインタ
	TreasureBox* m_pTreasureBox;
	//大きいキャラかどうかを判断する
	bool m_isBoss;
	//キャラの大きさ
	float m_scale;
	//当たり判定の大きさ
	float m_colScale;
	//使う画像のY座標を入れる
	int m_srcY;
	//弱点の当たり判定をとる
	CircleCol m_weakCircle;
	//弱点の中心座標
	Vec2 m_weakPos;
	//あたったときにどのエフェクトを出すか判断する
	bool m_isHitFlag;
	//弱点に当たったかどうかを判断する
	bool m_isHitWeakFlag;
	//衝突した点の座標
	Vec2 m_hitPos;
	//エフェクトを出す時間をカウントする(後で消すかも)
	int m_effectCount;
	//持っているお金の量
	int m_haveGold;
	//持っている経験値の量
	int m_haveExp;
	//ぶつかった時のノックバックの大きさ
	float m_knockBackPow;
	//パーティクルのポインタ
	Particle* m_pParticle;
	//アイテムのハンドル
	int m_itemHandle;
};

