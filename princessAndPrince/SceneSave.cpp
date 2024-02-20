#include "SceneSave.h"
#include "DataManager.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
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
	constexpr int kFramePosX[2] = { 115,521 };
	constexpr int kFramePosY[2] = { 500,675 };
	//�t���[���̑傫��
	constexpr int kFrameWidth = 360;
	constexpr int kFrameHeight = 130;
	//�t���[���̊g�k�̕�
	constexpr int kFrameRatioLangeX = 6;
	constexpr int kFrameRatioLangeY = 3;
	//�t���[���̊g�k�̑���
	constexpr float kFrameSpeedX = 0.6f;
	constexpr float kFrameSpeedY = 0.3f;
	//�X�e�[�W�̖��O��\��������W
	constexpr int kStageNamePosX[2] = { 480,620 };
	constexpr int kStageNamePosY[2] = { 485,730 };
	//�f�[�^�̃i���o�[��\��������W
	constexpr int kSaveDataNumPosX[2] = { 205,610 };
	constexpr int kSaveDataNumPosY[2] = { 540,715 };
	//�G�l�~�[�̓���\������Ԋu
	constexpr int kEnemyHeadDistance = 40;
	//�G�l�~�[�̓��摜�̃O���t�B�b�N�̑傫��
	constexpr int kEnemyHeadGraphSize = 30;
	//�E�B���h�E���J�������̕����̍��W
	constexpr int kWindowStringPosX = 480;
	constexpr int kWindowStringPosY[3] = { 180,270,360 };
	//�E�B���h�E���J�������̃J�[�\���̍��W
	constexpr int kWindowCursorPosX[3] = { 265,262,290 };
	//�J�[�\���̗h��镝
	constexpr int kWindowCursorShakeLange = 5;
	//�J�[�\����h�炷����
	constexpr float kShakeSpeed = 0.5f;
	//�x����\��������W
	constexpr int kDeleteStringPosX = 510;
	constexpr int kDeleteStringPosY[2] = { 230,320 };
	//�f���[�g�E�B���h�E���J���Ă���Ƃ��̃J�[�\���̃|�W�V����
	constexpr int kDeleteCursorPosX[2] = { 275,465 };
	constexpr int kDeleteCursorPosY = 320;
}
SceneSave::SceneSave(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_bgGraph(0),
	m_frameGraph(0),
	m_isLastKey(true),
	m_selectSaveNum(0),
	m_isZoom(true),
	m_frameRatioX(0),
	m_frameRatioY(0),
	m_cursorSe(0),
	m_isMoveScene(false),
	m_firstLoadData(),
	m_secondLoadData(),
	m_thirdLoadData(),
	m_fourthLoadData(),
	m_isShowSaveWindow(false),
	m_selectDeleteNum(false),
	m_selectWindowNum(0),
	m_cursorShakeLange(0),
	m_isCursorMoveRight(true),
	m_isShowDeleteWindow(false),
	m_cancelSe(0)
{

	m_frameGraph = DataManager.SearchGraph("saveDataFrameGraph");
	m_bgGraph = DataManager.SearchGraph("sceneSaveGraph");
	m_enemyHeadGraph = DataManager.SearchGraph("enemyHeadGraph");
	m_cursorGraph = DataManager.SearchGraph("saveSceneCursorGraph");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_appSe = DataManager.SearchSound("approveSe");
	m_bgm = DataManager.SearchSound("saveSceneBgm");
	m_cancelSe = DataManager.SearchSound("cancelSe");
}

SceneSave::~SceneSave()
{
}

void SceneSave::Init()
{
	//�Z�[�u�f�[�^�����[�h����
	LoadFile();
	//���y��炷
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
}

void SceneSave::Update(Pad& pad)
{
	XINPUT_STATE input;
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	//�{�^����������^�C�~���O��������
	if (!m_isLastKey && !m_isMoveScene)
	{
		//�{�^���������ꂽ��J�[�\���𓮂���
		MoveCursor(input);
	}
	//���ׂẴ{�^���������ꂽ�Ƃ�
	if (!CheckHitKeyAll() && !GetPushButton(input))
	{
		//�{�^����������悤�ɂ���
		m_isLastKey = false;
	}

	//�t���[���𓮂���
	ScalingFrame();


	if (m_cursorShakeLange > kWindowCursorShakeLange)
	{
		m_isCursorMoveRight = false;
	}
	else if (m_cursorShakeLange < -kWindowCursorShakeLange)
	{
		m_isCursorMoveRight = true;
	}

	if (m_isCursorMoveRight)
	{
		m_cursorShakeLange += kShakeSpeed;
	}
	else
	{
		m_cursorShakeLange -= kShakeSpeed;
	}

}

void SceneSave::Draw()
{
	//�w�i�̕\��
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);

	//�X�e�[�^�X��\������
	DrawStatus();

	//�f�[�^�̔ԍ���\������
	DrawSaveDataNum();

	//�Z�[�u�f�[�^�̃t���[����\������
	DrawFrame(m_selectSaveNum);

	//�Z�[�u�f�[�^���I�΂ꂽ�Ƃ�
	if (m_isShowSaveWindow)
	{
		int stringWidth;
		//�e��\������
		stringWidth = GetStringLength("�X�^�[�g����") * kFontSize;
		DrawString(kWindowStringPosX + kShiftShadowLange - stringWidth, kWindowStringPosY[0] + kShiftShadowLange, "�X�^�[�g����", GetColor(0, 0, 0));
		stringWidth = GetStringLength("�f�[�^������") * kFontSize;
		DrawString(kWindowStringPosX + kShiftShadowLange - stringWidth, kWindowStringPosY[1] + kShiftShadowLange, "�f�[�^������", GetColor(0, 0, 0));
		stringWidth = GetStringLength("�L�����Z��") * kFontSize;
		DrawString(kWindowStringPosX + kShiftShadowLange - stringWidth, kWindowStringPosY[2] + kShiftShadowLange, "�L�����Z��", GetColor(0, 0, 0));
		//������\������
		stringWidth = GetStringLength("�X�^�[�g����") * kFontSize;
		DrawString(kWindowStringPosX - stringWidth, kWindowStringPosY[0], "�X�^�[�g����", GetColor(255, 255, 255));
		stringWidth = GetStringLength("�f�[�^������") * kFontSize;
		DrawString(kWindowStringPosX - stringWidth, kWindowStringPosY[1], "�f�[�^������", GetColor(255, 255, 255));
		stringWidth = GetStringLength("�L�����Z��") * kFontSize;
		DrawString(kWindowStringPosX - stringWidth, kWindowStringPosY[2], "�L�����Z��", GetColor(255, 255, 255));

		DrawGraph(kWindowCursorPosX[m_selectWindowNum] + static_cast<int>(m_cursorShakeLange), kWindowStringPosY[m_selectWindowNum], m_cursorGraph, true);
	}
	if (m_isShowDeleteWindow)
	{
		//�e��\������
		int stringWidth = GetStringLength("�{���ɂɃf�[�^�������܂����H") * kFontSize;
		DrawString(kDeleteStringPosX + kShiftShadowLange - stringWidth, kDeleteStringPosY[0] + kShiftShadowLange, "�{���Ƀf�[�^�������܂����H", GetColor(0, 0, 0));
		stringWidth = GetStringLength("�͂��@�@������") * kFontSize;
		DrawString(kDeleteStringPosX + kShiftShadowLange - stringWidth, kDeleteStringPosY[1] + kShiftShadowLange, "�͂��@�@������", GetColor(0, 0, 0));
		//������\������
		stringWidth = GetStringLength("�{���ɂɃf�[�^�������܂����H") * kFontSize;
		DrawString(kDeleteStringPosX - stringWidth, kDeleteStringPosY[0] + kShiftShadowLange, "�{���Ƀf�[�^�������܂����H", GetColor(255, 255, 255));
		stringWidth = GetStringLength("�͂��@�@������") * kFontSize;
		DrawString(kDeleteStringPosX - stringWidth, kDeleteStringPosY[1] + kShiftShadowLange, "�͂��@�@������", GetColor(255, 255, 255));

		DrawGraph(kDeleteCursorPosX[m_selectDeleteNum] + static_cast<int>(m_cursorShakeLange), kDeleteCursorPosY, m_cursorGraph, true);
	}

}

int SceneSave::GetDrawData(int selectNum, dataKind kind)
{
	//�I�����Ă���f�[�^�ŕԂ��l��ς���
	switch (selectNum)
	{
	case 0:
		//���C�����x��
		if (kind == dataKind::main)
		{
			return m_firstLoadData[static_cast<int>(dataKind::main)];
		}
		//�����������x��
		if (kind == dataKind::atk)
		{
			return m_firstLoadData[static_cast<int>(dataKind::atk)];
		}
		//�h�䃌�x��
		if (kind == dataKind::def)
		{
			return m_firstLoadData[static_cast<int>(dataKind::def)];
		}
		//���ǂ����x��
		if (kind == dataKind::spd)
		{
			return m_firstLoadData[static_cast<int>(dataKind::spd)];
		}
		//�t�@�C�A���x��
		if (kind == dataKind::fire)
		{
			return m_firstLoadData[static_cast<int>(dataKind::fire)];
		}
		//�E�B���h���x��
		if (kind == dataKind::wind)
		{
			return m_firstLoadData[static_cast<int>(dataKind::wind)];
		}
		//�����S�[���h
		if (kind == dataKind::gold)
		{
			return m_firstLoadData[static_cast<int>(dataKind::gold)];
		}
		//���l���o���l
		if (kind == dataKind::exp)
		{
			return m_firstLoadData[static_cast<int>(dataKind::exp)];
		}
		break;
	case 1:
		//���C�����x��
		if (kind == dataKind::main)
		{
			return m_secondLoadData[static_cast<int>(dataKind::main)];
		}
		//�����������x��
		if (kind == dataKind::atk)
		{
			return m_secondLoadData[static_cast<int>(dataKind::atk)];
		}
		//�h�䃌�x��
		if (kind == dataKind::def)
		{
			return m_secondLoadData[static_cast<int>(dataKind::def)];
		}
		//���ǂ����x��
		if (kind == dataKind::spd)
		{
			return m_secondLoadData[static_cast<int>(dataKind::spd)];
		}
		//�t�@�C�A���x��
		if (kind == dataKind::fire)
		{
			return m_secondLoadData[static_cast<int>(dataKind::fire)];
		}
		//�E�B���h���x��
		if (kind == dataKind::wind)
		{
			return m_secondLoadData[static_cast<int>(dataKind::wind)];
		}
		//�����S�[���h
		if (kind == dataKind::gold)
		{
			return m_secondLoadData[static_cast<int>(dataKind::gold)];
		}
		//���l���o���l
		if (kind == dataKind::exp)
		{
			return m_secondLoadData[static_cast<int>(dataKind::exp)];
		}
		break;
	case 2:
		//���C�����x��
		if (kind == dataKind::main)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::main)];
		}
		//�����������x��
		if (kind == dataKind::atk)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::atk)];
		}
		//�h�䃌�x��
		if (kind == dataKind::def)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::def)];
		}
		//���ǂ����x��
		if (kind == dataKind::spd)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::spd)];
		}
		//�t�@�C�A���x��
		if (kind == dataKind::fire)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::fire)];
		}
		//�E�B���h���x��
		if (kind == dataKind::wind)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::wind)];
		}
		//�����S�[���h
		if (kind == dataKind::gold)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::gold)];
		}
		//���l���o���l
		if (kind == dataKind::exp)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::exp)];
		}
	case 3:
		//���C�����x��
		if (kind == dataKind::main)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::main)];
		}
		//�����������x��
		if (kind == dataKind::atk)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::atk)];
		}
		//�h�䃌�x��
		if (kind == dataKind::def)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::def)];
		}
		//���ǂ����x��
		if (kind == dataKind::spd)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::spd)];
		}
		//�t�@�C�A���x��
		if (kind == dataKind::fire)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::fire)];
		}
		//�E�B���h���x��
		if (kind == dataKind::wind)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::wind)];
		}
		//�����S�[���h
		if (kind == dataKind::gold)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::gold)];
		}
		//���l���o���l
		if (kind == dataKind::exp)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::exp)];
		}
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

int SceneSave::AlignmentRight(int num)
{
	if (num >= 1000000)
	{
		return kFontSize * 6;
	}
	else if (num >= 100000)
	{
		return kFontSize * 5;
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
		//�g�k����t���[��
		DrawExtendGraph(kFramePosX[0] - static_cast<int>(m_frameRatioX), kFramePosY[0] - static_cast<int>(m_frameRatioY),//�n�_
			kFramePosX[0] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[0] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[1], kFramePosY[0],//�n�_
			kFramePosX[1] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[0], kFramePosY[1],//�n�_
			kFramePosX[0] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[1], kFramePosY[1],//�n�_
			kFramePosX[1] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		break;
	case 1:
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[0], kFramePosY[0],//�n�_
			kFramePosX[0] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//�g�k����t���[��
		DrawExtendGraph(kFramePosX[1] - static_cast<int>(m_frameRatioX), kFramePosY[0] - static_cast<int>(m_frameRatioY),//�n�_
			kFramePosX[1] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[0] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[0], kFramePosY[1],//�n�_
			kFramePosX[0] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[1], kFramePosY[1],//�n�_
			kFramePosX[1] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		break;
	case 2:
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[0], kFramePosY[0],//�n�_
			kFramePosX[0] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[1], kFramePosY[0],//�n�_
			kFramePosX[1] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//�g�k����t���[��
		DrawExtendGraph(kFramePosX[0] - static_cast<int>(m_frameRatioX), kFramePosY[1] - static_cast<int>(m_frameRatioY),//�n�_
			kFramePosX[0] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[1] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[1], kFramePosY[1],//�n�_
			kFramePosX[1] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		break;
	case 3:
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[0], kFramePosY[0],//�n�_
			kFramePosX[0] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[1], kFramePosY[0],//�n�_
			kFramePosX[1] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//�������Ȃ��t���[��
		DrawExtendGraph(kFramePosX[0], kFramePosY[1],//�n�_
			kFramePosX[0] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		//�g�k����t���[��
		DrawExtendGraph(kFramePosX[1] - static_cast<int>(m_frameRatioX), kFramePosY[1] - static_cast<int>(m_frameRatioY),//�n�_
			kFramePosX[1] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[1] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		break;
	default:
		break;
	}
}

void SceneSave::SetSaveData(int selectNum)
{
	m_isMoveScene = true;
	StopSoundMem(m_bgm);
	switch (selectNum)
	{
	case 0:
		UserData::userMainLevel = m_firstLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_firstLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_firstLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_firstLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_firstLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_firstLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_firstLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_firstLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_firstLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_firstLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_firstLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	case 1:
		UserData::userMainLevel = m_secondLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_secondLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_secondLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_secondLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_secondLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_secondLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_secondLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_secondLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_secondLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_secondLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_secondLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	case 2:
		UserData::userMainLevel = m_thirdLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_thirdLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_thirdLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_thirdLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_thirdLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_thirdLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_thirdLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_thirdLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_thirdLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_thirdLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_thirdLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	case 3:
		UserData::userMainLevel = m_fourthLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_fourthLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_fourthLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_fourthLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_fourthLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_fourthLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_fourthLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_fourthLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_fourthLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_fourthLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_fourthLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	default:
		break;
	}
}

void SceneSave::DrawSaveDataNum()
{
	//�e��\��
	DrawString(kSaveDataNumPosX[0] + kShiftShadowLange, kSaveDataNumPosY[0] + kShiftShadowLange, "�f�[�^1", GetColor(0, 0, 0));
	DrawString(kSaveDataNumPosX[1] + kShiftShadowLange, kSaveDataNumPosY[0] + kShiftShadowLange, "�f�[�^2", GetColor(0, 0, 0));
	DrawString(kSaveDataNumPosX[0] + kShiftShadowLange, kSaveDataNumPosY[1] + kShiftShadowLange, "�f�[�^3", GetColor(0, 0, 0));
	DrawString(kSaveDataNumPosX[1] + kShiftShadowLange, kSaveDataNumPosY[1] + kShiftShadowLange, "�f�[�^4", GetColor(0, 0, 0));
	//�f�[�^�̔ԍ���\��
	DrawString(kSaveDataNumPosX[0], kSaveDataNumPosY[0], "�f�[�^1", GetColor(255, 255, 255));
	DrawString(kSaveDataNumPosX[1], kSaveDataNumPosY[0], "�f�[�^2", GetColor(255, 255, 255));
	DrawString(kSaveDataNumPosX[0], kSaveDataNumPosY[1], "�f�[�^3", GetColor(255, 255, 255));
	DrawString(kSaveDataNumPosX[1], kSaveDataNumPosY[1], "�f�[�^4", GetColor(255, 255, 255));
}

void SceneSave::DrawStatus()
{
	//�e��\��
	{
		DrawString(kMainStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::main)] + kShiftShadowLange, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kMainLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::main) + 1), kUiPosYArr[0] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::main) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::atk)] + kShiftShadowLange, "��������Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::atk) + 1), kUiPosYArr[1] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::atk) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::def)] + kShiftShadowLange, "�ڂ�����Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::def) + 1), kUiPosYArr[2] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::def) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::spd)] + kShiftShadowLange, "�� �� ��Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::spd) + 1), kUiPosYArr[3] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::spd) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::fire)] + kShiftShadowLange, "�t�@�C�ALv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::fire) + 1), kUiPosYArr[4] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::fire) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::wind)] + kShiftShadowLange, "�E�B���hLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::wind) + 1), kUiPosYArr[5] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::wind) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::gold)] + kShiftShadowLange, "�����S�[���h", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::gold)), kUiPosYArr[6] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::gold));
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::exp)] + kShiftShadowLange, "���l���o���l", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::exp)), kUiPosYArr[7] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::exp));
	}
	//���C�����x����\������
	DrawString(kMainStatusPos, kUiPosYArr[static_cast<int>(dataKind::main)], "Lv", GetColor(255, 255, 255));
	DrawFormatString(kMainLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::main) + 1), kUiPosYArr[0],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::main) + 1);
	//�U���͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::atk)], "��������Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::atk) + 1), kUiPosYArr[1],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::atk) + 1);
	//�h��͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::def)], "�ڂ�����Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::def) + 1), kUiPosYArr[2],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::def) + 1);
	//�ړ��͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::spd)], "�� �� ��Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::spd) + 1), kUiPosYArr[3],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::spd) + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::fire)], "�t�@�C�ALv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::fire) + 1), kUiPosYArr[4],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::fire) + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::wind)], "�E�B���hLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::wind) + 1), kUiPosYArr[5],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::wind) + 1);
	//�S�[���h�̗�
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::gold)], "�����S�[���h", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::gold)), kUiPosYArr[6],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::gold));
	//���l���ʂ̕\��
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::exp)], "���l���o���l", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::exp)), kUiPosYArr[7],//���W
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::exp));
}

void SceneSave::LoadFile()
{
	{
		//�t�@�C�����J��
		std::ifstream ifs("./data/save/firstSaveData.txt");
		int loopCount = 0;
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
		std::ifstream ifs("./data/save/secondSaveData.txt");
		int loopCount = 0;
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
		std::ifstream ifs("./data/save/thirdSaveData.txt");
		int loopCount = 0;
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
	{
		//�t�@�C�����J��
		std::ifstream ifs("./data/save/fourthSaveData.txt");
		int loopCount = 0;
		//�z����쐬
		char str[kArraySize];
		//�����������s���ǂݍ���
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_fourthLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
}

bool SceneSave::GetPushButton(XINPUT_STATE& input)
{
	//�\���L�[��������Ă��邩�ǂ���
	if (input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && input.Buttons[XINPUT_BUTTON_DPAD_UP]
		&& input.Buttons[XINPUT_BUTTON_DPAD_RIGHT] && input.Buttons[XINPUT_BUTTON_DPAD_LEFT])
	{
		return true;
	}
	//A�{�^����������Ă��邩
	else if (input.Buttons[XINPUT_BUTTON_A])
	{
		return true;
	}
	return false;
}

void SceneSave::ScalingFrame()
{
	//�K��l�����������Ȃ�����
	if (m_frameRatioX > kFrameRatioLangeX || m_frameRatioY > kFrameRatioLangeY)
	{
		//�g����n�߂�
		m_isZoom = true;
	}
	//�K��l�����傫���Ȃ�����
	else if (m_frameRatioX < -kFrameRatioLangeX || m_frameRatioY < -kFrameRatioLangeY)
	{
		//�k�����n�߂�
		m_isZoom = false;
	}
	//�g�債�Ă��邩�ǂ���
	if (m_isZoom)
	{
		//�t���[���̊g��
		m_frameRatioX -= kFrameSpeedX;
		m_frameRatioY -= kFrameSpeedY;
	}
	else
	{
		//�t���[���̏k��
		m_frameRatioX += kFrameSpeedX;
		m_frameRatioY += kFrameSpeedY;
	}
}

void SceneSave::MoveCursor(XINPUT_STATE& input)
{
	if (CheckHitKey(KEY_INPUT_RETURN) || input.Buttons[XINPUT_BUTTON_A])
	{
		m_isLastKey = true;
		//�f���[�g�E�B���h�E���\������Ă���Ƃ�
		if (m_isShowDeleteWindow)
		{
			switch (m_selectDeleteNum)
			{
			case 0:
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				DeleteSaveData(m_selectSaveNum);
				m_isShowSaveWindow = false;
				m_isShowDeleteWindow = false;
				return;
				break;
			case 1:
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isShowDeleteWindow = false;
				m_isShowSaveWindow = true;
				return;
				break;
			default:
				break;
			}
		}

		//�Z�[�u�E�B���h�E���\������Ă��鎞
		if (m_isShowSaveWindow)
		{
			switch (m_selectWindowNum)
			{
			case 0:
				SetSaveData(m_selectSaveNum);
				m_selectWindowNum = 0;
				break;
			case 1:
				m_selectWindowNum = 0;
				m_isShowSaveWindow = false;
				m_selectDeleteNum = 1;
				m_isShowDeleteWindow = true;
				break;
			case 2:
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isShowSaveWindow = false;
				m_selectWindowNum = 0;
				return;
				break;
			default:
				break;
			}
		}
		//�f���[�g�E�B���h�E���\������Ă��Ȃ�������
		if (!m_isShowDeleteWindow)
		{
			m_isShowSaveWindow = true;
		}

		PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
	}
	//��{�^���������ꂽ��
	else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W) || input.Buttons[XINPUT_BUTTON_DPAD_UP])
	{
		//�f���[�g�E�B���h�E���J����Ă��Ȃ��Ƃ�
		if (!m_isShowDeleteWindow)
		{
			//�Z�[�u�E�B���h�E���J����Ă��Ȃ��Ƃ�
			if (!m_isShowSaveWindow)
			{
				//���̍s�ɂ���ꍇ
				if (m_selectSaveNum > 1)
				{
					//��̍s�ɓ�����
					m_selectSaveNum -= 2;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				//��������ꏊ�ɃJ�[�\������������
				if (m_selectWindowNum > 0)
				{
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_selectWindowNum--;
				}
			}
		}
		m_isLastKey = true;
	}
	//���{�^���������ꂽ��
	else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S) || input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
	{
		//�f���[�g�E�B���h�E���\������Ă��Ȃ��Ƃ�
		if (!m_isShowDeleteWindow)
		{
			//�Z�[�u�E�B���h�E���J����Ă��Ȃ��Ƃ�
			if (!m_isShowSaveWindow)
			{

				//��̍s�ɂ���ꍇ
				if (m_selectSaveNum < 2)
				{
					//���̍s�ɓ�����
					m_selectSaveNum += 2;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				//��������ꏊ�ɃJ�[�\������������
				if (m_selectWindowNum < 2)
				{
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_selectWindowNum++;
				}
			}
		}
		m_isLastKey = true;
	}
	//�E�{�^���������ꂽ��
	else if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D) || input.Buttons[XINPUT_BUTTON_DPAD_RIGHT])
	{
		//�E�B���h�E���J����Ă��Ȃ�������
		if (!m_isShowDeleteWindow && !m_isShowSaveWindow)
		{
			//����ɂ�����
			if (m_selectSaveNum == 0 || m_selectSaveNum == 2)
			{
				m_selectSaveNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
		}
		if (m_isShowDeleteWindow)
		{
			if (m_selectDeleteNum == 0)
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_selectDeleteNum++;
			}
		}
		m_isLastKey = true;
	}
	//���{�^���������ꂽ��
	else if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A) || input.Buttons[XINPUT_BUTTON_DPAD_LEFT])
	{
		//�E�B���h�E���J����Ă��Ȃ�������
		if (!m_isShowDeleteWindow && !m_isShowSaveWindow)
		{
			//�E��ɂ�����
			if (m_selectSaveNum == 1 || m_selectSaveNum == 3)
			{
				m_selectSaveNum--;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
		}
		if (m_isShowDeleteWindow)
		{
			if (m_selectDeleteNum == 1)
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_selectDeleteNum--;
			}
		}
		m_isLastKey = true;
	}
	else if (CheckHitKey(KEY_INPUT_ESCAPE) || input.Buttons[XINPUT_BUTTON_B])
	{
		PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
		if (m_isShowSaveWindow)
		{
			m_isShowSaveWindow = false;
		}
		else
		{
			StopSoundMem(m_bgm);
			m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager, 0), true);
		}
		m_isLastKey = true;
	}
}

void SceneSave::DeleteSaveData(int dataNum)
{
	if (dataNum == 0)
	{
		std::string tempS;
		ofstream outputfile("./data/save/firstSaveData.txt");
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();

		//�t�@�C�����J��
		std::ifstream ifs("./data/save/firstSaveData.txt");
		int loopCount = 0;
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
	else if (dataNum == 1)
	{
		std::string tempS;
		ofstream outputfile("./data/save/secondSaveData.txt");

		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();
		{
			//�t�@�C�����J��
			std::ifstream ifs("./data/save/secondSaveData.txt");
			int loopCount = 0;
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
	}
	else if (dataNum == 2)
	{
		std::string tempS;
		ofstream outputfile("./data/save/thirdSaveData.txt");
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();
		{
			//�t�@�C�����J��
			std::ifstream ifs("./data/save/thirdSaveData.txt");
			int loopCount = 0;
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
	else if (dataNum == 3)
	{
		std::string tempS;
		ofstream outputfile("./data/save/fourthSaveData.txt");
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();
		{
			//�t�@�C�����J��
			std::ifstream ifs("./data/save/fourthSaveData.txt");
			int loopCount = 0;
			//�z����쐬
			char str[kArraySize];
			//�����������s���ǂݍ���
			while (ifs.getline(str, kArraySize))
			{
				int temp;
				temp = std::stoi(str);
				m_fourthLoadData[loopCount] = temp;
				loopCount++;
			}
			ifs.close();
		}
	}
	UserData::userMainLevel = 0;
	UserData::userAtkLevel = 0;
	UserData::userDefLevel = 0;
	UserData::userSpdLevel = 0;
	UserData::userFireLevel = 0;
	UserData::userWindLevel = 0;
	UserData::userGold = 0;
	UserData::userExp = 0;
	UserData::userClearStageNum = 0;
	UserData::userSaveDataNum = dataNum;

}

