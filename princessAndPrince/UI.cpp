#include "UI.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Princess.h"
#include "UserData.h"
#include "SceneMain.h"
namespace
{
	constexpr float kMaxBarWidth = 200.0f;
	constexpr float kBarHeight = 50.0f;
	//�\������Ԋu
	constexpr int kShowTime = 60;
	//Exp�\������^�C�~���O
	constexpr int kExpShowTime = 120;
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false)
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
	DrawFormatString(Game::kScreenWidth - 300, 100,//���W
		GetColor(0, 0, 0), "�擾�o���l%d", m_pPlayer->GetExp());

	//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
	DrawFormatString(Game::kScreenWidth - 300, 200,//���W
		GetColor(0, 0, 0), "�擾�S�[���h%d", m_pPlayer->GetGold());

	//�U���͂̃��x����\������
	DrawFormatString(Game::kScreenWidth - 200, 300,//���W
		GetColor(0, 0, 0), "��������Lv�@%d", UserData::userAtkLevel + 1);
	//�h��͂̃��x����\������	
	DrawFormatString(Game::kScreenWidth - 200, 400,//���W
		GetColor(0, 0, 0), "�ڂ�����Lv�@%d", UserData::userDefLevel + 1);
	//�ړ����x�̃��x����\������	
	DrawFormatString(Game::kScreenWidth - 200, 500,//���W
		GetColor(0, 0, 0), "�� �� ��Lv�@%d", UserData::userSpdLevel + 1);
	//�����@�̃��x����\������	
	DrawFormatString(Game::kScreenWidth - 200, 600,//���W
		GetColor(0, 0, 0), "�t�@�C�ALv�@%d", UserData::userFireLevel + 1);
	//�����@�̃��x����\������
	DrawFormatString(Game::kScreenWidth - 200, 700,//���W
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
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2, 200, "�Q�[���N���A", GetColor(0, 0, 0));
		//�l�������S�[���h�ƌo���l��\������
		DrawString(300, 500, "�l���o���l", GetColor(0, 0, 0));
		DrawString(300, 600, "�����o���l", GetColor(0, 0, 0));
		DrawString(300, 700, "�l���S�[���h", GetColor(0, 0, 0));
		DrawString(300, 800, "�����S�[���h", GetColor(0, 0, 0));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(600, 500, GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());
		DrawFormatString(600, 600, GetColor(0, 0, 0), "%d", UserData::userExp);
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(600, 700, GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());
		DrawFormatString(600, 800, GetColor(0, 0, 0), "%d", UserData::userGold);
	}
}
