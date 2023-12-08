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
	//全部同じ位置に出てしまわないように少しばらけさせる
	m_pos.x = pos.x + GetRand(30) - 15;
	m_pos.y = pos.y + GetRand(30) - 15;

	//画面外に出てしまわないようにする
	if (m_pos.x > Game::kScreenWidth)
	{
		m_pos.x = Game::kScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y座標も同じ処理を行う
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
	//色のついたBoxで仮実装
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(255, 255, 0), true);
}

void TreasureBox::OpenTreasure()
{
	
}
