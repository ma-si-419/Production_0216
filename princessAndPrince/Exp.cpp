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
	//êFÇÃÇ¬Ç¢ÇΩBoxÇ≈âºé¿ëï
	DrawBox((int)m_pos.x - (m_colScale / 2), (int)m_pos.y - (m_colScale / 2),
		m_pos.x + (m_colScale / 2), m_pos.y + (m_colScale / 2),
		GetColor(0, 255, 0), true);
}
