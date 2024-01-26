#include "SceneSelect.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneShop.h"
#include "UserData.h"
#include "Player.h"
namespace
{
	//�X�e�[�W�̐�
	constexpr int kMaxSceneNum = 2;
	//�|�[�Y���J�������̍��ڂ̐�
	constexpr int kMaxPauseNum = 2;
	//�I��ł���X�e�[�W��\��������W(�n�_�ƏI�_�܂�)
	constexpr int kSelectSceneStartPosX = 400;
	constexpr int kSelectSceneStartPosY = 180;
	constexpr int kSelectSceneEndPosX = 1300;
	constexpr int kSelectSceneEndPosY = 900;
	//�v���C���[��\��������W
	constexpr int kPlayerPosX = 850;
	constexpr int kPlayerPosY = Game::kPlayScreenHeight * 0.5f;
	//������\��������W
	constexpr int kPrincessPosX = 750;
	constexpr int kPrincessPosY = Game::kPlayScreenHeight * 0.5f;
	//�v���C���[�̃O���t�B�b�N�̑傫��
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	//�L�����N�^�[�̑傫��
	constexpr float kCharcterScale = 6.0f;
	//�A�j���[�V�����̎g���ꏊ
	constexpr int kUseFrame[] = { 1,2,3,2 };
	//�A�j���[�V�����P�R�}�̃t���[����
	constexpr int kAnimFrameNum = 12;
	// �A�j���[�V�����̂P�T�C�N���̃t���[����
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
}
SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& DataManager) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(0),
	m_pauseSelectNum(0),
	m_isSelectKeyDown(false),
	m_isPause(false),
	m_isStatus(false),
	m_isSelectScene(false),
	m_animFrame(kAnimFrameNum),
	m_dir(Game::kDirDown)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
	m_bgGraph = DataManager.SearchGraph("selectBgGraph");
	m_selectSceneBgGraph = DataManager.SearchGraph("bgGraph");
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
	//�{�^�����A���ŉ�����Ȃ����߂̏���
	{
		//A�{�^����B�{�^�����A���ŉ�����Ȃ��悤��
		if (!m_input.Buttons[XINPUT_BUTTON_A] && !m_input.Buttons[XINPUT_BUTTON_B] && //�p�b�h
			!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_ESCAPE))//�L�[�{�[�h
		{
			m_isKeyDown = true;
		}
		//��L�[�Ɖ��L�[�������ꂽ��
		if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&//�p�b�h
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
		{
			m_isSelectKeyDown = false;
		}
	}
	//�|�[�Y���J����Ă��Ȃ��Ƃ�
	if (!m_isPause)
	{
		//�A���ŉ�����Ȃ����߂̏���
		if (m_isKeyDown)
		{
			if (!m_isSelectScene)
			{
				//A�{�^���������ꂽ��
				if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//B�{�^������������^�C�g���ɖ߂�
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Y�{�^���������ꂽ��
				else if (m_input.Buttons[XINPUT_BUTTON_Y] && !m_isPause || CheckHitKey(KEY_INPUT_P))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					m_isPause = true;
				}
			}
		}

		if (!m_isSelectKeyDown)
		{

			//��L�[�������ꂽ��
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_stageSelectNum--;
				if (m_stageSelectNum < 0)
				{
					m_stageSelectNum = kMaxSceneNum;
				}
				m_isSelectKeyDown = true;
			}
			//���L�[�����͂��ꂽ��
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				m_stageSelectNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				if (m_stageSelectNum > kMaxSceneNum)
				{
					m_stageSelectNum = 0;
				}
				m_isSelectKeyDown = true;
			}
		}
	}
	//�|�[�Y���J����Ă���Ƃ�
	if (m_isPause && !m_isStatus)
	{
		if (!m_isSelectKeyDown)
		{
			//��L�[�������ꂽ��
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
			{
				m_pauseSelectNum--;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				if (m_pauseSelectNum < 0)
				{
					m_pauseSelectNum = kMaxPauseNum;
				}
				m_isSelectKeyDown = true;
			}
			//���L�[�����͂��ꂽ��
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				m_pauseSelectNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				if (m_pauseSelectNum > kMaxPauseNum)
				{
					m_pauseSelectNum = 0;
				}
				m_isSelectKeyDown = true;
			}
		}
		if (m_isKeyDown)
		{

			//B�{�^������������|�[�Y��������
			if (m_input.Buttons[XINPUT_BUTTON_B] && !m_isStatus ||
				CheckHitKey(KEY_INPUT_ESCAPE) && !m_isStatus)
			{
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isPause = false;
				m_isKeyDown = false;
			}
			//A�{�^���������ꂽ��m_pauseSelectNum�ɉ����ď������s��
			else if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
			{
				if (!m_isSelectScene)
				{

					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					switch (m_pauseSelectNum)
					{
					case 0://�悳
						//�X�e�[�^�X���Ђ炭
						m_isStatus = true;
						m_isKeyDown = false;
						break;
					case 1://���݂�
					{
						//�V���b�v�V�[���Ɉڍs����
						m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager));
						m_isSelectScene = true;
					}
					break;
					case 2://�^�C�g��
					{
						//�^�C�g���V�[���Ɉڍs����
						m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
						m_isSelectScene = true;
					}
					break;
					}
					m_isKeyDown = false;
				}
			}
		}
	}
	//�悳��ʂ��J����Ă���Ƃ�
	if (m_isStatus)
	{
		if (m_isKeyDown)
		{

			//B�{�^���������ꂽ��|�[�Y��ʂɖ߂�
			if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
			{
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isStatus = false;
				m_isKeyDown = false;
			}
		}
	}
	//0���c���N���Ȃ��悤��
	if (m_animFrame == 0)m_animFrame = 1;
}

void SceneSelect::Draw()
{
	//�w�i�̉��ɍ��I��ł���X�e�[�W��\������
	DrawExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY, m_selectSceneBgGraph, true);
	//�w�i�̕\��
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//�v���C���[�Ɩ�����\������
	int animEle = m_animFrame / kAnimFrameNum;
	//�摜�̂ǂ���؂��邩�v�Z
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;
	DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		kCharcterScale,
		0.0,
		m_playerGraph, true, false);
	DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
		0, 20,
		14, 20,
		6.0,
		0.0,
		m_princessGraph, true, false);
	//�I��ł���X�e�[�W��\������
	DrawFormatString(300, 300, GetColor(255, 255, 255), "%d,%d", UserData::userGold, UserData::userExp);
	DrawFormatString(200, 200, GetColor(255, 255, 255), "%d", m_stageSelectNum);
	DrawFormatString(400, 400, GetColor(255, 255, 255), "�X�e�[�W%d", m_stageSelectNum + 1);
	//�|�[�Y���̏���
	if (m_isPause)
	{
		DrawBox(400, 400, 800, 800, GetColor(255, 200, 0), true);
		DrawString(600, 500, "�悳", GetColor(0, 0, 0));
		DrawString(600, 600, "���݂�", GetColor(0, 0, 0));
		DrawString(600, 700, "�^�C�g��", GetColor(0, 0, 0));

		switch (m_pauseSelectNum)
		{
		case 0:
			DrawString(600, 500, "�悳", GetColor(255, 0, 0));
			break;
		case 1:
			DrawString(600, 600, "���݂�", GetColor(255, 0, 0));
			break;
		case 2:
			DrawString(600, 700, "�^�C�g��", GetColor(255, 0, 0));
			break;

		}
	}
	//�|�[�Y��ʂ��狭�����������Ƃ��̏���
	if (m_isStatus)
	{
		DrawBox(400, 400, 800, 800, GetColor(255, 255, 255), true);
		DrawFormatString(500, 450, GetColor(0, 0, 0), "�S�[���h   %d", UserData::userGold);
		DrawFormatString(500, 500, GetColor(0, 0, 0), "��������   %d", UserData::userExp);
		DrawFormatString(500, 550, GetColor(0, 0, 0), "��������Lv %d", UserData::userAtkLevel + 1);
		DrawFormatString(500, 600, GetColor(0, 0, 0), "�ڂ�����Lv %d", UserData::userDefLevel + 1);
		DrawFormatString(500, 650, GetColor(0, 0, 0), "�� �� ��Lv %d", UserData::userSpdLevel + 1);
		DrawFormatString(500, 700, GetColor(0, 0, 0), "�t�@�C�ALv %d", UserData::userFireLevel + 1);
		DrawFormatString(500, 750, GetColor(0, 0, 0), "�E�B���hLv %d", UserData::userWindLevel + 1);

	}

}
