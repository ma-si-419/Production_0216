#include "Fade.h"
#include "DxLib.h"
#include "Game.h"
namespace
{
	//フェードインするフレーム数
	constexpr int kFadeFrameNum = 20;
}
Fade::Fade() :
	m_alphaNum(0),
	m_fadeState(fadeState::kFadeIn)
{
}

Fade::~Fade()
{
}

void Fade::Update()
{
	//フェードアウトフラッグが立っていたら
	if (m_fadeState == fadeState::kFadeOut)
	{
		//最大値をフレーム数で割った数を足していく
		m_alphaNum += 255 / kFadeFrameNum;
		//最大値を超えたら限界値に戻す
		if (m_alphaNum > 255)
		{
			m_alphaNum = 255;
		}
	}
	//フェードインフラグが立っていたら
	if (m_fadeState == fadeState::kFadeIn)
	{
		//最大値をフレーム数で割った数を引いていく
		m_alphaNum -= 255 / kFadeFrameNum;
		//最低値を下回ったら最低値に戻す
		if (m_alphaNum < 0)
		{
			m_alphaNum = 0;
		}
	}
	//アルファが最大値か最低値だったら処理が完了したフラグを立てる
	if (m_alphaNum == 255)
	{
		m_fadeState = fadeState::kFadeIn;
	}
	if (m_alphaNum == 0)
	{
		m_fadeState = fadeState::kNormal;
	}
}

void Fade::Draw()
{
	//画面全体に黒い箱を出す
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaNum);
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}	
