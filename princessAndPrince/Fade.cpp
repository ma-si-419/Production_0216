#include "Fade.h"
#include "DxLib.h"
#include "Game.h"
Fade::Fade()
{
}

Fade::~Fade()
{
}

void Fade::Update()
{
}

void Fade::Draw()
{
	//âÊñ ëSëÃÇ…çïÇ¢î†ÇèoÇ∑
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaNum);
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}	

bool Fade::FadeIn()
{
	m_alphaNum--;
	if (m_alphaNum <= 0)
	{
		m_alphaNum = 0;
		return true;
	}
	return false;
}

bool Fade::FadeOut()
{
	m_alphaNum++;
	if (m_alphaNum >= 255)
	{
		m_alphaNum = 255;
		return true;
	}
	return false;
}
