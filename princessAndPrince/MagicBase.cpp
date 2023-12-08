#include "MagicBase.h"
#include "Princess.h"
#include "DxLib.h"
namespace
{
	//��{�I�Ȗ��@�̍U����
	constexpr int kMagicAttack = 3;
	//��{�I�Ȗ��@�̑傫��
	constexpr float kMagicScale = 20;
	//��{�I�ȉ����@�̃X�s�[�h
	constexpr float kMagicSpeed = 3;
	//�����@���o�����Ƀv���C���[������W�����炷���Ɏg��
	constexpr float kShiftPosX = 50;
	//�����@�̉~�^���̑傫���Ȃ鑬�x
	constexpr float kWindSpeed = 0.12f;
	//�����@����鑬�x
	constexpr float kWindSpinSpeed = 0.05f;

}

MagicBase::MagicBase(Princess* pPrincess) :
	//�R���X�g���N�^���ɑ��݂��Ă���t���O�𗧂Ă�
	m_isExist(true),
	//�v�����Z�X�̃|�C���^������
	m_pPrincess(pPrincess),
	//�ړ��x�N�g���̏�����
	m_moveVec(0, 0),
	//�|�W�V�����̏�����
	m_princessPos(0, 0),
	m_magicPos(0,0),
	//�U���͂̏�����
	m_atk(kMagicAttack),
	//���@�̑傫���̏�����
	m_scale(kMagicScale),
	//�ŏ��Ɍ����Ă��閂�@�����ɂ���
	m_isFire(true),
	//���@�̑��x�̏�����
	m_spd(kMagicSpeed),
	//�����@�̕ϐ��̏�����
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
	}
	// �c���̈ړ�����
	if (m_magicPos.y < 0 - m_scale)
		m_isExist = false;
	else if (Game::kScreenHeight + m_scale < m_magicPos.y)
		m_isExist = false;
	// �����̈ړ�����
	if (m_magicPos.x < 0 - m_scale)
		m_isExist = false;
	else if (Game::kScreenWidth + m_scale < m_magicPos.x)
		m_isExist = false;
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
}
