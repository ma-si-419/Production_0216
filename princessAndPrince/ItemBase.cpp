#include "ItemBase.h"
#include"DxLib.h"
#include"Player.h"
namespace
{
	constexpr int kColScale = 10;
}
ItemBase::ItemBase() :
	m_handle(0),
	m_kind(Game::kEmpty),
	m_pos(0, 0),
	m_isExist(false),
	m_colScale(kColScale),
	m_nowState(Game::kNormal)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init(Vec2 pos)
{
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