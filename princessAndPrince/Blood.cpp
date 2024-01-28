#include "Blood.h"
#include "DxLib.h"
Blood::Blood()
{
	m_kind = Game::kBlood;
}

Blood::~Blood()
{
}

void Blood::Draw()
{
	//àÍíUêFÇÃÇ¬Ç¢ÇΩBoxÇ≈âºé¿ëï
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale,m_pos.y + m_colScale,
		GetColor(255, 0, 0), true);
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}
