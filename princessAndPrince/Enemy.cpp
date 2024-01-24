#include "Game.h"
#include "Enemy.h"
#include "Player.h"
#include "MagicBase.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "TreasureBox.h"
#include "SceneMain.h"
#include "Particle.h"
#include<memory>
namespace
{
	// キャラクターのサイズ
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;
	// キャラクターのアニメーション
	constexpr int kUseFrame[] = { 0,1 };
	// アニメーション1コマのフレーム数
	constexpr int kAnimFrameNum = 60;
	// アニメーションの１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//ノックバックの大きさ
	constexpr int kKnockBackScale = 3;
	//エネミーが宝箱を落とす確率(Max100)
	constexpr int kDropProb = 100;
	//マジックにヒットするインターバル
	constexpr int kHitMagicInterval = 30;
	//プレイヤーにヒットするインターバル
	constexpr int kHitPlayerInterval = 8;
	//生まれる方向の最大数
	constexpr int kMaxDir = 3;
	//ボスのサイズ
	constexpr float kBossSize = 2.0f;
	//通常の敵のサイズ
	constexpr float kEnemySize = 1.0f;
	//弱点の当たり判定をずらすベクトル
	constexpr int kShiftPosX = 20;
	constexpr int kShiftPosY = 10;
	//エフェクトを出す時間(後で消す)
	constexpr int kEffectTime = 10;
	//エフェクトの大きさ(仮実装)
	constexpr int kEffectSize = 60;
	//パーティクルの数
	constexpr int kParticleVol = 30;

}
Enemy::Enemy(SceneMain* pMain) :
	m_targetPos(Game::kPlayScreenWidth / 2, Game::kPlayScreenHeight / 2),
	m_pMain(pMain),
	m_hitCount(0),
	m_isHitFlag(false),
	m_effectCount(0),
	m_haveGold(0),
	m_haveExp(0),
	m_isBoss(false),
	m_isHitWeakFlag(false),
	m_isLeftFlag(false),
	m_kind(goblin),
	m_pTreasureBox(nullptr),
	m_scale(0.0f),
	m_srcY(0),
	m_knockBackPow(0)
{
	m_animFrame = 0;
	m_nowState = Game::kNormal;

}
Enemy::~Enemy()
{
}
void Enemy::Init(int kinds)
{
	m_animFrame = 0;
	int bornPos = GetRand(3);
	switch (bornPos)
	{
		//左方向から出てくる
	case 0:
		m_pos.x = 0;
		m_pos.y = GetRand(Game::kPlayScreenHeight);
		break;
		//上方向から出てくる
	case 1:
		m_pos.x = GetRand(Game::kPlayScreenWidth);
		m_pos.y = 0;
		break;
		//右方向から出てくる
	case 2:
		m_pos.x = Game::kPlayScreenWidth;
		m_pos.y = GetRand(Game::kPlayScreenHeight);
		break;
		//下方向から出てくる
	case 3:
		m_pos.x = GetRand(Game::kPlayScreenWidth);
		m_pos.y = Game::kPlayScreenHeight;
		break;
	default:
		break;
	}
	m_radius = Game::kRadius;
	////////////////////////////
	/*エネミーのステータス設定*/
	////////////////////////////
	if (kinds == static_cast<int>(goblin))
	{
		m_hp = 12;
		m_atk = 3.0f;
		m_spd = 0.2f;
		m_scale = kEnemySize;
		m_srcY = 0;
		m_haveGold = GetRand(5) + 3;
		m_haveExp = GetRand(3) + 1;
		m_kind = goblin;
		m_isBoss = false;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(boar))
	{
		m_hp = 7;
		m_atk = 1.0f;
		m_spd = 0.5f;
		m_scale = kEnemySize;
		m_srcY = 1;
		m_haveGold = GetRand(10) + 8;
		m_haveExp = GetRand(5) + 2;
		m_kind = boar;
		m_isBoss = false;
		m_knockBackPow = 4;

	}
	else if (kinds == static_cast<int>(doragon))
	{
		m_hp = 20;
		m_atk = 20.0f;
		m_spd = 0.1f;
		m_scale = kEnemySize;
		m_srcY = 2;
		m_haveGold = GetRand(100) + 50;
		m_haveExp = GetRand(10) + 5;
		m_kind = doragon;
		m_isBoss = false;
		m_knockBackPow = 4;

	}
	else if (kinds == static_cast<int>(skeleton))
	{
		m_hp = 15;
		m_atk = 3.0f;
		m_spd = 0.3f;
		m_scale = kEnemySize;
		m_srcY = 3;
		m_kind = skeleton;
		m_haveGold = GetRand(20) + 20;
		m_haveExp = GetRand(10) + 3;
		m_isBoss = false;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(snowman))
	{
		m_hp = 50;
		m_atk = 5.0f;
		m_spd = 0.1f;
		m_scale = kEnemySize;
		m_srcY = 4;
		m_kind = snowman;
		m_haveGold = GetRand(150) + 50;
		m_haveExp = GetRand(20) + 10;
		m_isBoss = false;
		m_knockBackPow = 1;
	}
	else if (kinds == static_cast<int>(bossGoblin))
	{
		m_hp = 48;
		m_atk = 6.0f;
		m_spd = 0.2f;
		m_scale = kBossSize;
		m_srcY = 0;
		m_kind = bossGoblin;
		m_haveGold = GetRand(150) + 100;
		m_haveExp = GetRand(20) + 10;
		m_isBoss = true;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossBoar))
	{
		m_hp = 21;
		m_atk = 3.0f;
		m_spd = 0.4f;
		m_scale = kBossSize;
		m_srcY = 1;
		m_kind = bossBoar;
		m_haveGold = GetRand(150) + 150;
		m_haveExp = GetRand(30) + 15;
		m_isBoss = true;
		m_knockBackPow = 3;

	}
	else if (kinds == static_cast<int>(bossDoragon))
	{
		m_hp = 60;
		m_atk = 25.0f;
		m_spd = 0.1f;
		m_scale = kBossSize;
		m_srcY = 2;
		m_kind = bossDoragon;
		m_haveGold = GetRand(300) + 500;
		m_haveExp = GetRand(50) + 50;
		m_isBoss = true;
		m_knockBackPow = 3;
	}
	else if (kinds == static_cast<int>(bossSkeleton))
	{
		m_hp = 50;
		m_atk = 7.0f;
		m_spd = 0.3f;
		m_scale = kBossSize;
		m_srcY = 3;
		m_kind = bossSkeleton;
		m_haveGold = GetRand(300) + 300;
		m_haveExp = GetRand(40) + 40;
		m_isBoss = true;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossSnowman))
	{
		m_hp = 150;
		m_atk = 12.0f;
		m_spd = 0.1f;
		m_scale = kBossSize;
		m_srcY = 4;
		m_kind = bossSnowman;
		m_haveGold = GetRand(1000) + 1000;
		m_haveExp = GetRand(100) + 100;
		m_isBoss = true;
		m_knockBackPow = 1;
	}
}

void Enemy::Update()
{
	//状態がkDeleteじゃない場合のみ動く
	if (m_nowState != Game::kDelete)
	{
		//アニメーションの更新処理
		m_animFrame++;
		if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 0;
		//ノックバック処理
		if (m_knockBack.x != 0 || m_knockBack.y != 0)
		{
			//ノックバックする時間カウント
			m_knockBackTime++;
			//規定の時間を過ぎたら
			if (m_knockBackTime > 5)
			{
				//ノックバックの大きさを0にする
				m_knockBack *= 0;
				//時間カウントを0にする
				m_knockBackTime = 0;
				//敵が死ぬときのタイミングが
				//ノックバックした後のほうが見栄えがいいのでここに死亡処理を入れる
				if (m_hp < 0)
				{
					//状態を変化させる
					m_nowState = Game::kDelete;
					//血のメモリの確保
					std::shared_ptr<Blood> pBlood
						= std::make_shared<Blood>();
					//血を落とす処理
					pBlood->Init(m_pos);
					m_pMain->AddItem(pBlood);
					//経験値のメモリの確保
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//経験値を落とす処理
					pExp->Init(m_pos);
					pExp->SetExp(m_haveExp);
					m_pMain->AddItem(pExp);
					//お金のメモリ確保
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//お金を落とす処理
					pGold->Init(m_pos);
					pGold->SetPrice(m_haveGold);
					m_pMain->AddItem(pGold);
					if (GetRand(100) < kDropProb)
					{
						m_pTreasureBox = new TreasureBox(m_pMain);
						m_pTreasureBox->Init(m_pos);
						//メインに宝箱を生成する関数を作成する
						m_pMain->AddTreasure(m_pTreasureBox);
					}
					//ボスだったらボスを倒したとシーンメインに伝える
					if (m_isBoss)
					{
						m_pMain->CountKillBoss();
					}
				}
			}
		}
		//魔法に連続で当たらないように状態を変化させる
		if (m_nowState == Game::kHitMagic)
		{
			m_hitCount++;
			if (m_hitCount > kHitMagicInterval)
			{
				m_nowState = Game::kNormal;
				m_hitCount = 0;
			}
		}
		//プレイヤーに連続で当たらないように状態を変化させる
		if (m_nowState == Game::kHitPlayer)
		{
			m_hitCount++;
			if (m_hitCount > kHitPlayerInterval)
			{
				m_nowState = Game::kNormal;
				m_hitCount = 0;
			}
		}
		if (m_nowState != Game::kStop)
		{
			//移動量の計算
			m_moveVec = m_targetPos - m_pos;
			m_moveVec.Normalize();
			m_moveVec *= m_spd;
			//移動処理
			m_pos += m_moveVec;
			m_pos -= m_knockBack;
			//当たり判定の更新
			m_circleCol.SetCenter(m_pos, m_radius * m_scale);
			//弱点のY座標の更新
			m_weakPos.y = m_pos.y + kShiftPosY;
		}
		if (m_moveVec.x > 0)
		{
			m_isLeftFlag = false;
			//弱点のX座標の更新
			m_weakPos.x = m_pos.x - kShiftPosX;
		}
		else
		{
			m_isLeftFlag = true;
			//弱点のX座標の更新
			m_weakPos.x = m_pos.x + kShiftPosX;

		}
		//弱点当たり判定の更新
		m_weakCircle.SetCenter(m_weakPos, m_radius * m_scale);
	}
}
void Enemy::Draw()
{
	//状態がkDeleteじゃない場合のみ動かす
	if (m_nowState != Game::kDelete)
	{

		int animEle = m_animFrame / kAnimFrameNum;

		int srcX = kGraphWidth * kUseFrame[animEle];
		int srcY = kGraphHeight * m_srcY;

		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			4.0 * m_scale,
			0.0,
			m_handle, true, m_isLeftFlag);
#ifdef _DEBUG
		m_circleCol.Draw(m_radius * m_scale, 0x0000ff, false);
		m_weakCircle.Draw(m_radius * m_scale, 0xff0000, false);
#endif
	}
}

void Enemy::HitPlayer(Player& player, bool weak)
{
	//プレイヤーの座標を参考にして、ノックバックの方向を決める
	m_knockBack = player.GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= m_knockBackPow * kKnockBackScale;
	m_isHitFlag = true;
	//中点を出す(衝突点の座標)
	m_hitPos = (player.GetPos() + m_pos) / 2;
	//弱点に当たっていたら
	if (weak)
	{
		m_hp -= (player.GetAtk() - m_def) * 2;
		//赤いエフェクトを出す
		for (int i = 0; i < kParticleVol; i++)
		{
			m_pParticle = new Particle(m_hitPos, 40.0f, 4.0f, 5, 1);
			m_pMain->AddParticle(m_pParticle);
		}
		m_knockBack *= 0.6f;
	}
	//当たっていなかったら
	else
	{
		m_hp -= player.GetAtk() - m_def;
		//白いエフェクトを出す
		for (int i = 0; i < kParticleVol; i++)
		{
			m_pParticle = new Particle(m_hitPos, 40.0f, 4.0f, 5, 0);
			m_pMain->AddParticle(m_pParticle);
		}
	}
}

void Enemy::HitMagic(MagicBase* magic)
{
	//体力を減らす
	m_hp -= magic->GetAtk();
	//ぶつかったときのエフェクトを出す
	m_isHitFlag = true;
	m_isHitWeakFlag = false;
	//白いエフェクトを出す
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_pos, 40.0f, 4.0f, 5, 0);
		m_pMain->AddParticle(m_pParticle);
	}
	//もし魔法が炎魔法だったら
	if (magic->GetMagicKind())
	{
		//ぶつかった魔法を消す
		magic->m_nowState = Game::kDelete;
	}
	//魔法で死んだ場合の処理をここに入れておく
	if (m_hp < 0)
	{//血のメモリの確保
		std::shared_ptr<Blood> pBlood
			= std::make_shared<Blood>();
		//血を落とす処理
		pBlood->Init(m_pos);
		m_pMain->AddItem(pBlood);
		//経験値のメモリの確保
		std::shared_ptr<Exp> pExp
			= std::make_shared<Exp>();
		//経験値を落とす処理
		pExp->SetExp(m_haveExp);
		pExp->Init(m_pos);
		m_pMain->AddItem(pExp);
		//お金のメモリ確保
		std::shared_ptr<Gold> pGold
			= std::make_shared<Gold>();
		//お金を落とす処理
		pGold->SetPrice(m_haveGold);
		pGold->Init(m_pos);
		m_pMain->AddItem(pGold);
		if (GetRand(100) < kDropProb)
		{
			m_pTreasureBox = new TreasureBox(m_pMain);
			m_pTreasureBox->Init(m_pos);
			//メインに宝箱を生成する関数を作成する
			m_pMain->AddTreasure(m_pTreasureBox);
		}
		//ボスを倒したらボスを倒したとシーンメインに教える
		if (m_isBoss)
		{
			m_pMain->CountKillBoss();
		}
		m_nowState = Game::kDelete;
	}
}
