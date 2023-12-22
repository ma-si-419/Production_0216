#include "ItemBase.h"
#include"DxLib.h"
#include"Player.h"
namespace
{
	constexpr float kColScale = 10.0f;
}
ItemBase::ItemBase() :
	m_handle(0),
	m_kind(Game::kEmpty),
	m_pos(0, 0),
	m_isExist(false),
	m_colScale(kColScale),
	m_nowState(Game::kNormal),
	m_haveExp(0),
	m_haveGold(0)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init(Vec2 pos)
{
	//�S�������ʒu�ɏo�Ă��܂�Ȃ��悤�ɏ����΂炯������
	m_pos.x = pos.x + GetRand(50) - 25;
	m_pos.y = pos.y + GetRand(50) - 25;
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
}

void ItemBase::Draw()
{
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}