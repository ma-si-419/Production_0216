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
	//A�{�^�����A���ŉ�����Ȃ��悤��
	if (!m_input.Buttons[12])
	{
		m_isKeyDown = true;
	}
	//A�{�^���������ꂽ��
	if (m_input.Buttons[12] && m_isKeyDown)
	{
		//���l�ɂ������V�[���ɔ��
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
	//��L�[�������ꂽ��
	if (m_input.Buttons[0] && !m_isSelectKeyDown)
	{
		m_selectNum--;
		if (m_selectNum < 0)
		{
			m_selectNum = kMaxSceneNum;
		}
		m_isSelectKeyDown = true;
	}
	//���L�[�����͂��ꂽ��
	else if (m_input.Buttons[1] && !m_isSelectKeyDown)
	{
		m_selectNum++;
		if (m_selectNum > kMaxSceneNum)
		{
			m_selectNum = 0;
		}
		m_isSelectKeyDown = true;
	}
	//��L�[�Ɖ��L�[�������ꂽ��
	else if(!m_input.Buttons[0] && !m_input.Buttons[1])
	{
		m_isSelectKeyDown = false;
	}
}

void SceneSelect::Draw()
{
	DrawString(100, 100, "����������", GetColor(255, 255, 255));
	DrawFormatString(200, 200, GetColor(255,255,255), "%d", m_selectNum);
	switch (m_selectNum)
	{
	case 0:
		DrawString(400, 400, "�o�g���V�[��", GetColor(255, 255, 255));
		break;
	case 1:
		DrawString(400, 400, "�^�C�g���V�[��", GetColor(255, 255, 255));		
		break;
	case 2:
		DrawString(400, 400, "�V���b�v�V�[��", GetColor(255, 255, 255));
		break;
	}
}
