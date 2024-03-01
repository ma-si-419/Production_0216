#include "ItemBase.h"
#include"DxLib.h"
#include"Player.h"
namespace
{
	//�A�C�e���̑傫��
	constexpr float kColScale = 18.0f;

	//UI�̏ꏊ
	constexpr int kUiPosX = Game::kScreenWidth - 300;
	constexpr int kUiPosY = 150;

	//�A�C�e���������X�s�[�h
	constexpr int kSpeed = 35;
	//�A�C�e�����΂炯������
	constexpr int kShiftLange = 70;
	//�A�C�e����\������UI�̃|�W�V����
	constexpr int kUiPos = Game::kScreenWidth - 300;
}
ItemBase::ItemBase() :
	m_handle(0),
	m_kind(Game::ItemKinds::kEmpty),
	m_pos(0, 0),
	m_isExist(false),
	m_colScale(kColScale),
	m_nowState(Game::State::kNormal),
	m_haveExp(0),
	m_haveGold(0),
	m_moveVec(0, 0),
	m_pPlayer(),
	m_srcX(0)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init(Vec2 pos)
{
	//�S�������ʒu�ɏo�Ă��܂�Ȃ��悤�ɏ����΂炯������
	m_pos.x = pos.x + GetRand(kShiftLange) - kShiftLange / 2;
	m_pos.y = pos.y + GetRand(kShiftLange) - kShiftLange / 2;
	//��ʊO�ɏo�Ă��܂�Ȃ��悤�ɂ���
	if (m_pos.x > Game::kPlayScreenWidth)
	{
		m_pos.x = Game::kPlayScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y���W�������������s��
	if (m_pos.y > Game::kPlayScreenHeight)
	{
		m_pos.y = Game::kPlayScreenHeight;
	}
	else if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}
	m_isExist = true;
}

void ItemBase::Update()
{
	m_circleCol.SetCenter(m_pos, m_colScale);
	m_pos += m_moveVec;
	//�S�[���h�ƌo���l�͉E���UI�܂œ�����
	if (m_pos.x > kUiPos)
	{
		m_nowState = Game::State::kDelete;
		if (m_kind == Game::ItemKinds::kGold)
		{
			m_pPlayer->PickUpGold(m_haveGold);
		}
		if (m_kind == Game::ItemKinds::kExp)
		{
			m_pPlayer->PickUpExp(m_haveExp);
		}
	}
}

void ItemBase::Draw()
{
#ifdef _DEBUG
	m_circleCol.Draw(static_cast<int>(m_colScale), 0x0000ff, false);
#endif
}

void ItemBase::MoveItem(Player* player)
{
	Vec2 temp;
	temp.x = kUiPosX;
	temp.y = kUiPosY;
	m_moveVec = temp - m_pos;
	m_moveVec.Normalize();
	m_moveVec *= kSpeed;
	m_pPlayer = player;
}
