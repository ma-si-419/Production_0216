#include "UI.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
UI::UI(Player* pPlayer):
	m_pPlayer(pPlayer)
{
}

UI::~UI()
{
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw()
{
	//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
	DrawFormatString(Game::kPlayScreenWIdth - 200, 0,//���W
		GetColor(0, 0, 0), "�擾�S�[���h%d", m_pPlayer->GetGold());

	//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������
	DrawFormatString(Game::kPlayScreenWIdth - 200, 100,//���W
		GetColor(0, 0, 0), "�擾�o���l%d", m_pPlayer->GetExp());
}
