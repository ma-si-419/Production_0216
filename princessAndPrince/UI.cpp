#include "UI.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Princess.h"
#include "UserData.h"
#include "SceneMain.h"
#include "DataManager.h"
namespace
{
	constexpr float kMaxBarWidth = 189.0f;
	constexpr float kBarHeight = 100.0f;
	//�\������Ԋu
	constexpr int kShowTime = 60;
	//Exp�\������^�C�~���O
	constexpr int kExpShowTime = 120;
	//�t�H���g�̑傫��(���p)
	constexpr int kFontSize = 24;
	//�X�e�[�^�X��\��������W
	constexpr int kStatusPos = 1100;
	//�v���C���[�̃��x������\��������W
	constexpr int kMainStatusPos = 1210;
	//�X�e�[�^�X�̃��x����\�����W
	constexpr int kStatusLevelPos = 1430;
	//���C�����x���̐���\��������W
	constexpr int kMainLevelPos = 1325;
	//�擾�����A�C�e����\��������W
	constexpr int kGetItemPos = 1350;
	//�t�H���g�̌��ɕ\������e�̂��炷�傫��
	constexpr int kShiftShadowLange = 3;
	//�A�C�e���O���t�̑傫��
	constexpr int kItemGraphScale = 32;
	//Ui��h�炷��
	constexpr int kShakeLange = 1;
	//Ui��\��������W
	constexpr int kUiPosYArr[6] = {315,390,465,540,615,690};
	//�{�^�����ǂ��܂ő傫���Ȃ邩
	constexpr double kAngryButtonMaxRatio = 0.8;
	//�{�^�����ǂ��܂ŏ������Ȃ邩
	constexpr double kAngryButtonMinRatio = 0.4;
	//�{�^�����傫���Ȃ�X�s�[�h
	constexpr double kAngryButtonZoomSpeed = 0.01;
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false),
	m_isLeaveButton(false),
	m_isClearUIEnd(false),
	m_angryGaugeUiShiftPosX(0)
{
	m_buttonsGraph = m_pMain->GetButtonsGraph();
}

UI::~UI()
{
}

void UI::Init()
{
}

void UI::Update()
{
	if (m_pMain->GetSpecialGaugeRate() == 1)
	{
		if (m_isAngryGaugeUiShake)
		{
			m_angryGaugeUiShiftPosX++;
		}
		else if (!m_isAngryGaugeUiShake)
		{
			m_angryGaugeUiShiftPosX--;
		}
	}
	if (m_angryGaugeUiShiftPosX > kShakeLange)
	{
		m_isAngryGaugeUiShake = false;
	}
	else if (m_angryGaugeUiShiftPosX < kShakeLange)
	{
		m_isAngryGaugeUiShake = true;
	}
	if (m_isButtonZoom)
	{
		m_angryButtonRatio += kAngryButtonZoomSpeed;
	}
	else
	{
		m_angryButtonRatio -= kAngryButtonZoomSpeed;
	}
	if (m_angryButtonRatio > kAngryButtonMaxRatio)
	{
		m_isButtonZoom = false;
		m_angryButtonRatio = kAngryButtonMaxRatio;
	}
	else if (m_angryButtonRatio < kAngryButtonMinRatio)
	{
		m_isButtonZoom = true;
		m_angryButtonRatio = kAngryButtonMinRatio;
	}
	
}

void UI::Draw()
{
	//UI�̔w�i(�E��)�\��
	DrawGraph(Game::kPlayScreenWidth, 0, m_pMain->GetUiBg(), true);


	DrawGetItem();

	DrawStatus();
	//���ǂ���̖��@��ł��Ă��邩��\������
	if (m_pPrincess->GetMagicKind())
	{
		DrawRectRotaGraph(1320, 855, 106, 0, 106, 106, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(1495, 855, 0, 0, 106, 106, 1.0, 0.0, m_magicUiBgGraph, true, false, false);

	}
	else
	{
		DrawRectRotaGraph(1320, 855, 0, 0, 106, 106, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(1495, 855, 106, 0, 106, 106, 1.0, 0.0, m_magicUiBgGraph, true, false, false);

	}
	//�����@��Ui�\��
	DrawRectRotaGraph(1318, 855, 0, Game::kFire * kItemGraphScale, kItemGraphScale, kItemGraphScale,
		2.7, 4.72, m_magicGraph, true, false, false);
	//�����@��Ui�\��
	DrawRectRotaGraph(1495, 855, 0, Game::kTyphoon * kItemGraphScale, kItemGraphScale, kItemGraphScale,
		2.7, 0.0, m_magicGraph, true, false, false);
	if (m_pPrincess->GetMagicKind())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawCircle(1495, 855, 50, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawCircle(1320, 855, 50, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//�������[�h�̃Q�[�W��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(1060, 805,//�n�_
		1060 + (int)(kMaxBarWidth / m_pMain->GetSpecialGaugeRate()),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		805 + (int)kBarHeight, GetColor(255, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(1050 + m_angryGaugeUiShiftPosX, 800, m_angryGaugeUiGraph, true);
	if (m_pMain->GetSpecialGaugeRate() == 1)
	{
		DrawRotaGraph(1050, 800,m_angryButtonRatio,0.0,m_angryButtonGraph,true,0,0);
	}
}

void UI::SceneClearUI()
{
	m_timeCount++;
	//�������������ȃ{�b�N�X��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Game::kPlayScreenWidth + 5, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		int stringWidth = GetDrawStringWidth("�Q�[���N���A", -1);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 - 150, 200, "�Q �[ ��", GetColor(255, 255, 255));
		DrawRectRotaGraph((Game::kPlayScreenWidth - stringWidth) / 2 + 100, 200,
			48, Game::kDirDeath * 16,
			16, 16,
			6.0,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 + 150, 200, "�N �� �A", GetColor(255, 255, 255));
		//���݂̃��x����\������
		DrawString(100, 400, "���݂̃��x��", GetColor(255, 255, 255));
		DrawFormatString(700 - AlignmentRight(UserData::userMainLevel + 1), 400, GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//�l�������S�[���h�ƌo���l��\������
		DrawString(100, 500, "�l���o���l", GetColor(255, 255, 255));
		DrawString(100, 600, "���̃��x���܂�", GetColor(255, 255, 255));
		DrawString(100, 700, "�l���S�[���h", GetColor(255, 255, 255));
		DrawString(100, 800, "�����S�[���h", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetExp()), 500, GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(700 - AlignmentRight(m_pMain->GetNextExp()), 600, GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetGold()), 700, GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(700 - AlignmentRight(UserData::userGold), 800, GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	if (m_isLeaveButton)
	{
		DrawRectRotaGraph(440, 900, 0, 0, 32, 32, 2.4, 0.0, m_buttonsGraph, true, 0, 0);
		DrawString(480, 880, "�Ŗ߂�", GetColor(255, 255, 255));
	}
}

void UI::GameOverUI()
{
	m_timeCount++;
	//�������������ȃ{�b�N�X��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Game::kPlayScreenWidth + 3, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		int stringWidth = GetDrawStringWidth("�Q�[���I�[�o�[", -1);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 - 150, 200, "�Q �[ ��", GetColor(255, 255, 255));
		DrawRectRotaGraph((Game::kPlayScreenWidth - stringWidth) / 2 + 100, 220,
			16, Game::kDirDeath * 16,
			16, 16,
			6.0,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 + 150, 200, "�I �[ �o �[", GetColor(255, 255, 255));
		//���݂̃��x����\������
		DrawString(100, 400, "���݂̃��x��", GetColor(255, 255, 255));
		DrawFormatString(700 - AlignmentRight(UserData::userMainLevel + 1), 400, GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//�l�������S�[���h�ƌo���l��\������
		DrawString(100, 500, "�l���o���l", GetColor(255, 255, 255));
		DrawString(100, 600, "���̃��x���܂�", GetColor(255, 255, 255));
		DrawString(100, 700, "�l���S�[���h", GetColor(255, 255, 255));
		DrawString(100, 800, "�����S�[���h", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetExp()), 500, GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(700 - AlignmentRight(m_pMain->GetNextExp()), 600, GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetGold()), 700, GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(700 - AlignmentRight(UserData::userGold), 800, GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	if (m_isLeaveButton)
	{
		DrawRectRotaGraph(440, 900, 0, 0, 32, 32, 2.4, 0.0, m_buttonsGraph, true, 0, 0);
		DrawString(480, 880, "�Ŗ߂�", GetColor(255, 255, 255));
	}
}

int UI::AlignmentRight(int num)
{
	if (num >= 10000)
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


void UI::DrawStatus()
{
	//�e��\��
	{
		DrawString(kMainStatusPos + kShiftShadowLange, kUiPosYArr[0] + kShiftShadowLange, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kMainLevelPos + kShiftShadowLange - AlignmentRight(UserData::userMainLevel + 1), kUiPosYArr[0] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userMainLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[1] + kShiftShadowLange, "��������Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userAtkLevel + 1), kUiPosYArr[1] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userAtkLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[2] + kShiftShadowLange, "�ڂ�����Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userDefLevel + 1), kUiPosYArr[2] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userDefLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[3] + kShiftShadowLange, "�� �� ��Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userSpdLevel + 1), kUiPosYArr[3] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userSpdLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[4] + kShiftShadowLange, "�t�@�C�ALv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userFireLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[5] + kShiftShadowLange, "�E�B���hLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userWindLevel + 1);
	}
	//���C�����x����\������
	DrawString(kMainStatusPos, kUiPosYArr[0], "Lv", GetColor(255, 255, 255));
	DrawFormatString(kMainLevelPos - AlignmentRight(UserData::userMainLevel + 1), kUiPosYArr[0],//���W
		GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
	//�U���͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[1], "��������Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userAtkLevel + 1), kUiPosYArr[1],//���W
		GetColor(255, 255, 255), "%d", UserData::userAtkLevel + 1);
	//�h��͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[2], "�ڂ�����Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userDefLevel + 1), kUiPosYArr[2],//���W
		GetColor(255, 255, 255), "%d", UserData::userDefLevel + 1);
	//�ړ��͂̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[3], "�� �� ��Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userSpdLevel + 1), kUiPosYArr[3],//���W
		GetColor(255, 255, 255), "%d", UserData::userSpdLevel + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[4], "�t�@�C�ALv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4],//���W
		GetColor(255, 255, 255), "%d", UserData::userFireLevel + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, kUiPosYArr[5], "�E�B���hLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5],//���W
		GetColor(255, 255, 255), "%d", UserData::userWindLevel + 1);
}

void UI::DrawGetItem()
{
	//�e��\������
	{
		//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
		DrawFormatString(kGetItemPos + kShiftShadowLange - AlignmentRight(m_pPlayer->GetGold()), 105 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());

		//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������	
		DrawFormatString(kGetItemPos + kShiftShadowLange - AlignmentRight(m_pPlayer->GetExp()), 175 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());
	}

	//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
	DrawFormatString(kGetItemPos - AlignmentRight(m_pPlayer->GetGold()), 105,//���W
		GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());

	//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������	
	DrawFormatString(kGetItemPos - AlignmentRight(m_pPlayer->GetExp()), 175,//���W
		GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
}
