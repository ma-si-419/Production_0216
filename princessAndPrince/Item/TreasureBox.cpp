#include "TreasureBox.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "Portion.h"
#include "SceneMain.h"
#include "Player.h"
#include "Particle.h"
namespace
{
	//落とす血の量
	constexpr int kDropBlood = 3;
	//落とすお金の量
	constexpr int kDropGold = 5;
	//落とす経験値の量
	constexpr int kDropExp = 5;
	//グラフィックの半分の大きさ
	constexpr int kHalfGraphSize = 40;
	//宝箱の大きさ
	constexpr int kColScale = 30;
	//魔法にヒットするインターバル
	constexpr int kHitMagicInterval = 30;
	//ノックバックの大きさ
	constexpr int kKnockBackScale = 3;
	//パーティクルの数
	constexpr int kParticleVol = 30;
	//ノックバックの大きさ
	constexpr float kKnockBackPow = 3;
	//ノックバックする時間
	constexpr int kKnockBackTime = 5;
	//血を落とさないシーン
	constexpr int kNotDropBloodScene = 2;
	//基本的なHP
	constexpr int kBaseHp = 5;;
	//HPの振れ幅
	constexpr int kHpLange = 3;
	//プレイヤーとぶつかった時の基本的なダメージ
	constexpr int kBaseDamage = 2;
	//プレイヤーとぶつかった時のダメージの振れ幅
	constexpr int kDamageLange = 5;
	//基本的な落とすものの種類数
	constexpr int kBaseDropKind = 3;
	//宝箱の画像サイズ
	constexpr int kGraphSize = 32;
	//宝箱の拡大率
	constexpr double kGraphScale = 2.0;
	//アイテムを落とす時の落とす範囲
	constexpr int kDropPosLange = 30;
	//ぶつかった時に出すパーティクルの情報
	constexpr int kParticleInfo[4] = { 40, 4, 5, 0 };
}
TreasureBox::TreasureBox(SceneMain* sceneMain) :
	m_pMain(sceneMain),
	m_hp(GetRand(kHpLange) + kBaseHp),
	m_colScale(kColScale),
	m_handle(-1),
	m_isExist(true),
	m_knockBackVec(),
	m_knockBackPow(kKnockBackPow),
	m_exp(0),
	m_gold(0),
	m_knockBackTime(0),
	m_magicCount(0),
	m_nowState(Game::State::kNormal),
	m_pParticle()
{
}

TreasureBox::~TreasureBox()
{
}

void TreasureBox::Init(Vec2 pos)
{
	m_nowState = Game::State::kNormal;
	//全部同じ位置に出てしまわないように少しばらけさせる
	m_pos.x = pos.x + GetRand(kDropPosLange) - kDropPosLange / 2;
	m_pos.y = pos.y + GetRand(kDropPosLange) - kDropPosLange / 2;

	//画面外に出てしまわないようにする
	if (m_pos.x > Game::kPlayScreenWidth)
	{
		m_pos.x = Game::kPlayScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y座標も同じ処理を行う
	if (m_pos.y > Game::kPlayScreenHeight)
	{
		m_pos.y = Game::kPlayScreenHeight;
	}
	else if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}
}

void TreasureBox::Update()
{
	if (m_nowState != Game::State::kDelete)
	{
		m_pos -= m_knockBackVec;
		//ノックバック処理
		if (m_knockBackVec.x != 0 || m_knockBackVec.y != 0)
		{
			//ノックバックする時間カウント
			m_knockBackTime++;
			//規定の時間を過ぎたら
			if (m_knockBackTime > kKnockBackTime)
			{
				//ノックバックの大きさを0にする
				m_knockBackVec *= 0;
				//時間カウントを0にする
				m_knockBackTime = 0;
			}
		}
		m_circleCol.SetCenter(m_pos, static_cast<float>(m_colScale));
		if (m_hp < 0)
		{
			//どの処理が行われるか設定する
			int randomNumber;
			//2ステージまでは血を落とさないようにする
			if (m_pMain->GetSceneNum() < kNotDropBloodScene)
			{
				//血を落とさないように1減らす
				randomNumber = GetRand(kBaseDropKind -1 );
			}
			else
			{
				randomNumber = GetRand(kBaseDropKind);
			}
			if (randomNumber == static_cast<int>(Dropkind::kPortion))
			{
				//体力回復アイテムを落とす処理を作る
				//ポーションのメモリ確保
				std::shared_ptr<Portion> pPortion
					= std::make_shared<Portion>();
				//ポーションの初期化処理
				pPortion->Init(m_pos);
				pPortion->SetHandle(m_handle);
				//ポーションを落とす処理
				m_pMain->AddItem(pPortion);
			}
			else if (randomNumber == static_cast<int>(Dropkind::kBlood))
			{
				for (int i = 0; i < kDropBlood; i++)
				{
					//血のメモリの確保
					std::shared_ptr<Blood> pBlood
						= std::make_shared<Blood>();
					//血の初期化処理
					pBlood->Init(m_pos);
					pBlood->SetHandle(m_handle);
					//血を落とす処理
					m_pMain->AddItem(pBlood);
				}
			}
			else if (randomNumber == static_cast<int>(Dropkind::kExp))
			{
				for (int i = 0; i < kDropExp; i++)
				{
					//経験値のメモリの確保
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//経験値の初期化処理
					pExp->Init(m_pos);
					pExp->SetExp(m_exp);
					pExp->SetHandle(m_handle);
					//経験値を落とす処理
					m_pMain->AddItem(pExp);
				}
			}
			else if (randomNumber == static_cast<int>(Dropkind::kGold))
			{
				for (int i = 0; i < kDropGold; i++)
				{
					//お金のメモリ確保
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//お金の初期化処理
					pGold->Init(m_pos);
					pGold->SetPrice(m_gold);
					pGold->SetHandle(m_handle);

					//お金を落とす処理
					m_pMain->AddItem(pGold);
				}
			}
			//状態を変化させる
			m_nowState = Game::State::kDelete;

		}
		if (m_nowState == Game::State::kHitMagic)
		{
			m_magicCount++;
			if (m_magicCount > kHitMagicInterval)
			{
				m_nowState = Game::State::kNormal;
				m_magicCount = 0;
			}
		}
	}
}

void TreasureBox::Draw()
{

	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, static_cast<int>(Game::ItemGraph::kDropTreasure) * kGraphSize,
		kGraphSize, kGraphSize,
		kGraphScale,
		0.0,
		m_handle, true, false);
	//当たり判定の表示
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}

void TreasureBox::HitPlayer(Player* player)
{
	m_hp -= GetRand(kDamageLange) + kBaseDamage;
	//ノックバック処理を入れる
	m_knockBackVec = player->GetMoveVec() * -1;
	m_knockBackVec.Normalize();
	m_knockBackVec *= kKnockBackScale;
	//衝突点の座標を出す
	m_hitPos = (player->GetPos() + m_pos) / 2;
	//白いエフェクトを出す
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_hitPos, static_cast<float>(kParticleInfo[0]), static_cast<float>(kParticleInfo[1]), kParticleInfo[2], kParticleInfo[3]);
		m_pMain->AddParticle(m_pParticle);
	}
}

void TreasureBox::HitMagic()
{
	m_nowState = Game::State::kHitMagic;
	//白いエフェクトを出す
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_pos, static_cast<float>(kParticleInfo[0]), static_cast<float>(kParticleInfo[1]), kParticleInfo[2], kParticleInfo[3]);
		m_pMain->AddParticle(m_pParticle);
	}
	m_hp--;
	if (m_hp < 0)
	{
		//どの処理が行われるか設定する
		int randomNumber = GetRand(kBaseDropKind);

		if (randomNumber == static_cast<int>(Dropkind::kPortion))
		{
			//体力回復アイテムを落とす処理を作る
			//ポーションのメモリ確保
			std::shared_ptr<Portion> pPortion
				= std::make_shared<Portion>();
			//ポーションの初期化処理
			pPortion->Init(m_pos);
			pPortion->SetHandle(m_handle);
			//ポーションを落とす処理
			m_pMain->AddItem(pPortion);
		}
		else if (randomNumber == static_cast<int>(Dropkind::kBlood))
		{
			for (int i = 0; i < kDropBlood; i++)
			{
				//血のメモリの確保
				std::shared_ptr<Blood> pBlood
					= std::make_shared<Blood>();
				//血の初期化処理
				pBlood->Init(m_pos);
				pBlood->SetHandle(m_handle);
				//血を落とす処理
				m_pMain->AddItem(pBlood);
			}
		}
		else if (randomNumber == static_cast<int>(Dropkind::kExp))
		{
			for (int i = 0; i < kDropExp; i++)
			{
				//経験値のメモリの確保
				std::shared_ptr<Exp> pExp
					= std::make_shared<Exp>();
				//経験値の初期化処理
				pExp->Init(m_pos);
				pExp->SetExp(m_exp);
				pExp->SetHandle(m_handle);
				//経験値を落とす処理
				m_pMain->AddItem(pExp);
			}
		}
		else if (randomNumber == static_cast<int>(Dropkind::kGold))
		{
			for (int i = 0; i < kDropGold; i++)
			{
				//お金のメモリ確保
				std::shared_ptr<Gold> pGold
					= std::make_shared<Gold>();
				//お金の初期化処理
				pGold->Init(m_pos);
				pGold->SetPrice(m_gold);
				pGold->SetHandle(m_handle);
				//お金を落とす処理
				m_pMain->AddItem(pGold);
			}
		}
		//状態を変化させる
		m_nowState = Game::State::kDelete;

	}
}
