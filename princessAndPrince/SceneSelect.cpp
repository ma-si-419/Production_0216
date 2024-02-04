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
	constexpr int kPlayerPosY = 590;
	//������\��������W
	constexpr int kPrincessPosX = Game::kScreenWidth / 2;
	constexpr int kPrincessPosY = 700;
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
	constexpr int kBgMoveSpeed = 64;
	//�V���b�v�̃O���t�B�b�N�̑傫��
	constexpr int kShopGraphSize = 64;
	//�V���b�v�̃A�j���[�V�����̒���
	constexpr int kShopAnimFrameNum = 48;
	//�{�X�̃A�j���[�V�����̒���
	constexpr int kBossAnimFrameNum = 32;
	//�t�H���g�̔����̑傫��
	constexpr int kFontHalfSize = 24;
	//�^�C�g���̍��WX
	constexpr int kTitlePosX = 390;
	//�^�C�g���̍��WY
	constexpr int kTitlePosY = 115;
	//��̎O�p�`��Y���W
	constexpr int kHighTrianglePosY = 125;
	//���̎O�p�`��Y���W
	constexpr int kLowTrianglePosY = 850;
	//�O�p�`�̑傫��
	constexpr int kTriangleScale = 60;
	//�O�p�`�̃t���[���̑傫��
	constexpr int kTriangleFrameScale = 80;
	//�O�p�`�̃t���[����Y���W
	constexpr int kTriangleFrameShiftPosY = 12;
	//�V���b�v�̊�{�I�ȍ��W
	constexpr int kshopStartPosX = 1100;
	constexpr int kshopStartPosY = 300;
	constexpr int kshopEndPosX = 1620;
	constexpr int kshopEndPosY = 870;
	//�V���b�v���傫���Ȃ��Ă����X�s�[�h
	constexpr int kShopStartPosXSpeed = 28;
	constexpr int kShopStartPosYSpeed = 50;
	constexpr int kShopEndPosXSpeed = 20;
	constexpr int kShopEndPosYSpeed = 5;
	//�V���b�v���ŏI�I�ɑ傫���Ȃ���W
	constexpr int kShopMaxSizePosX = 500;
	//�L�����̓����X�s�[�h
	constexpr int kCharMoveSpeed = 10;
	//�w�i�̏�ӂ̍���
	constexpr int kBgTopEndPos = 125;
	//�{�X�̃O���t�B�b�N�̑傫��
	constexpr int kBossGraphSize = 32;
	//�{�X��\��������W
	constexpr int kBossPosY = 300;
	//�{�X�̃t�F�[�h�̑���
	constexpr int kBossFadeSpeed = 10;
}
SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(selectSceneNum),
	m_isSelectKeyDown(false),
	m_isSelectScene(false),
	m_animFrame(kAnimFrameNum),
	m_dir(Game::kDirDown),
	m_cutBgPosY(kBgGraphSize* kMaxSceneNum - selectSceneNum * kBgGraphSize),
	m_isStaging(false),
	m_shopAnimFrame(0),
	m_shopSrcX(0),
	m_shopSrcY(0),
	m_isChangeStage(false),
	m_isSceneUp(true),
	m_isShopButton(false),
	m_shopStartPosX(kshopStartPosX),
	m_shopStartPosY(kshopStartPosY),
	m_shopEndPosX(kshopEndPosX),
	m_shopEndPosY(kshopEndPosY),
	m_isMoveMainScene(false),
	m_clearStageNum(0),
	m_charPosY(0),
	m_isCharArrTopEnd(false),
	m_isCharArrBottomEnd(false),
	m_isCharMoveDown(false),
	m_bossSrcY(selectSceneNum),
	m_bossSrcX(0),
	m_bossAnimFrame(0),
	m_bossAlpha(0)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_moveMainSceneSe = DataManager.SearchSound("moveMainSceneSe");
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
	m_buttonsUiGraph = DataManager.SearchGraph("selectSceneButtonUiGraph");
	m_storyGraph[0] = DataManager.SearchGraph("storyGraph1");
	m_storyGraph[1] = DataManager.SearchGraph("storyGraph2");
	m_storyGraph[2] = DataManager.SearchGraph("storyGraph3");
	m_storyGraph[3] = DataManager.SearchGraph("storyGraph4");
	m_storyGraph[4] = DataManager.SearchGraph("storyGraph5");
	m_storyGraph[5] = DataManager.SearchGraph("storyGraph6");
	m_storyGraph[6] = DataManager.SearchGraph("storyGraph7");
	m_storyGraph[7] = DataManager.SearchGraph("storyGraph8");
	m_enemyGraph = DataManager.SearchGraph("enemyGraph");

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
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN) && !m_isStaging)
		{
			m_isSelectKeyDown = false;
		}
	}
	//�|�[�Y���J����Ă��Ȃ��A���o���łȂ��ꍇ
	if (!m_isStaging && !m_isShopButton && !m_isMoveMainScene)
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
					//�ʂ̉��ɕς���
					PlaySoundMem(m_moveMainSceneSe, DX_PLAYTYPE_BACK);
					m_animFrame = 24;
					m_dir = Game::kDirDeath;
					m_isMoveMainScene = true;
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//B�{�^������������^�C�g���ɖ߂�
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					StopSoundMem(m_bgm);
					PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager, m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Y�{�^���������ꂽ��
				else if (m_input.Buttons[XINPUT_BUTTON_Y] || CheckHitKey(KEY_INPUT_P))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					//�V���b�v�ɓ��鉉�o������
					m_shopSrcY = 64;
					m_isShopButton = true;
					m_isSelectScene = true;

				}
			}
		}

		if (!m_isSelectKeyDown)
		{

			//��L�[�������ꂽ��
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))
			{
				//�ړ��s��������
				if (m_stageSelectNum >= kMaxSceneNum)
				{
					m_stageSelectNum = kMaxSceneNum;
				}
				//�ړ��\��������
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = true;
					m_bossSrcY++;
				}
				m_isSelectKeyDown = true;
			}
			//���L�[�����͂��ꂽ��
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))
			{
				//�ړ��s��������
				if (m_stageSelectNum <= 0)
				{
					m_stageSelectNum = 0;
				}
				//�ړ��\��������
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = false;
					m_isCharArrBottomEnd = true;
					m_isCharMoveDown = true;
					m_bossSrcY--;
				}
				m_isSelectKeyDown = true;

			}
		}
	}

	//�ړ��̉��o��
	if (m_isStaging)
	{
		//�{�X������
		m_bossAlpha = 0;
		//�A�j���[�V�����𓮂���
		m_animFrame++;
		if (m_animFrame > kAnimFrameCycle)
		{
			m_animFrame = 0;
		}

		//�L��������̒[�܂ł��Ă��Ȃ�������
		if (!m_isCharArrTopEnd)
		{
			//����͂�����Ă�����
			if (m_isSceneUp)
			{
				//�L��������Ɉړ�������
				m_charPosY -= kCharMoveSpeed;
				m_dir = Game::kDirUp;
			}
		}
		//�L��������̒[�ɂ�����
		else
		{
			//�L�����̃|�W�V������������
			m_charPosY += kBgMoveSpeed;
			//�L�����̃|�W�V�����������肷���Ȃ��悤��
			if (m_charPosY > 0)
			{
				m_charPosY = 0;
			}
			m_cutBgPosY -= kBgMoveSpeed;
			//�w�i��؂��鐧��
			if (m_cutBgPosY % kBgGraphSize == 0 && !m_isMoveMainScene)
			{
				m_isStaging = false;
				m_isCharArrTopEnd = false;
				m_dir = Game::kDirDown;
				m_animFrame = kAnimFrameNum;
				if (m_isChangeStage)
				{
					m_stageSelectNum++;
					m_isChangeStage = false;
				}
			}
		}
		//�L���������̒[�܂ł��Ă��Ȃ�������

			//�����͂�����Ă�����
		if (!m_isSceneUp)
		{
			if (m_isCharMoveDown)
			{
				//�L���������Ɉړ�������
				m_charPosY += kCharMoveSpeed;
				m_dir = Game::kDirDown;
				if (m_charPosY > 0)
				{
					m_isCharMoveDown = false;
					m_animFrame = kAnimFrameNum;
				}
			}
		}

		//�L���������̒[�ɂ�����
		if (m_isCharArrBottomEnd)
		{
			//�L�����̃|�W�V������������
			m_charPosY -= kBgMoveSpeed;
			//�L�����̃|�W�V�����������肷���Ȃ��悤��
			if (m_charPosY + kPlayerPosY < kBgTopEndPos)
			{
				m_charPosY = kBgTopEndPos - kPlayerPosY;
			}
			m_cutBgPosY += kBgMoveSpeed;
			//�w�i��؂��鐧��
			if (m_cutBgPosY % kBgGraphSize == 0 && !m_isMoveMainScene)
			{
				m_isCharMoveDown = true;
				m_isCharArrBottomEnd = false;
				m_dir = Game::kDirDown;

				if (m_isChangeStage)
				{
					m_stageSelectNum--;
					m_isChangeStage = false;
				}
			}
		}
	}
	else
	{
		//�ړ����I�������{�X��\������
		m_bossAlpha += kBossFadeSpeed;
		if (m_bossAlpha > 255)
		{
			m_bossAlpha = 255;
		}
	}

	//�L��������̒[�ɓ��B������
	if (m_charPosY + kPlayerPosY < kBgTopEndPos && m_isSceneUp)
	{
		m_isCharArrTopEnd = true;
	}
	//�L���������̒[�ɓ��B������
	if (m_charPosY > 0 && !m_isCharMoveDown)
	{
		m_isStaging = false;
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
	if (m_isShopButton)
	{
		m_shopStartPosX -= kShopStartPosXSpeed;
		m_shopStartPosY -= kShopStartPosYSpeed;
		m_shopEndPosX += kShopEndPosXSpeed;
		m_shopEndPosY += kShopEndPosYSpeed;

		if (m_shopStartPosX < kShopMaxSizePosX)
		{
			StopSoundMem(m_bgm);
			m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager, m_stageSelectNum));
		}
	}
	//�{�X�̃A�j���[�V�������񂵑�����
	m_bossAnimFrame++;
	if (m_bossAnimFrame > kBossAnimFrameNum)
	{
		m_bossSrcX = kBossGraphSize;
	}
	if (m_bossAnimFrame > kBossAnimFrameNum * 2)
	{
		m_bossAnimFrame = 0;
		m_bossSrcX = 0;
	}

	if (m_isMoveMainScene)
	{
		//�������񂾂�t�F�[�h���Ă���
		if (!CheckSoundMem(m_moveMainSceneSe))
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum));
		}
	}
}

void SceneSelect::Draw()
{
	//�w�i�̉��ɍ��I��ł���X�e�[�W��\������
	DrawRectExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY,
		0, m_cutBgPosY, kBgGraphSize, kBgGraphSize, m_selectSceneBgGraph, true);
	//�v���C���[�Ɩ�����\������
	int animEle = m_animFrame / kAnimFrameNum;
	//�摜�̂ǂ���؂��邩�v�Z
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;

	//�T���ƕP�̕`��
	DrawRectRotaGraph(kPlayerPosX, kPlayerPosY + m_charPosY,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		kCharcterScale,
		0.0,
		m_playerGraph, true, false);
	DrawRectRotaGraph(kPrincessPosX, kPrincessPosY + m_charPosY,
		0, 24,
		24, 24,
		5.0,
		0.0,
		m_princessGraph, true, false);
	//�X�e�[�W���ړ�������Ȃ�������
	if (!m_isStaging)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_bossAlpha);
		//�e�X�e�[�W�̃{�X�\��
		DrawRectRotaGraph(Game::kScreenWidth / 2, kBossPosY,
			m_bossSrcX, GetShowBossKind(m_bossSrcY) * kBossGraphSize,
			kBossGraphSize, kBossGraphSize,
			8.0, 0.0, m_enemyGraph, true, 0, 0);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//�w�i�̕\��
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//�I��ł���V�[���̃t���[��
	DrawExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY - 50,
		m_selectSceneFrame, true);
	//�O�p�`�̕\��
	DrawSceneSrideTriangle();
	//�X�e�[�W���̕\��
	DrawExtendGraph(70, 90, 730, 180, m_backBoxGraph, true);
	//����ɃX�g�[���[�\��
	DrawGraph(130, 200, m_storyGraph[m_stageSelectNum], true);
	//�����ɑ�����@�\��
	DrawGraph(130, 650, m_buttonsUiGraph, true);
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
			stringWidth = GetStringLength("���҂�K�˂�") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "���҂�K�˂�", GetColor(255, 255, 255));
			break;
		case 2:
			stringWidth = GetStringLength("��݂�����P") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "��݂�����P", GetColor(255, 255, 255));
			break;
		case 3:
			stringWidth = GetStringLength("�{��̃p���[") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "�{��̃p���[", GetColor(255, 255, 255));
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
	//�V���b�v�̕\��
	DrawRectExtendGraph(static_cast<int>(m_shopStartPosX), static_cast<int>(m_shopStartPosY),
		m_shopEndPosX, m_shopEndPosY, m_shopSrcX, m_shopSrcY, 64, 64, m_shopGraph, true);
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

void SceneSelect::DrawSceneSrideTriangle()
{
	if (m_stageSelectNum != kMaxSceneNum)
	{
		//��̎O�p�`�\��
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleFrameScale, kHighTrianglePosY + kTriangleFrameScale,
			Game::kScreenWidth / 2, kHighTrianglePosY,
			Game::kScreenWidth / 2 - kTriangleFrameScale, kHighTrianglePosY + kTriangleFrameScale,
			GetColor(0, 0, 0), true);
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleScale, kHighTrianglePosY + kTriangleScale + kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2, kHighTrianglePosY + kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2 - kTriangleScale, kHighTrianglePosY + kTriangleScale + kTriangleFrameShiftPosY,
			GetColor(255, 255, 255), true);
	}

	if (m_stageSelectNum != 0)
	{
		//���̎O�p�`�\��
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleFrameScale, kLowTrianglePosY - kTriangleFrameScale,
			Game::kScreenWidth / 2, kLowTrianglePosY,
			Game::kScreenWidth / 2 - kTriangleFrameScale, kLowTrianglePosY - kTriangleFrameScale,
			GetColor(0, 0, 0), true);
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleScale, kLowTrianglePosY - kTriangleScale - kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2, kLowTrianglePosY - kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2 - kTriangleScale, kLowTrianglePosY - kTriangleScale - kTriangleFrameShiftPosY,
			GetColor(255, 255, 255), true);
	}

}

int SceneSelect::GetShowBossKind(int num)
{
	if (num == 0)
	{
		return 0;
	}
	else if (num == 1)
	{
		return 5;
	}
	else if (num == 2)
	{
		return 1;
	}
	else if (num == 3)
	{
		return 3;
	}
	else if (num == 4)
	{
		return 6;
	}
	else if (num == 5)
	{
		return 2;
	}
	else if (num == 6)
	{
		return 4;
	}
	else if (num == 7)
	{
		return 7;
	}
	else
	{
		return 0;
	}
}

