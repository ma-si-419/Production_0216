#include "Gold.h"
#include "DxLib.h"
namespace
{
	constexpr int kGraphSize = 32;
	constexpr double kGraphScale = 2.0;
}
Gold::Gold()
{
	m_kind = Game::ItemKinds::kGold;
}

Gold::~Gold()
{
}

void Gold::Draw()
{
	DrawRectRotaGraph(static_cast<int>(m_pos.x),static_cast<int>(m_pos.y),
		m_srcX, static_cast<int>(Game::ItemGraph::kDropCoin) * kGraphSize,
		kGraphSize, kGraphSize,
		kGraphScale,
		0.0,
		m_handle, true, false);
#ifdef _DEBUG
	m_circleCol.Draw(static_cast<int>(m_colScale), 0x0000ff, false);
#endif
}
