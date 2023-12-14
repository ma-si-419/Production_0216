#include "SceneSelect.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneShop.h"
namespace
{
	constexpr int kMaxSceneNum = 2;
}
SceneSelect::SceneSelect(SceneManager& manager) :
	Scene(manager),
	m_isKeyDown(false),
	m_selectNum(0),
	m_isSelectKeyDown(false)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
}

void SceneSelect::End()
{
}

void SceneSelect::Update(Pad& pad)
{
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//Aボタンが連続で押されないように
	if (!m_input.Buttons[12])
	{
		m_isKeyDown = true;
	}
	//Aボタンが押されたら
	if (m_input.Buttons[12] && m_isKeyDown)
	{
		//数値にあったシーンに飛ぶ
		switch (m_selectNum)
		{
		case 0:
			m_manager.ChangeScene(std::make_shared<SceneMain>(m_manager));
			break;
		case 1:
			m_manager.ChangeScene(std::make_shared<SceneTitle>(m_manager));
			break;
		case 2:
			m_manager.ChangeScene(std::make_shared<SceneShop>(m_manager));
		}
	}
	//上キーが押されたら
	if (m_input.Buttons[0] && !m_isSelectKeyDown)
	{
		m_selectNum--;
		if (m_selectNum < 0)
		{
			m_selectNum = kMaxSceneNum;
		}
		m_isSelectKeyDown = true;
	}
	//下キーが入力されたら
	else if (m_input.Buttons[1] && !m_isSelectKeyDown)
	{
		m_selectNum++;
		if (m_selectNum > kMaxSceneNum)
		{
			m_selectNum = 0;
		}
		m_isSelectKeyDown = true;
	}
	//上キーと下キーが離されたら
	else if(!m_input.Buttons[0] && !m_input.Buttons[1])
	{
		m_isSelectKeyDown = false;
	}
}

void SceneSelect::Draw()
{
	DrawString(100, 100, "あいうえお", GetColor(255, 255, 255));
	DrawFormatString(200, 200, GetColor(255,255,255), "%d", m_selectNum);
	switch (m_selectNum)
	{
	case 0:
		DrawString(400, 400, "バトルシーン", GetColor(255, 255, 255));
		break;
	case 1:
		DrawString(400, 400, "タイトルシーン", GetColor(255, 255, 255));		
		break;
	case 2:
		DrawString(400, 400, "ショップシーン", GetColor(255, 255, 255));
		break;
	}
}
