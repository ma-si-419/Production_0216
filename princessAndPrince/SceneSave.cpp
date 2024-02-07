#include "SceneSave.h"
#include "DataManager.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "DxLib.h"
#include "Game.h"
#include "UserData.h"
#include "MyString.h"
namespace
{
	//���x����\��������W
	constexpr int kMainStatusPos = 1120;
	constexpr int kMainLevelPos = 1260;
	//�X�e�[�^�X�̃��x����\��������W
	constexpr int kStatusPos = 950;
	constexpr int kStatusLevelPos = 1430;
	//�e��\������Ƃ��ɂ��炷���W
	constexpr int kShiftShadowLange = 3;
	//UI��\��������W
	constexpr int kUiPosYArr[8] = { 170,270,350,430,510,590,670,750 };
	//�t�H���g�̑傫��(���p)
	constexpr int kFontSize = 24;
	//�z��̑傫��
	constexpr int kArraySize = 81;
	//�Z�[�u�f�[�^�̃t���[����\��������W
	constexpr int kFramePosX = 115;
	constexpr int kFramePosY[3] = { 125,370,615 };
	//�t���[���̏c�̑傫��
	constexpr int kFrameWidth = 735;
	constexpr int kFrameHeight = 225;
	//�t���[���̊g�k�̕�
	constexpr int kFrameRatioLangeX = 6;
	constexpr int kFrameRatioLangeY = 3;
	//�t���[���̊g�k�̑���
	constexpr float kFrameSpeedX = 0.6f;
	constexpr float kFrameSpeedY = 0.3f;

}
SceneSave::SceneSave(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_bgGraph(0),
	m_frameGraph(0),
	m_isLastKey(true),
	m_selectSaveNum(0),
	m_isZoom(true),
	m_frameRatioX(0),
	m_frameRatioY(0)
{
	m_frameGraph = DataManager.SearchGraph("saveDataFrameGraph");
	m_bgGraph = DataManager.SearchGraph("sceneSaveGraph");
}

SceneSave::~SceneSave()
{
}

void SceneSave::Init()
{
	{
		//�t�@�C�����J��
		std::ifstream ifs("./data/firstSaveData.txt");
		int loopCount = 0;
		//�A���Ă����l��Ԃ��z��
		vector<string> tempS;
		//�z����쐬
		char str[kArraySize];
		//�����������s���ǂݍ���
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_firstLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	{
		//�t�@�C�����J��
		std::ifstream ifs("./data/secondSaveData.txt");
		int loopCount = 0;
		//�A���Ă����l��Ԃ��z��
		vector<string> tempS;
		//�z����쐬
		char str[kArraySize];
		//�����������s���ǂݍ���
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_secondLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	{
		//�t�@�C�����J��
		std::ifstream ifs("./data/thirdSaveData.txt");
		int loopCount = 0;
		//�A���Ă����l��Ԃ��z��
		vector<string> tempS;
		//�z����쐬
		char str[kArraySize];
		//�����������s���ǂݍ���
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_thirdLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
}

void SceneSave::Update(Pad& pad)
{
	XINPUT_STATE input;
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	if (!m_isLastKey)
	{
		if (CheckHitKey(KEY_INPUT_RETURN) || input.Buttons[XINPUT_BUTTON_A])
		{
			SetSaveData(m_selectSaveNum);
			m_isLastKey = true;
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W) || input.Buttons[XINPUT_BUTTON_DPAD_UP])
		{
			if (m_selectSaveNum > 0)
			{
				m_selectSaveNum--;
			}
			m_isLastKey = true;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S) || input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
		{
			if (m_selectSaveNum < 2)
			{
				m_selectSaveNum++;
			}
			m_isLastKey = true;
		}
	}
	if (!CheckHitKeyAll() && !input.Buttons[XINPUT_BUTTON_A] && 
		!input.Buttons[XINPUT_BUTTON_DPAD_UP] && !input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
	{
		m_isLastKey = false;
	}
	if (m_frameRatioX > kFrameRatioLangeX || m_frameRatioY > kFrameRatioLangeY)
	{

		m_isZoom = true;
	}
	else if (m_frameRatioX < -kFrameRatioLangeX || m_frameRatioY < -kFrameRatioLangeY)
	{

		m_isZoom = false;
	}
	if (m_isZoom)
	{
		m_frameRatioX -= kFrameSpeedX;
		m_frameRatioY -= kFrameSpeedY;
	}
	else
	{
		m_frameRatioX += kFrameSpeedX;
		m_frameRatioY += kFrameSpeedY;
	}


}

void SceneSave::Draw()
{
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//�e��\��
	{
		DrawString(kMainStatusPos + kShiftShadowLange, kUiPosYArr[0] + kShiftShadowLange, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kMainLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, main) + 1), kUiPosYArr[0] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, main) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[1] + kShiftShadowLange, "��������Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, atk) + 1), kUiPosYArr[1] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, atk) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[2] + kShiftShadowLange, "�ڂ�����Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, def) + 1), kUiPosYArr[2] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, def) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[3] + kShiftShadowLange, "�� �� ��Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, spd) + 1), kUiPosYArr[3] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, spd) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[4] + kShiftShadowLange, "�t�@�C�ALv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, fire) + 1), kUiPosYArr[4] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, fire) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[5] + kShiftShadowLange, "�E�B���hLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, wind) + 1), kUiPosYArr[5] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, wind) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[6] + kShiftShadowLange, "�����S�[���h", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, gold)), kUiPosYArr[6] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, gold));
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[7] + kShiftShadowLange, "���l���o���l", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, exp)), kUiPosYArr[7] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, exp));
	}
	//���C�����x����\������
	DrawString(kMainStatusPos, kUiPosYArr[0], "Lv", GetColor(255, 255, 255));
	DrawFormatString(kMainLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, main) + 1), kUiPosYArr[0],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, main) + 1);
	//�U���͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[1], "��������Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, atk) + 1), kUiPosYArr[1],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, atk) + 1);
	//�h��͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[2], "�ڂ�����Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, def) + 1), kUiPosYArr[2],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, def) + 1);
	//�ړ��͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[3], "�� �� ��Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, spd) + 1), kUiPosYArr[3],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, spd) + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[4], "�t�@�C�ALv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, fire) + 1), kUiPosYArr[4],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, fire) + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[5], "�E�B���hLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, wind) + 1), kUiPosYArr[5],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, wind) + 1);
	//�S�[���h�̗�
	DrawString(kStatusPos, kUiPosYArr[6], "�����S�[���h", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, gold)), kUiPosYArr[6],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, gold));
	//���l���ʂ̕\��
	DrawString(kStatusPos, kUiPosYArr[7], "���l���o���l", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, exp)), kUiPosYArr[7],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, exp));

	DrawFrame(m_selectSaveNum);
#ifdef _DEBUG
	DrawFormatString(100, 100, GetColor(0, 0, 0), "%d", m_selectSaveNum);
#endif // DEBUG

}

void SceneSave::DrawSceneName(int sceneNum)
{
}
int SceneSave::GetDrawData(int selectNum, dataKind kind)
{
	//�I�����Ă���f�[�^�ŕԂ��l��ς���
	switch (selectNum)
	{
	case 0:
		//���C�����x��
		if (static_cast<int>(kind) == 0)
		{
			return m_firstLoadData[0];
		}
		//�����������x��
		if (static_cast<int>(kind) == 1)
		{
			return m_firstLoadData[1];
		}
		//�h�䃌�x��
		if (static_cast<int>(kind) == 2)
		{
			return m_firstLoadData[2];
		}
		//���ǂ����x��
		if (static_cast<int>(kind) == 3)
		{
			return m_firstLoadData[3];
		}
		//�t�@�C�A���x��
		if (static_cast<int>(kind) == 4)
		{
			return m_firstLoadData[4];
		}
		//�E�B���h���x��
		if (static_cast<int>(kind) == 5)
		{
			return m_firstLoadData[5];
		}
		//�����S�[���h
		if (static_cast<int>(kind) == 6)
		{
			return m_firstLoadData[6];
		}
		//���l���o���l
		if (static_cast<int>(kind) == 7)
		{
			return m_firstLoadData[7];
		}
		break;
	case 1:
		//���C�����x��
		if (static_cast<int>(kind) == 0)
		{
			return m_secondLoadData[0];
		}
		//�����������x��
		if (static_cast<int>(kind) == 1)
		{
			return m_secondLoadData[1];
		}
		//�h�䃌�x��
		if (static_cast<int>(kind) == 2)
		{
			return m_secondLoadData[2];
		}
		//���ǂ����x��
		if (static_cast<int>(kind) == 3)
		{
			return m_secondLoadData[3];
		}
		//�t�@�C�A���x��
		if (static_cast<int>(kind) == 4)
		{
			return m_secondLoadData[4];
		}
		//�E�B���h���x��
		if (static_cast<int>(kind) == 5)
		{
			return m_secondLoadData[5];
		}
		//�����S�[���h
		if (static_cast<int>(kind) == 6)
		{
			return m_secondLoadData[6];
		}
		//���l���o���l
		if (static_cast<int>(kind) == 7)
		{
			return m_secondLoadData[7];
		}
		break;
	case 2:
		//���C�����x��
		if (static_cast<int>(kind) == 0)
		{
			return m_thirdLoadData[0];
		}
		//�����������x��
		if (static_cast<int>(kind) == 1)
		{
			return m_thirdLoadData[1];
		}
		//�h�䃌�x��
		if (static_cast<int>(kind) == 2)
		{
			return m_thirdLoadData[2];
		}
		//���ǂ����x��
		if (static_cast<int>(kind) == 3)
		{
			return m_thirdLoadData[3];
		}
		//�t�@�C�A���x��
		if (static_cast<int>(kind) == 4)
		{
			return m_thirdLoadData[4];
		}
		//�E�B���h���x��
		if (static_cast<int>(kind) == 5)
		{
			return m_thirdLoadData[5];
		}
		//�����S�[���h
		if (static_cast<int>(kind) == 6)
		{
			return m_thirdLoadData[6];
		}
		//���l���o���l
		if (static_cast<int>(kind) == 7)
		{
			return m_thirdLoadData[7];
		}
		break;
	}
}
int SceneSave::AlignmentRight(int num)
{
	if (num >= 1000000)
	{
		return kFontSize * 7;
	}
	else if (num >= 100000)
	{
		return kFontSize * 6;
	}
	else if (num >= 10000)
	{
		return kFontSize * 4;
	}
	else if (num >= 1000)
	{
		return kFontSize * 3;
	}
	else if (num >= 100)
	{
		return kFontSize * 2;
	}
	else if (num >= 10)
	{
		return kFontSize * 1;
	}
	else
	{
		return kFontSize * 0;
	}
	return 0;
}

void SceneSave::DrawFrame(int selectNum)
{
	switch (selectNum)
	{
	case 0:
		DrawExtendGraph(kFramePosX - m_frameRatioX, kFramePosY[0] - m_frameRatioY,//�n�_
			kFramePosX + kFrameWidth + m_frameRatioX, kFramePosY[0] + kFrameHeight + m_frameRatioY, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[1],//�n�_
			kFramePosX + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[2],//�n�_
			kFramePosX + kFrameWidth, kFramePosY[2] + kFrameHeight, m_frameGraph, true);		break;
	case 1:
		DrawExtendGraph(kFramePosX, kFramePosY[0],//�n�_
			kFramePosX + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX - m_frameRatioX, kFramePosY[1] - m_frameRatioY,//�n�_
			kFramePosX + kFrameWidth + m_frameRatioX, kFramePosY[1] + kFrameHeight + m_frameRatioY, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[2],//�n�_
			kFramePosX + kFrameWidth, kFramePosY[2] + kFrameHeight, m_frameGraph, true);		break;
	case 2:
		DrawExtendGraph(kFramePosX, kFramePosY[0],//�n�_
			kFramePosX + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[1],//�n�_
			kFramePosX + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX - m_frameRatioX, kFramePosY[2] - m_frameRatioY,//�n�_
			kFramePosX + kFrameWidth + m_frameRatioX, kFramePosY[2] + kFrameHeight + m_frameRatioY, m_frameGraph, true);
		break;
	default:
		break;
	}
}

void SceneSave::SetSaveData(int selectNum)
{
	switch (selectNum)
	{
	case 0:
		UserData::userMainLevel = m_firstLoadData[static_cast<int>(main)];
		UserData::userAtkLevel = m_firstLoadData[static_cast<int>(atk)];
		UserData::userDefLevel = m_firstLoadData[static_cast<int>(def)];
		UserData::userSpdLevel = m_firstLoadData[static_cast<int>(spd)];
		UserData::userFireLevel = m_firstLoadData[static_cast<int>(fire)];
		UserData::userWindLevel = m_firstLoadData[static_cast<int>(wind)];
		UserData::userGold = m_firstLoadData[static_cast<int>(gold)];
		UserData::userExp = m_firstLoadData[static_cast<int>(exp)];
		UserData::userClearStageNum = m_firstLoadData[stage];
		UserData::userSaveDataNum = m_firstLoadData[dataNum];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_firstLoadData[static_cast<int>(stage)]), true);
		break;
	case 1:
		UserData::userMainLevel = m_secondLoadData[static_cast<int>(main)];
		UserData::userAtkLevel = m_secondLoadData[static_cast<int>(atk)];
		UserData::userDefLevel = m_secondLoadData[static_cast<int>(def)];
		UserData::userSpdLevel = m_secondLoadData[static_cast<int>(spd)];
		UserData::userFireLevel = m_secondLoadData[static_cast<int>(fire)];
		UserData::userWindLevel = m_secondLoadData[static_cast<int>(wind)];
		UserData::userGold = m_secondLoadData[static_cast<int>(gold)];
		UserData::userExp = m_secondLoadData[static_cast<int>(exp)];
		UserData::userClearStageNum = m_secondLoadData[stage];
		UserData::userSaveDataNum = m_secondLoadData[dataNum];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_secondLoadData[static_cast<int>(stage)]), true);
		break;
	case 2:
		UserData::userMainLevel = m_thirdLoadData[static_cast<int>(main)];
		UserData::userAtkLevel = m_thirdLoadData[static_cast<int>(atk)];
		UserData::userDefLevel = m_thirdLoadData[static_cast<int>(def)];
		UserData::userSpdLevel = m_thirdLoadData[static_cast<int>(spd)];
		UserData::userFireLevel = m_thirdLoadData[static_cast<int>(fire)];
		UserData::userWindLevel = m_thirdLoadData[static_cast<int>(wind)];
		UserData::userGold = m_thirdLoadData[static_cast<int>(gold)];
		UserData::userExp = m_thirdLoadData[static_cast<int>(exp)];
		UserData::userClearStageNum = m_thirdLoadData[stage];
		UserData::userSaveDataNum = m_thirdLoadData[dataNum];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_thirdLoadData[static_cast<int>(stage)]), true);
		break;
	default:
		break;
	}
}
