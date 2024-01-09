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
	//àÍíUêFÇÃÇ¬Ç¢ÇΩBoxÇ≈âºé¿ëï
	DrawBox((int)m_pos.x - (m_colScale / 2), (int)m_pos.y - (m_colScale / 2),
		m_pos.x + (m_colScale / 2), m_pos.y + (m_colScale / 2),
		GetColor(120,120, 255), true);
}
