#include "TreasureBox.h"
#include "DxLib.h"
#include "SceneMain.h"

TreasureBox::TreasureBox(SceneMain* sceneMain):
	m_pSceneMain(m_pSceneMain),
	m_hp(GetRand(2) + 3),
	m_colScale(30),
	m_handle(-1),
	m_isExist(true)
{
}

TreasureBox::~TreasureBox()
{
}

void TreasureBox::Init(Vec2 pos)
{
	//�S�������ʒu�ɏo�Ă��܂�Ȃ��悤�ɏ����΂炯������
	m_pos.x = pos.x + GetRand(30) - 15;
	m_pos.y = pos.y + GetRand(30) - 15;

	//��ʊO�ɏo�Ă��܂�Ȃ��悤�ɂ���
	if (m_pos.x > Game::kScreenWidth)
	{
		m_pos.x = Game::kScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y���W�������������s��
	if (m_pos.y > Game::kScreenHeight)
	{
		m_pos.y = Game::kScreenHeight;
	}
	else if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}
	m_isExist = true;
}

void TreasureBox::Update()
{
}

void TreasureBox::Draw()
{
	//�F�̂���Box�ŉ�����
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(255, 255, 0), true);
}

void TreasureBox::OpenTreasure()
{
	
}
