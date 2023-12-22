#include "SceneTitle.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSelect.h"
SceneTitle::SceneTitle(SceneManager& manager) :
	Scene(manager),
	m_isKeyDown(false)
{
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
	//A�{�^�����A���ŉ�����Ȃ��悤��
	if (!m_input.Buttons[12])
	{
		m_isKeyDown = true;
	}
	 //A�{�^���������ꂽ��
	if (m_input.Buttons[12] && m_isKeyDown)
	{
		m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
	}
}

void SceneTitle::Draw()
{
	int DrawWidth = GetDrawStringWidth("�P�Ɖ��q", -1);

	DrawString((Game::kPlayScreenWidth - DrawWidth) / 2, Game::kPlayScreenHeight / 2,
		"�P�Ɖ��q", GetColor(255, 255, 255), true);
	DrawWidth = GetDrawStringWidth("PRESS START BUTTON", -1);
	DrawString((Game::kPlayScreenWidth - DrawWidth) / 2, 500,
		"PRESS START BUTTON", GetColor(255, 255, 255), true);
}
