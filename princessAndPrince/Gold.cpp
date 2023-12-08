#include "Gold.h"
#include "DxLib.h"

Gold::Gold()
{
	m_kind = Game::kGold;
}

Gold::~Gold()
{
}

void Gold::Draw()
{
	//色のついたBoxで仮実装
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(255, 255, 100), true);
}
