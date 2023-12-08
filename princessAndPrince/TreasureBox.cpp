#include "TreasureBox.h"
#include "DxLib.h"

TreasureBox::TreasureBox():
{
	m_hp = GetRand(2) + 3;
	m_colScale = 30;
}

TreasureBox::~TreasureBox()
{

}

void TreasureBox::Draw()
{
	//êFÇÃÇ¬Ç¢ÇΩBoxÇ≈âºé¿ëï
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(255, 255, 0), true);
}
