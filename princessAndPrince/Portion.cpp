#include "Portion.h"
#include "DxLib.h"


Portion::Portion()
{
	m_kind = Game::kPortion;
}

Portion::~Portion()
{
}

void Portion::Draw()
{
	//一旦色のついたBoxで仮実装
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(120,120, 255), true);
}
