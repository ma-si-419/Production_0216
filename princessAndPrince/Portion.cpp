#include "Portion.h"
#include "DxLib.h"


Portion::Portion()
{
	m_kind = Game::ItemKinds::kPortion;
}

Portion::~Portion()
{
}

void Portion::Draw()
{
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, static_cast<int>(Game::ItemGraph::kDropPortion) * 32,
		32, 32,
		2.0,
		0.0,
		m_handle, true, false);
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}
