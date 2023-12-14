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
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	if (m_input.Buttons[12])
	{
		m_manager.ChangeScene(std::make_shared<SceneMain>(m_manager));
	}
}

void SceneTitle::Draw()
{
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2,
					"•P‚Æ‰¤Žq",GetColor(255,255,255), true);
}
