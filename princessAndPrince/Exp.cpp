#include "Exp.h"
#include "DxLib.h"

Exp::Exp()
{
	m_kind = Game::kExp;
}

Exp::~Exp()
{
}

void Exp::Draw()
{
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, Game::kDropExp * 32,
		32, 32,
		2.0,
		0.0,
		m_handle, true, false);
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}
