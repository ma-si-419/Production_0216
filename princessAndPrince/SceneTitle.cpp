#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSelect.h"
SceneTitle::SceneTitle(SceneManager& sceneManager,DataManager& DataManager) :
	Scene(sceneManager,DataManager),
	m_isKeyDown(true),
	m_handle(-1),
	m_bgGraph(0)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_bgm = DataManager.SearchSound("titleBgm");
	m_bgGraph = DataManager.SearchGraph("titleSceneBgGraph");
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::Update(Pad& pad)
{
	if (!CheckSoundMem(m_bgm) && m_isKeyDown)
	{
		PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
	}
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	 //Aƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚ç
	if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown || CheckHitKey(KEY_INPUT_RETURN) && m_isKeyDown)
	{
		StopSoundMem(m_bgm);
		PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));
		m_isKeyDown = false;
	}
}

void SceneTitle::Draw()
{
	DrawGraph(0,0,m_bgGraph,true);
	int DrawWidth = GetDrawStringWidth("•P‚Æ‰¤Žq", -1);

	DrawString((Game::kPlayScreenWidth - DrawWidth) / 2, Game::kPlayScreenHeight / 2,
		"•P‚Æ‰¤Žq", GetColor(255, 255, 255), true);
	DrawWidth = GetDrawStringWidth("PRESS START BUTTON", -1);
	DrawString((Game::kPlayScreenWidth - DrawWidth) / 2, 500,
		"PRESS START BUTTON", GetColor(255, 255, 255), true);
}
