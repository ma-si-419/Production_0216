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
	constexpr int kMaxSceneNum = 7;
	//�|�[�Y���J�������̍��ڂ̐�
	constexpr int kMaxPauseNum = 2;
	//�I��ł���X�e�[�W��\��������W(�n�_�ƏI�_�܂�)
	constexpr int kSelectSceneStartPosX = 480;
	constexpr int kSelectSceneStartPosY = 180;
	constexpr int kSelectSceneEndPosX = 1090;
	constexpr int kSelectSceneEndPosY = 845;
	//�v���C���[�̃O���t�B�b�N�̑傫��
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	//�v���C���[��\��������W
	constexpr int kPlayerPosX = Game::kScreenWidth / 2;
	constexpr int kPlayerPosY = Game::kPlayScreenHeight * 0.5f;
	//������\��������W
	constexpr int kPrincessPosX = Game::kScreenWidth / 2;
	constexpr int kPrincessPosY = Game::kPlayScreenHeight * 0.5f - 30;
	//�w�i�摜�̑傫��
	constexpr int kBgGraphSize = 640;
	//�L�����N�^�[�̑傫��
	constexpr float kCharcterScale = 6.0f;
	//�A�j���[�V�����̎g���ꏊ
	constexpr int kUseFrame[] = { 1,2,3,2 };
	//�A�j���[�V�����P�R�}�̃t���[����
	constexpr int kAnimFrameNum = 12;
	// �A�j���[�V�����̂P�T�C�N���̃t���[����
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//�V�[���̔w�i�ړ��̃X�s�[�h(�������ق�������)
	constexpr int kBgMoveSpeed = 128;
	//�V���b�v�̃O���t�B�b�N�̑傫��
	constexpr int kShopGraphSize = 64;
	//�V���b�v�̃A�j���[�V�����̒���
	constexpr int kShopAnimFrameNum = 48;
	//�t�H���g�̔����̑傫��
	constexpr int kFontHalfSize = 24;
	//�^�C�g���̍��WX
	constexpr int kTitlePosX = 390;
	//�^�C�g���̍��WY
	constexpr int kTitlePosY = 115;
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
	m_dir(Game::kDirDown),
	m_cutBgPosY(kBgGraphSize* kMaxSceneNum),
	m_isStaging(false),
	m_shopAnimFrame(0),
	m_shopSrcX(0),
	m_isChangeStage(false)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
	m_bgm = DataManager.SearchSound("selectBgm");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
	m_bgGraph = DataManager.SearchGraph("selectBgGraph");
	m_selectSceneBgGraph = DataManager.SearchGraph("allFieldBgGraph");
	m_shopGraph = DataManager.SearchGraph("shopGraph");
	m_selectSceneFrame = DataManager.SearchGraph("selectSceneFrameGraph");
	m_backBoxGraph = DataManager.SearchGraph("backBoxGraph");
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
	if (!CheckSoundMem(m_bgm) && !m_isSelectScene)
	{
		PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
	}
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
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S) && !m_isStaging)
		{
			m_isSelectKeyDown = false;
		}
	}
	//�|�[�Y���J����Ă��Ȃ��A���o���łȂ��ꍇ
	if (!m_isPause && !m_isStaging)
	{
		//�A���ŉ�����Ȃ����߂̏���
		if (m_isKeyDown)
		{
			if (!m_isSelectScene)
			{
				//A�{�^���������ꂽ��
				if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
				{
					StopSoundMem(m_bgm);
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//B�{�^������������^�C�g���ɖ߂�
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					StopSoundMem(m_bgm);
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
				if (m_stageSelectNum >= kMaxSceneNum)
				{
					m_stageSelectNum = kMaxSceneNum;
				}
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = true;
				}
				m_isSelectKeyDown = true;
			}
			//���L�[�����͂��ꂽ��
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				if (m_stageSelectNum <= 0)
				{
					m_stageSelectNum = 0;
				}
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = false;
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
						StopSoundMem(m_bgm);
						//�V���b�v�V�[���Ɉڍs����
						m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager));
						m_isSelectScene = true;
					}
					break;
					case 2://�^�C�g��
					{
						StopSoundMem(m_bgm);
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
	if (m_isStaging)
	{
		MoveScene(m_isSceneUp);
	}
	if (m_cutBgPosY % kBgGraphSize == 0)
	{
		m_isStaging = false;
		m_dir = Game::kDirDown;
		m_animFrame = kAnimFrameNum;
		if (m_isChangeStage)
		{
			if (m_isSceneUp)
			{
				m_stageSelectNum++;
			}
			else
			{
				m_stageSelectNum--;
			}
			m_isChangeStage = false;

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
	//�V���b�v�̃A�j���[�V�������񂵑�����
	m_shopAnimFrame++;
	if (m_shopAnimFrame > kShopAnimFrameNum)
	{
		m_shopSrcX = kShopGraphSize;
	}
	if (m_shopAnimFrame > kShopAnimFrameNum * 2)
	{
		m_shopAnimFrame = 0;
		m_shopSrcX = 0;
	}
}

void SceneSelect::Draw()
{
	//�w�i�̉��ɍ��I��ł���X�e�[�W��\������
	DrawRectExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY,
		0, m_cutBgPosY, kBgGraphSize, kBgGraphSize, m_selectSceneBgGraph, true);
	//�w�i�̕\��
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//�I��ł���V�[���̃t���[��
	DrawExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY - 50,
		m_selectSceneFrame, true);
	//�X�e�[�W���̕\��
	DrawExtendGraph(70, 90, 730, 180, m_backBoxGraph, true);
	//�v���C���[�Ɩ�����\������
	int animEle = m_animFrame / kAnimFrameNum;
	//�摜�̂ǂ���؂��邩�v�Z
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;
	DrawFormatString(500, 500, GetColor(0, 0, 0), "%d", m_animFrame);
	if (m_isStaging && m_isSceneUp)
	{
		DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			kCharcterScale,
			0.0,
			m_playerGraph, true, false);
		DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
			0, 48,
			24, 24,
			5.0,
			0.0,
			m_princessGraph, true, false);
	}
	else
	{
		DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
			0, 24,
			24, 24,
			5.0,
			0.0,
			m_princessGraph, true, false);
		DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			kCharcterScale,
			0.0,
			m_playerGraph, true, false);
	}
	//�I��ł���X�e�[�W��\������
	{
		int stringWidth;
		switch (m_stageSelectNum)
		{
		case 0:
			stringWidth = GetStringLength("�`���̎n�܂�") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�`���̎n�܂�", GetColor(255, 255, 255));
			break;
		case 1:
			stringWidth = GetStringLength("��݂�����P") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "��݂�����P", GetColor(255, 255, 255));
			break;
		case 2:
			stringWidth = GetStringLength("�{��̃p���[") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�{��̃p���[", GetColor(255, 255, 255));
			break;
		case 3:
			stringWidth = GetStringLength("�C�m�V�V���ӕ�") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�C�m�V�V���ӕ�", GetColor(255, 255, 255));
			break;
		case 4:
			stringWidth = GetStringLength("�A�u�i�C�̎���") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�A�u�i�C�̎���", GetColor(255, 255, 255));
			break;
		case 5:
			stringWidth = GetStringLength("�h���S���̑��A") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�h���S���̑��A", GetColor(255, 255, 255));
			break;
		case 6:
			stringWidth = GetStringLength("��ɐ��ގ҂���") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "��ɐ��ގ҂���", GetColor(255, 255, 255));
			break;
		case 7:
			stringWidth = GetStringLength("�������̐킢�͂��ꂩ�炾") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�������̐킢�͂��ꂩ�炾", GetColor(255, 255, 255));
			break;
		default:
			break;
		}
	}
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
	//�V���b�v�̕\��
	DrawRectExtendGraph(1050, 350, 1550, 850, m_shopSrcX, 0, 64, 64, m_shopGraph, true);
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

void SceneSelect::MoveScene(bool up)
{
	float speed;
	speed = kBgGraphSize / kBgMoveSpeed;
	if (up)
	{
		m_cutBgPosY -= speed;
		m_animFrame++;
		if (m_animFrame > kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
		m_dir = Game::kDirUp;
	}
	else
	{
		m_cutBgPosY += speed;
		m_animFrame++;
		if (m_animFrame > kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
		m_dir = Game::kDirDown;
	}
}