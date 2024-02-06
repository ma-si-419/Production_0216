#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSelect.h"
namespace
{
	//フォントの半分の大きさ
	constexpr int kFontHalfSize = 24;
	//点灯させる時間
	constexpr int kShowTime = 48;
	//消している時間
	constexpr int kEraseTime = 8;
}
SceneTitle::SceneTitle(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(true),
	m_handle(-1),
	m_bgGraph(0),
	m_isString(true),
	m_count(0),
	m_selectSceneNum(selectSceneNum)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_bgm = DataManager.SearchSound("titleBgm");
	m_bgGraph = DataManager.SearchGraph("titleSceneBgGraph");
	m_titleGraph = DataManager.SearchGraph("titleGraph");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_enemyGraph = DataManager.SearchGraph("enemyGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	SetFontThickness(9);
}

void SceneTitle::Update(Pad& pad)
{
	if (!CheckSoundMem(m_bgm) && m_isKeyDown)
	{
		PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
	}
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//Aボタンが押されたら
	if (m_isKeyDown)
	{
		if (m_input.Buttons[XINPUT_BUTTON_A]|| CheckHitKey(KEY_INPUT_RETURN) || m_input.Buttons[XINPUT_BUTTON_START])
		{
			StopSoundMem(m_bgm);
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
			m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectSceneNum),true);
			m_isKeyDown = false;
		}
	}
	m_count++;
	if (m_count > kShowTime)
	{
		m_isString = false;
	}
	if (m_count > kShowTime + kEraseTime)
	{
		m_isString = true;
		m_count = 0;
	}
}

void SceneTitle::Draw()
{
	DrawGraph(0, 0, m_bgGraph, true);
	DrawGraph(0, 0, m_titleGraph, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	DrawBox(0, 800, Game::kScreenWidth, 850, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (m_isString)
	{
		int stringWidth = GetStringLength("PRESS START BUTTON") * kFontHalfSize;
		DrawString((Game::kScreenWidth - stringWidth) / 2, 800,
			"PRESS START BUTTON", GetColor(255, 255, 255), true);
	}
}

void SceneTitle::ShowTitleMovie()
{
}
