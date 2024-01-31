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
	constexpr float kMaxBarWidth = 200.0f;
	constexpr float kBarHeight = 50.0f;
	//�\������Ԋu
	constexpr int kShowTime = 60;
	//Exp�\������^�C�~���O
	constexpr int kExpShowTime = 120;
	//�t�H���g�̑傫��(���p)
	constexpr int kFontSize = 24;
	//�X�e�[�^�X��\��������W
	constexpr int kStatusPos = 1100;
	//�X�e�[�^�X�̃��x����\�����W
	constexpr int kStatusLevelPos = 1430;
	//�擾�����A�C�e����\��������W
	constexpr int kGetItemPos = 1350;
	//�t�H���g�̌��ɕ\������e�̂��炷�傫��
	constexpr int kShiftShadowLange = 3;
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false),
	m_isLeaveButton(false),
	m_isClearUIEnd(false)
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
}

void UI::Draw()
{
	//UI�̔w�i(�E��)�\��
	DrawGraph(960, 0, m_pMain->GetUiBg(), true);


	DrawGetItem();

	DrawStatus();
	//���ǂ���̖��@��ł��Ă��邩��\������
	if (m_pPrincess->GetMagicKind())
	{
		DrawBox(960, Game::kPlayScreenHeight - 100, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(255, 0, 0), true);
	}
	else
	{
		DrawBox(960, Game::kPlayScreenHeight - 100, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(0, 255, 0), true);
	}
	//�������[�h�̃Q�[�W��\������
	//���ɍ���Box���o��
	DrawBox(Game::kPlayScreenWidth, 700,//�n�_
		Game::kPlayScreenWidth + (int)kMaxBarWidth,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		700 + (int)kBarHeight,//�I�_
		GetColor(0, 0, 0), true);
	DrawBox(Game::kPlayScreenWidth, 700,//�n�_
		Game::kPlayScreenWidth + (int)(kMaxBarWidth / m_pMain->GetSpecialGaugeRate()),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		700 + (int)kBarHeight,//�I�_
		GetColor(255, 255, 255), true);

}

void UI::SceneClearUI()
{
	m_timeCount++;
	//�������������ȃ{�b�N�X��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight,
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
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight,
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
		DrawCircle(400, 900, 20, GetColor(255, 255, 255), true);
		DrawString(500, 900, "�Ŗ߂�", GetColor(255, 255, 255));
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
		DrawString(kStatusPos + kShiftShadowLange, 350 + kShiftShadowLange, "��������Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userAtkLevel + 1), 350 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userAtkLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 440 + kShiftShadowLange, "�ڂ�����Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userDefLevel + 1), 440 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userDefLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 530 + kShiftShadowLange, "�� �� ��Lv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userSpdLevel + 1), 530 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userSpdLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 620 + kShiftShadowLange, "�t�@�C�ALv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userFireLevel + 1), 620 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userFireLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 710 + kShiftShadowLange, "�E�B���hLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userWindLevel + 1), 710 + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", UserData::userWindLevel + 1);
	}
	//�U���͂̃��x����\������
	DrawString(kStatusPos, 350, "��������Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userAtkLevel + 1), 350,//���W
		GetColor(255, 255, 255), "%d", UserData::userAtkLevel + 1);
	//�h��͂̃��x����\������
	DrawString(kStatusPos, 440, "�ڂ�����Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userDefLevel + 1), 440,//���W
		GetColor(255, 255, 255), "%d", UserData::userDefLevel + 1);
	//�ړ��͂̃��x����\������
	DrawString(kStatusPos, 530, "�� �� ��Lv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userSpdLevel + 1), 530,//���W
		GetColor(255, 255, 255), "%d", UserData::userSpdLevel + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, 620, "�t�@�C�ALv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), 620,//���W
		GetColor(255, 255, 255), "%d", UserData::userFireLevel + 1);
	//�����@�̃��x����\������
	DrawString(kStatusPos, 710, "�E�B���hLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), 710,//���W
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
	DrawFormatString(kGetItemPos - AlignmentRight(m_pPlayer->GetExp()), 105,//���W
		GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());

	//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������	
	DrawFormatString(kGetItemPos - AlignmentRight(m_pPlayer->GetExp()), 175,//���W
		GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
}
