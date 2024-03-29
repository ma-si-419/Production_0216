#include "MagicBase.h"
#include "Princess.h"
#include "DxLib.h"
#include "UserData.h"
#include "cmath"
namespace
{

	//基本的な炎魔法のスピード
	constexpr float kMagicSpeed = 3;
	//風魔法を出す時にプレイヤーから座標をずらす時に使う
	constexpr float kShiftPosX = 50;
	//風魔法の円運動の大きくなる速度
	constexpr float kWindSpeed = 0.3f;
	//風魔法が回る速度
	constexpr float kWindSpinSpeed = 0.1f;
	//グラフィックの大きさ
	constexpr int kGraphSize = 32;
	//魔法の大きさ
	constexpr float kMagicSize = 2.0f;
	//炎魔法の基礎攻撃力
	constexpr float kInitialFireAtk = 3.0f;
	//風魔法の基礎攻撃力
	constexpr float kInitialWindAtk = 0.15f;
	//炎魔法のレベルボーナス
	constexpr float kFireLevelBonus = 1.5f;
	//風魔法のレベルボーナス
	constexpr float kWindLevelBonus = 0.15f;

}

MagicBase::MagicBase(Princess* pPrincess, float scale) :
	//コンストラクタ時に存在しているフラグを立てる
	m_nowState(Game::State::kNormal),
	//プリンセスのポインタを入れる
	m_pPrincess(pPrincess),
	//移動ベクトルの初期化
	m_moveVec(0, 0),
	//ポジションの初期化
	m_princessPos(m_pPrincess->GetPos()),
	m_magicPos(m_princessPos),
	//炎魔法の攻撃力の初期化
	m_fireAtk(UserData::userFireLevel * kFireLevelBonus + kInitialFireAtk),
	//風魔法の攻撃力の初期化
	m_windAtk(UserData::userWindLevel * kWindLevelBonus + kInitialWindAtk),
	//魔法の大きさの初期化
	m_scale(scale),
	//最初に撃っている魔法を炎にする
	m_isFire(true),
	//魔法の速度の初期化
	m_spd(kMagicSpeed),
	//風魔法の変数の初期化
	m_windAngle(0),
	m_windLength(0),
	m_isRightWind(true),
	m_graphAngle(0.0f),
	m_handle(0),
	m_turnFlag(false)
{
}

MagicBase::~MagicBase()
{
}

void MagicBase::Init(int MagicNum)
{
	//炎攻撃を行っていたら
	if (m_pPrincess->GetMagicKind())
	{
		m_magicPos = m_princessPos;
		m_isFire = m_pPrincess->GetMagicKind();
		m_moveVec = m_pPrincess->GetMagicDir();
		m_moveVec = m_moveVec - m_princessPos;
		m_moveVec.Normalize();
		m_moveVec *= m_spd;
	}
	else//風攻撃を行っていたら
	{
		m_isFire = m_pPrincess->GetMagicKind();
		//y座標はずらす必要がないため、魔女の座標を入れる
		m_magicPos.y = m_princessPos.y;
		//二つ同時に出すため、重ならないようにx座標をずらす
		if (MagicNum == 0)
		{
			m_magicPos.x = m_princessPos.x + kShiftPosX;
			m_isRightWind = true;
		}
		else
		{
			m_magicPos.x = m_princessPos.x - kShiftPosX;
			m_isRightWind = false;
		}
	}
}

void MagicBase::Update()
{
	m_magicPos += m_moveVec;
	m_graphAngle = -std::atan2(m_moveVec.x, m_moveVec.y);
	//風魔法を行っていたら
	if (!m_isFire)
	{
		m_windAngle += kWindSpinSpeed;
		m_windLength += kWindSpeed;
		m_moveVec.x = cosf(m_windAngle);
		m_moveVec.y = sinf(m_windAngle);
		if (m_isRightWind)
		{
			m_moveVec *= m_windLength;
		}
		else
		{
			m_moveVec *= -m_windLength;
		}
		if (m_magicPos.x > Game::kPlayScreenWidth / 2)
		{
			m_turnFlag = true;
		}
		else
		{
			m_turnFlag = false;
		}
	}
	// 縦軸の移動制限
	if (m_magicPos.y < 0 - m_scale)
		m_nowState = Game::State::kDelete;
	else if (Game::kPlayScreenHeight + m_scale < m_magicPos.y)
		m_nowState = Game::State::kDelete;
	// 横軸の移動制限
	if (m_magicPos.x < 0 - m_scale)
		m_nowState = Game::State::kDelete;
	else if (Game::kPlayScreenWidth + m_scale < m_magicPos.x)
		m_nowState = Game::State::kDelete;
	m_circleCol.SetCenter(m_magicPos, m_scale);

}

void MagicBase::Draw()
{
	if (m_isFire)
	{
		DrawRectRotaGraph(static_cast<int>(m_magicPos.x), static_cast<int>(m_magicPos.y),
			0, static_cast<int>(Game::ItemGraph::kFire) * kGraphSize,
			kGraphSize, kGraphSize,
			kMagicSize,
			m_graphAngle,
			m_handle,
			true,
			0, 0);
	}
	else if (!m_isFire)
	{
		DrawRectRotaGraph(static_cast<int>(m_magicPos.x), static_cast<int>(m_magicPos.y),
			0, static_cast<int>(Game::ItemGraph::kWind) * kGraphSize,
			kGraphSize, kGraphSize,
			kMagicSize,
			0,
			m_handle,
			true,
			m_turnFlag, 0);
	}
#ifdef _DEBUG
	m_circleCol.Draw(static_cast<int>(m_scale), 0, 0);

#endif // _DEBUG

}

float MagicBase::GetAtk()
{
	if (m_isFire)
	{
		return m_pPrincess->GetAtk() + m_fireAtk;
	}
	else
	{
		return m_pPrincess->GetAtk() + m_windAtk;
	}
}
