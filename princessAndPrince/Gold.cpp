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
	//êFÇÃÇ¬Ç¢ÇΩBoxÇ≈âºé¿ëï
	DrawBox((int)m_pos.x - (m_colScale / 2), (int)m_pos.y - (m_colScale / 2),
		m_pos.x + (m_colScale / 2), m_pos.y + (m_colScale / 2),
		GetColor(255, 255, 100), true);
}
