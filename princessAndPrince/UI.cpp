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
	DrawBox(960, 0, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(100, 100, 100), true);
	//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������	
	DrawFormatString(Game::kScreenWidth - 400, 100,//���W
		GetColor(0, 0, 0), "�擾�o���l%d", m_pPlayer->GetExp());

	//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
	DrawFormatString(Game::kScreenWidth - 400, 200,//���W
		GetColor(0, 0, 0), "�擾�S�[���h%d", m_pPlayer->GetGold());

	//�U���͂̃��x����\������
	DrawFormatString(Game::kScreenWidth - 400, 300,//���W
		GetColor(0, 0, 0), "��������Lv�@%d", UserData::userAtkLevel + 1);
	//�h��͂̃��x����\������	
	DrawFormatString(Game::kScreenWidth - 400, 400,//���W
		GetColor(0, 0, 0), "�ڂ�����Lv�@%d", UserData::userDefLevel + 1);
	//�ړ����x�̃��x����\������	
	DrawFormatString(Game::kScreenWidth - 400, 500,//���W
		GetColor(0, 0, 0), "�� �� ��Lv�@%d", UserData::userSpdLevel + 1);
	//�����@�̃��x����\������	
	DrawFormatString(Game::kScreenWidth - 400, 600,//���W
		GetColor(0, 0, 0), "�t�@�C�ALv�@%d", UserData::userFireLevel + 1);
	//�����@�̃��x����\������
	DrawFormatString(Game::kScreenWidth - 400, 700,//���W
		GetColor(0, 0, 0), "�E�B���hLv�@%d", UserData::userWindLevel + 1);
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
		DrawRectRotaGraph((Game::kPlayScreenWidth - stringWidth) / 2 + 100,200,
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
		DrawCircle(400, 900, 20, GetColor(255, 255, 255), true);
		DrawString(500, 900, "�Ŗ߂�", GetColor(255, 255, 255));
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
		DrawRectRotaGraph((Game::kPlayScreenWidth - stringWidth) / 2 + 100,220,
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
