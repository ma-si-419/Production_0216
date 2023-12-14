#include "TreasureBox.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "Portion.h"
#include "SceneMain.h"
namespace
{
	//落とす血の量
	constexpr int kDropBlood = 3;
	//落とすお金の量
	constexpr int kDropGold = 5;
	//落とす経験値の量
	constexpr int kDropExp = 5;
	//グラフィックの半分の大きさ
	constexpr int kHalfGraphSize = 20;
	//魔法にヒットするインターバル
	constexpr int kHitMagicInterval = 30;
}
TreasureBox::TreasureBox(SceneMain* sceneMain) :
	m_pMain(sceneMain),
	m_hp(GetRand(2) + 3),
	m_colScale(25),
	m_handle(-1),
	m_isExist(true)
{
}

TreasureBox::~TreasureBox()
{
}

void TreasureBox::Init(Vec2 pos)
{
	m_nowState = Game::kNormal;
	//全部同じ位置に出てしまわないように少しばらけさせる
	m_pos.x = pos.x + GetRand(30) - 15;
	m_pos.y = pos.y + GetRand(30) - 15;

	//画面外に出てしまわないようにする
	if (m_pos.x > Game::kScreenWidth)
	{
		m_pos.x = Game::kScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y座標も同じ処理を行う
	if (m_pos.y > Game::kScreenHeight)
	{
		m_pos.y = Game::kScreenHeight;
	}
	else if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}
}

void TreasureBox::Update()
{
	if (m_nowState != Game::kDelete)
	{


		m_circleCol.SetCenter(m_pos, m_colScale);
		if (m_hp < 0)
		{
			//どの処理が行われるか設定する
			int randomNumber = GetRand(3);

			if (randomNumber == 0)
			{
				for (int i = 0; i < kDropBlood; i++)
				{
					//血のメモリの確保
					std::shared_ptr<Blood> pBlood
						= std::make_shared<Blood>();
					//血の初期化処理
					pBlood->Init(m_pos);
					//血を落とす処理
					m_pMain->AddItem(pBlood);
				}
			}
			else if (randomNumber == 1)
			{
				for (int i = 0; i < kDropExp; i++)
				{
					//経験値のメモリの確保
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//経験値の初期化処理
					pExp->Init(m_pos);
					//経験値を落とす処理
					m_pMain->AddItem(pExp);
				}
			}
			else if (randomNumber == 2)
			{
				for (int i = 0; i < kDropGold; i++)
				{
					//お金のメモリ確保
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//お金の初期化処理
					pGold->Init(m_pos);
					//お金を落とす処理
					m_pMain->AddItem(pGold);
				}
			}
			else if (randomNumber == 3)
			{
				//体力回復アイテムを落とす処理を作る
				//お金のメモリ確保
				std::shared_ptr<Portion> pPortion
					= std::make_shared<Portion>();
				//お金の初期化処理
				pPortion->Init(m_pos);
				//お金を落とす処理
				m_pMain->AddItem(pPortion);
			}
			//状態を変化させる
			m_nowState = Game::kDelete;

		}
		if (m_nowState == Game::kHitMagic)
		{
			m_magicCount++;
			if (m_magicCount > kHitMagicInterval)
			{
				m_nowState = Game::kNormal;
				m_magicCount = 0;
			}
		}
	}
}

void TreasureBox::Draw()
{
	
	//色のついたBoxで仮実装
	DrawBox((int)m_pos.x - kHalfGraphSize, (int)m_pos.y - kHalfGraphSize,
		m_pos.x + kHalfGraphSize, m_pos.y + kHalfGraphSize,
		GetColor(255, 255, 0), true);
	//当たり判定の表示
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}

void TreasureBox::HitPlayer()
{
	m_hp--;
	//ノックバック処理を入れる
}

void TreasureBox::HitMagic()
{
	m_hp--;
}
