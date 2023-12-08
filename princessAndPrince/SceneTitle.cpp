#include "SceneTitle.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
SceneTitle::SceneTitle(SceneManager& manager):
	Scene(manager),
	m_handle(0)
{
	m_handle = LoadGraph("data/Image/_Start.png");
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::Update(Pad& pad)
{
	if (CheckHitKey(KEY_INPUT_Z))
	{
		m_manager.ChangeScene(std::make_shared<SceneMain>(m_manager));
	}
}

void SceneTitle::Draw()
{
	DrawGraph(225, 200, m_handle,true);
}
