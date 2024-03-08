#include "Portion.h"
#include "DxLib.h"
namespace
{
	constexpr int kGraphSize = 32;
	constexpr double kGraphScale = 2.0;
}
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
		0, static_cast<int>(Game::ItemGraph::kDropPortion) * kGraphSize,
		kGraphSize, kGraphSize,
		kGraphScale,
		0.0,
		m_handle, true, false);
#ifdef _DEBUG
	m_circleCol.Draw(static_cast<int>(m_colScale), 0x0000ff, false);
#endif
}
