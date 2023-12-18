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
	//入手したゴールドの量をプレイヤーから取得して表示する
	DrawFormatString(Game::kPlayScreenWIdth - 200, 0,//座標
		GetColor(0, 0, 0), "取得ゴールド%d", m_pPlayer->GetGold());

	//入手した経験値の量をプレイヤーから取得して表示する
	DrawFormatString(Game::kPlayScreenWIdth - 200, 100,//座標
		GetColor(0, 0, 0), "取得経験値%d", m_pPlayer->GetExp());
}
