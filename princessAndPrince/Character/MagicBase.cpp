#include "MagicBase.h"
#include "Princess.h"
#include "DxLib.h"
#include "UserData.h"
#include "cmath"
namespace
{

	//��{�I�ȉ����@�̃X�s�[�h
	constexpr float kMagicSpeed = 3;
	//�����@���o�����Ƀv���C���[������W�����炷���Ɏg��
	constexpr float kShiftPosX = 50;
	//�����@�̉~�^���̑傫���Ȃ鑬�x
	constexpr float kWindSpeed = 0.3f;
	//�����@����鑬�x
	constexpr float kWindSpinSpeed = 0.1f;
	//�O���t�B�b�N�̑傫��
	constexpr int kGraphSize = 32;
	//���@�̑傫��
	constexpr float kMagicSize = 2.0f;
	//�����@�̊�b�U����
	constexpr float kInitialFireAtk = 3.0f;
	//�����@�̊�b�U����
	constexpr float kInitialWindAtk = 0.15f;
	//�����@�̃��x���{�[�i�X
	constexpr float kFireLevelBonus = 1.5f;
	//�����@�̃��x���{�[�i�X
	constexpr float kWindLevelBonus = 0.15f;

}

MagicBase::MagicBase(Princess* pPrincess, float scale) :
	//�R���X�g���N�^���ɑ��݂��Ă���t���O�𗧂Ă�
	m_nowState(Game::State::kNormal),
	//�v�����Z�X�̃|�C���^������
	m_pPrincess(pPrincess),
	//�ړ��x�N�g���̏�����
	m_moveVec(0, 0),
	//�|�W�V�����̏�����
	m_princessPos(m_pPrincess->GetPos()),
	m_magicPos(m_princessPos),
	//�����@�̍U���͂̏�����
	m_fireAtk(UserData::userFireLevel * kFireLevelBonus + kInitialFireAtk),
	//�����@�̍U���͂̏�����
	m_windAtk(UserData::userWindLevel * kWindLevelBonus + kInitialWindAtk),
	//���@�̑傫���̏�����
	m_scale(scale),
	//�ŏ��Ɍ����Ă��閂�@�����ɂ���
	m_isFire(true),
	//���@�̑��x�̏�����
	m_spd(kMagicSpeed),
	//�����@�̕ϐ��̏�����
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
	//���U�����s���Ă�����
	if (m_pPrincess->GetMagicKind())
	{
		m_magicPos = m_princessPos;
		m_isFire = m_pPrincess->GetMagicKind();
		m_moveVec = m_pPrincess->GetMagicDir();
		m_moveVec = m_moveVec - m_princessPos;
		m_moveVec.Normalize();
		m_moveVec *= m_spd;
	}
	else//���U�����s���Ă�����
	{
		m_isFire = m_pPrincess->GetMagicKind();
		//y���W�͂��炷�K�v���Ȃ����߁A�����̍��W������
		m_magicPos.y = m_princessPos.y;
		//������ɏo�����߁A�d�Ȃ�Ȃ��悤��x���W�����炷
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
	//�����@���s���Ă�����
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
	// �c���̈ړ�����
	if (m_magicPos.y < 0 - m_scale)
		m_nowState = Game::State::kDelete;
	else if (Game::kPlayScreenHeight + m_scale < m_magicPos.y)
		m_nowState = Game::State::kDelete;
	// �����̈ړ�����
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