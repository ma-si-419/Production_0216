#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSave.h"
namespace
{
	//�t�H���g�̔����̑傫��
	constexpr int kFontHalfSize = 24;
	//�_�������鎞��
	constexpr int kShowTime = 48;
	//�����Ă��鎞��
	constexpr int kEraseTime = 8;
	//�Q�[������߂�ƕ\�����Ă���Ƃ���
	constexpr int kLeaveGameStringPosX = 25;
	constexpr int kLeaveGameStringPosY = 20;
	//PRESS	START BUTTON�̌��ɕ\������{�b�N�X�̍��W
	constexpr int kBackBoxStartPosY = 800;
	constexpr int kBackBoxEndPosY = 850;
	//PRESS	START BUTTON�̌��ɕ\������{�b�N�X�̓����x
	constexpr int kBackBoxAlpha = 170;
	//PRESS	START BUTTON�ƕ\��������W
	constexpr int kStartButtonPosX = 580;
	constexpr int kStartButtonPosY = 800;

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
	m_changeWindowModeGraph = DataManager.SearchGraph("changeWindowModeGraph");
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
	//A�{�^���������ꂽ��
	if (m_isKeyDown)
	{
		if (m_input.Buttons[XINPUT_BUTTON_A]|| CheckHitKey(KEY_INPUT_RETURN) || m_input.Buttons[XINPUT_BUTTON_START])
		{
			StopSoundMem(m_bgm);
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
			m_sceneManager.ChangeScene(std::make_shared<SceneSave>(m_sceneManager, m_dataManager, m_selectSceneNum),true);
			m_isKeyDown = false;
		}
	}
	//�G�X�P�[�v�{�^���������ꂽ��
	if (CheckHitKey(KEY_INPUT_ESCAPE) || m_input.Buttons[XINPUT_BUTTON_B])
	{
		DxLib_End();
		return;
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
	DrawGraph(kLeaveGameStringPosX, kLeaveGameStringPosY, m_changeWindowModeGraph, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBackBoxAlpha);
	DrawBox(0, kBackBoxStartPosY, Game::kScreenWidth, kBackBoxEndPosY, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (m_isString)
	{
		//PRESS START BUTTON�ƕ\������
		DrawString(kStartButtonPosX,kStartButtonPosY,"PRESS START BUTTON", GetColor(255, 255, 255), true);
	}
}
