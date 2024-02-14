#include "Blood.h"
#include "DxLib.h"
Blood::Blood()
{
	m_kind = Game::ItemKinds::kBlood;
}

Blood::~Blood()
{
}

void Blood::Draw()
{
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, static_cast<int>(Game::ItemGraph::kDropBlood) * 32,
		32, 32,
		2.0,
		0.0,
		m_handle, true, false);
#ifdef _DEBUG
	m_circleCol.Draw(static_cast<int>(m_colScale), 0x0000ff, false);
#endif
}
