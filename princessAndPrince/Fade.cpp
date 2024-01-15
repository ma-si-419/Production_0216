#include "Fade.h"
#include "DxLib.h"
#include "Game.h"
namespace
{
	//�t�F�[�h�C������t���[����
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
	//�t�F�[�h�A�E�g�t���b�O�������Ă�����
	if (m_fadeState == fadeState::kFadeOut)
	{
		//�ő�l���t���[�����Ŋ��������𑫂��Ă���
		m_alphaNum += 255 / kFadeFrameNum;
		//�ő�l�𒴂�������E�l�ɖ߂�
		if (m_alphaNum > 255)
		{
			m_alphaNum = 255;
		}
	}
	//�t�F�[�h�C���t���O�������Ă�����
	if (m_fadeState == fadeState::kFadeIn)
	{
		//�ő�l���t���[�����Ŋ��������������Ă���
		m_alphaNum -= 255 / kFadeFrameNum;
		//�Œ�l�����������Œ�l�ɖ߂�
		if (m_alphaNum < 0)
		{
			m_alphaNum = 0;
		}
	}
	//�A���t�@���ő�l���Œ�l�������珈�������������t���O�𗧂Ă�
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
	//��ʑS�̂ɍ��������o��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaNum);
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}	
