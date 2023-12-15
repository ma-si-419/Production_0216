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
	constexpr int kDropProb = 30;
	//マジックにヒットするインターバル
	constexpr int kHitMagicInterval = 30;
	//生まれる方向の最大数
	constexpr int kMaxDir = 3;
	//ボスのサイズ
	constexpr float kBossSize = 3.0f;
	//通常の敵のサイズ
	constexpr float kEnemySize = 1.0f;
}
Enemy::Enemy(SceneMain* pMain) :
	m_targetPos(Game::kScreenWidth / 2, Game::kScreenHeight / 2),
	m_pMain(pMain),
	m_hitMagicCount(0)
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
		m_pos.y = GetRand(Game::kScreenHeight);
		break;
		//上方向から出てくる
	case 1:
		m_pos.x = GetRand(Game::kScreenWidth);
		m_pos.y = 0;
		break;
		//右方向から出てくる
	case 2:
		m_pos.x = Game::kScreenWidth;
		m_pos.y = GetRand(Game::kScreenHeight);
		break;
		//下方向から出てくる
	case 3:
		m_pos.x = GetRand(Game::kScreenWidth);
		m_pos.y = Game::kScreenHeight;
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
		m_hp = 15;
		m_atk = 2;
		m_spd = 0.2f;
		m_scale = kEnemySize;
		m_srcY = 0;
		m_kind = goblin;
		m_isBoss = false;
	}
	else if (kinds == static_cast<int>(boar))
	{
		m_hp = 7;
		m_atk = 1;
		m_spd = 0.4f;
		m_scale = kEnemySize;
		m_srcY = 1;
		m_kind = boar;
		m_isBoss = false;

	}
	else if (kinds == static_cast<int>(doragon))
	{
		m_hp = 20;
		m_atk = 10;
		m_spd = 0.1f;
		m_scale = kEnemySize;
		m_srcY = 2;
		m_kind = doragon;
		m_isBoss = false;

	}
	else if (kinds == static_cast<int>(skeleton))
	{
		m_hp = 15;
		m_atk = 3;
		m_spd = 0.3f;
		m_scale = kEnemySize;
		m_srcY = 3;
		m_kind = skeleton;
		m_isBoss = false;

	}
	else if (kinds == static_cast<int>(snowman))
	{
		m_hp = 50;
		m_atk = 5;
		m_spd = 0.1f;
		m_scale = kEnemySize;
		m_srcY = 4;
		m_kind = snowman;
		m_isBoss = false;

	}
	else if (kinds == static_cast<int>(bossGoblin))
	{
		m_hp = 45;
		m_atk = 4;
		m_spd = 0.2f;
		m_scale = kBossSize;
		m_srcY = 0;
		m_kind = bossGoblin;
		m_isBoss = true;
	}
	else if (kinds == static_cast<int>(bossBoar))
	{
		m_hp = 21;
		m_atk = 3;
		m_spd = 0.4f;
		m_scale = kBossSize;
		m_srcY = 1;
		m_kind = bossBoar;
		m_isBoss = true;

	}
	else if (kinds == static_cast<int>(bossDoragon))
	{
		m_hp = 60;
		m_atk = 25;
		m_spd = 0.1f;
		m_scale = kBossSize;
		m_srcY = 2;
		m_kind = bossDoragon;
		m_isBoss = true;

	}
	else if (kinds == static_cast<int>(bossSkeleton))
	{
		m_hp = 50;
		m_atk = 7;
		m_spd = 0.3f;
		m_scale = kBossSize;
		m_srcY = 3;
		m_kind = skeleton;
		m_isBoss = true;

	}
	else if (kinds == static_cast<int>(bossSnowman))
	{
		m_hp = 150;
		m_atk = 12;
		m_spd = 0.1f;
		m_scale = kBossSize;
		m_srcY = 4;
		m_kind = bossSnowman;
		m_isBoss = true;

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
					m_pMain->AddItem(pExp);
					//お金のメモリ確保
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//お金を落とす処理
					pGold->Init(m_pos);
					m_pMain->AddItem(pGold);
					if (GetRand(100) < kDropProb)
					{
						m_pTreasureBox = new TreasureBox(m_pMain);
						m_pTreasureBox->Init(m_pos);
						//メインに宝箱を生成する関数を作成する
						m_pMain->AddTreasure(m_pTreasureBox);
					}
					if (m_isBoss)
					{
						m_pMain->CountKillBoss();
					}
					//状態を変化させる
					m_nowState = Game::kDelete;
				}
			}
		}
		if (m_nowState == Game::kHitMagic)
		{
			m_hitMagicCount++;
			if (m_hitMagicCount > kHitMagicInterval)
			{
				m_nowState = Game::kNormal;
				m_hitMagicCount = 0;
			}
		}
		m_moveVec = m_targetPos - m_pos;
		m_moveVec.Normalize();
		m_moveVec *= m_spd;
		m_pos += m_moveVec;
		m_pos -= m_knockBack;
		m_circleCol.SetCenter(m_pos, m_radius * m_scale);
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
	//状態がkDeleteじゃない場合のみ動かす
	if (m_nowState != Game::kDelete)
	{

		int animEle = m_animFrame / kAnimFrameNum;

		int srcX = kGraphWidth * kUseFrame[animEle];
		int srcY = kGraphHeight * m_srcY;

		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			2.0 * m_scale,
			0.0,
			m_handle, true, m_isLeftFlag);
#ifdef _DEBUG
		m_circleCol.Draw(m_radius * m_scale, 0x0000ff, false);
#endif
	}
}

void Enemy::HitPlayer(Player& player)
{
	m_knockBack = player.GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale;
	m_hp -= player.GetAtk() - m_def;
}

void Enemy::HitMagic(MagicBase* magic)
{
	m_hp -= magic->GetAtk();
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
		pExp->Init(m_pos);
		m_pMain->AddItem(pExp);
		//お金のメモリ確保
		std::shared_ptr<Gold> pGold
			= std::make_shared<Gold>();
		//お金を落とす処理
		pGold->Init(m_pos);
		m_pMain->AddItem(pGold);
		if (GetRand(100) < kDropProb)
		{
			//宝箱のメモリ確保
			std::shared_ptr<TreasureBox> pTreasure
				= std::make_shared<TreasureBox>(m_pMain);
			//メインに宝箱を生成する関数を作成する
		}
		m_nowState = Game::kDelete;
	}
}
