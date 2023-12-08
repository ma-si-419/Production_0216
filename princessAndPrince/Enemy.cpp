#include "Game.h"
#include "Enemy.h"
#include "Player.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "SceneMain.h"
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
	constexpr int kKnockBackScale = 4;
	//エネミーが宝箱を落とす確率(Max100)
	constexpr int kDropProb = 20;
}
Enemy::Enemy(SceneMain* pMain, Player* pPlayer) :
	m_targetPos(Game::kScreenWidth / 2, Game::kScreenHeight / 2),
	m_isExist(true),
	m_pPlayer(pPlayer),
	m_pMain(pMain)
{
	m_animFrame = 0;
}
Enemy::~Enemy()
{
}
void Enemy::Init(int kinds)
{
	m_animFrame = 0;
	m_pos.x = GetRand(1000) - 200;
	m_pos.y = GetRand(1000) - 200;
	m_radius = Game::kRadius;
	if (kinds == (int)goblin)
	{
		m_hp = 2;
		m_atk = 1;
		m_spd = 0.1f;
		m_kind = goblin;
	}
	if (kinds == (int)boar)
	{
		m_hp = 3;
		m_atk = 1;
		m_spd = 0.2f;
		m_kind = boar;
	}
	if (kinds == (int)doragon)
	{
		m_hp = 4;
		m_atk = 1;
		m_spd = 0.3f;
		m_kind = doragon;
	}
	if (kinds == (int)skeleton)
	{
		m_hp = 5;
		m_atk = 1;
		m_spd = 0.4f;
		m_kind = skeleton;
	}
	if (kinds == (int)snowman)
	{
		m_hp = 6;
		m_atk = 1;
		m_spd = 0.5f;
		m_kind = snowman;
	}
}

void Enemy::Update()
{
	//できるならポインタ自体を消したい
	//存在している敵のみUpdateする(仮処理)
	if (m_isExist)
	{
		//アニメーションの更新処理
		m_animFrame++;
		if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 0;
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
					//血のメモリの確保
					std::shared_ptr<Blood> pBlood
					 = std::make_shared<Blood>();
					//経験値のメモリの確保
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//お金のメモリ確保
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//血を落とす処理
					pBlood->Init(m_pos);
					m_pMain->AddItem(pBlood);
					//経験値を落とす処理
					pExp->Init(m_pos);
					m_pMain->AddItem(pExp);
					//お金を落とす処理
					pGold->Init(m_pos);
					m_pMain->AddItem(pGold);
					if (GetRand(100) < kDropProb)
					{

					}

					//存在するフラッグをfalseにする
					m_isExist = false;
				}
			}
		}
		m_moveVec = m_targetPos - m_pos;
		m_moveVec.Normalize();
		m_moveVec *= m_spd;
		m_pos += m_moveVec;
		m_pos -= m_knockBack;
		m_circleCol.SetCenter(m_pos, m_radius);
		if (m_moveVec.x > 0)
		{
			m_isLeftFlag = false;
		}
		else
		{
			m_isLeftFlag = true;
		}
	}
}
void Enemy::Draw()
{
	if (m_isExist)
	{

		int animEle = m_animFrame / kAnimFrameNum;

		int srcX = kGraphWidth * kUseFrame[animEle];
		int srcY = kGraphHeight * (int)m_kind;

		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			2.0,
			0.0,
			m_handle, true, m_isLeftFlag);
#ifdef _DEBUG
		m_circleCol.Draw(m_radius, 0x0000ff, false);
#endif
	}
}

void Enemy::OnDamage(Player& player)
{
	m_knockBack = player.GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale;
	m_hp -= player.GetAtk() - m_def;
}