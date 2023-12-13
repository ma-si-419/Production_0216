#include "MagicBase.h"
#include "Princess.h"
#include "DxLib.h"
namespace
{
	//基本的な炎魔法の攻撃力
	constexpr float kFireAttack = 3;
	//基本的な風魔法の攻撃力
	constexpr float kWindAttack = 1;
	//基本的な魔法の大きさ
	constexpr float kMagicScale = 20;
	//基本的な炎魔法のスピード
	constexpr float kMagicSpeed = 3;
	//風魔法を出す時にプレイヤーから座標をずらす時に使う
	constexpr float kShiftPosX = 50;
	//風魔法の円運動の大きくなる速度
	constexpr float kWindSpeed = 0.3f;
	//風魔法が回る速度
	constexpr float kWindSpinSpeed = 0.1f;

}

MagicBase::MagicBase(Princess* pPrincess) :
	//コンストラクタ時に存在しているフラグを立てる
	m_nowState(Game::kNormal),
	//プリンセスのポインタを入れる
	m_pPrincess(pPrincess),
	//移動ベクトルの初期化
	m_moveVec(0, 0),
	//ポジションの初期化
	m_princessPos(0, 0),
	m_magicPos(0,0),
	//炎魔法の攻撃力の初期化
	m_fireAtk(kFireAttack),
	//風魔法の攻撃力の初期化
	m_windAtk(kWindAttack),
	//魔法の大きさの初期化
	m_scale(kMagicScale),
	//最初に撃っている魔法を炎にする
	m_isFire(true),
	//魔法の速度の初期化
	m_spd(kMagicSpeed),
	//風魔法の変数の初期化
	m_windAngle(0),
	m_windLength(0),
	m_isRightWind(true)
{
}

MagicBase::~MagicBase()
{
}

void MagicBase::Init(int MagicNum)
{
	m_princessPos = m_pPrincess->GetPos();
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
	}
	// 縦軸の移動制限
	if (m_magicPos.y < 0 - m_scale)
		m_nowState = Game::kDelete;
	else if (Game::kScreenHeight + m_scale < m_magicPos.y)
		m_nowState = Game::kDelete;
	// 横軸の移動制限
	if (m_magicPos.x < 0 - m_scale)
		m_nowState = Game::kDelete;
	else if (Game::kScreenWidth + m_scale < m_magicPos.x)
		m_nowState = Game::kDelete;
	m_circleCol.SetCenter(m_magicPos, kMagicScale);

}

void MagicBase::Draw()
{
	if (m_isFire)
	{
			DrawBox(m_magicPos.x - m_scale / 2, m_magicPos.y - m_scale / 2,
				m_magicPos.x + m_scale / 2, m_magicPos.y + m_scale / 2,
			GetColor(255, 0, 0), true);
	}
	else if (!m_isFire)
	{
		DrawBox(m_magicPos.x - m_scale / 2, m_magicPos.y - m_scale / 2,
			m_magicPos.x + m_scale / 2, m_magicPos.y + m_scale / 2,
			GetColor(0, 255, 0), true);
	}
	m_circleCol.Draw(kMagicScale,0,0);
}

float MagicBase::GetAtk()
{
	if (m_isFire)
	{
		return m_fireAtk;
	}
	else
	{
		return m_windAtk;
	}
}
