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
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(0, 255, 0), true);
}
