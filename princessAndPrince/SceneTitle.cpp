#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSelect.h"
SceneTitle::SceneTitle(SceneManager& sceneManager,DataManager& DataManager) :
	Scene(sceneManager,DataManager),
	m_isKeyDown(true),
	m_handle(-1)
{
	m_appSe = DataManager.SearchSound("approveSe");
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
	 //Aƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚ç
	if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown || CheckHitKey(KEY_INPUT_RETURN) && m_isKeyDown)
	{
		PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));
		m_isKeyDown = false;
	}
}

void SceneTitle::Draw()
{
	int DrawWidth = GetDrawStringWidth("•P‚Æ‰¤Žq", -1);

	DrawString((Game::kPlayScreenWidth - DrawWidth) / 2, Game::kPlayScreenHeight / 2,
		"•P‚Æ‰¤Žq", GetColor(255, 255, 255), true);
	DrawWidth = GetDrawStringWidth("PRESS START BUTTON", -1);
	DrawString((Game::kPlayScreenWidth - DrawWidth) / 2, 500,
		"PRESS START BUTTON", GetColor(255, 255, 255), true);
}
