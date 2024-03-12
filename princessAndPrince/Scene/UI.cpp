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
	constexpr int kGetItemPosX = 1350;
	//�t�H���g�̌��ɕ\������e�̂��炷�傫��
	constexpr int kShiftShadowLange = 3;
	//�A�C�e���O���t�̑傫��
	constexpr int kItemGraphScale = 32;
	//�Q�[���N���A���̃v���C���[�̃A�j���t���[��
	constexpr int kPlayerClearAnimFrame = 48;
	//�Q�[���I�[�o�[���̃v���C���[�̃A�j���t���[��
	constexpr int kPlayerDeathAnimFrame = 16;
	//�v���C���[�̉摜�̑傫��
	constexpr int kPlayerGraphSize = 16;
	//�v���C���[�̊g�嗦
	constexpr double kPlayerScale = 6.0;
	//Ui��h�炷��
	constexpr int kShakeLange = 1;
	//���肵���o���l�ƃS�[���h��\��������W
	constexpr int kGetItemPosY[2] = { 105,175 };
	//Ui��\��������W
	constexpr int kUiPosYArr[6] = { 315,390,465,540,615,690 };
	//�{�^�����ǂ��܂ő傫���Ȃ邩
	constexpr double kAngryButtonMaxRatio = 0.8;
	//�{�^�����ǂ��܂ŏ������Ȃ邩
	constexpr double kAngryButtonMinRatio = 0.4;
	//�{�^�����傫���Ȃ�X�s�[�h
	constexpr double kAngryButtonZoomSpeed = 0.01;
	//���@�̃A�C�R����\��������W
	constexpr int kMagicUiPosX[2] = { 1320,1495 };
	constexpr int kMagicUiPosY = 855;
	//���@�̃A�C�R����\������w�i�摜�̃T�C�Y
	constexpr int kMagicUiBgSize = 106;
	//���@�̃A�C�R���̑傫���{��
	constexpr double kMagicUiRatio = 2.7;
	//�t�@�C�A�̃A�C�R���̊p�x
	constexpr double kFireUiAngle = 4.72;
	//�{��Q�[�W�̍��W
	constexpr int kAngryGaugePosX = 1060;
	constexpr int kAngryGaugePosY = 805;
	//�{��Q�[�W�̃t���[���̍��W
	constexpr int kAngryGaugeFramePosX = 1050;
	constexpr int kAngryGaugeFramePosY = 800;
	//���@��UI��\������Ƃ��ɏo���~�̑傫��
	constexpr int kMagicUiCircleRadius = 50;
	//�Q�[���N���A�ƕ\��������W
	constexpr int kGameClearPosX[2] = { 230,530 };
	//�Q�[���I�[�o�[�ƕ\��������W
	constexpr int kGameOverPosX[2] = { 200,500 };
	//�Q�[���I�[�o�[�̕����̒��ɏo���v���C���[�̍��W
	constexpr int kGameOverPlayerPos = 450;
	//�Q�[���N���A�̕����̒��ɏo���v���C���[�̍��W
	constexpr int kGameClearPlayerPos = 480;
	//�Q�[���N���A���̃��U���g��\��������W
	constexpr int kResultPosX[2] = {200,750};
	constexpr int kResultPosY[8] = { 210,380,480,580,680,780,900,880 };
	//�{�^���̃T�C�Y
	constexpr double kButtonSize = 2.5;
	//�{�^����\������|�W�V����
	constexpr int kButtonPosX = 440;
	//�߂��\������|�W�V����
	constexpr int kLeavePosX = 480;
	//�����{�b�N�X�̑傫��
	constexpr int kBlackBoxScale = 965;
	//���@���g���Ȃ��V�[��
	constexpr int kCantMagicScene = 2;
	//�{�胂�[�h���g���Ȃ��V�[��
	constexpr int kCantAngryModeScene = 3;
	//���@���g���Ȃ��Ƃ���UI�̐F
	const int CantMagicUiColor = GetColor(128, 128, 128);
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false),
	m_isLeaveButton(false),
	m_isClearUIEnd(false),
	m_angryGaugeUiShiftPosX(0),
	m_stoneAngryGaugeGraph(0),
	m_angryButtonGraph(0),
	m_angryButtonRatio(1.0),
	m_angryGaugeUiGraph(0),
	m_isAngryGaugeUiShake(false),
	m_isButtonZoom(false),
	m_magicGraph(0),
	m_magicUiBgGraph(0)


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
	//���肵���A�C�e����\������
	DrawGetItem();
	//�X�e�[�^�X��\������
	DrawStatus();
	//���ǂ���̖��@��ł��Ă��邩��\������
	if (m_pPrincess->GetMagicKind())
	{
		DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, kMagicUiBgSize, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);

	}
	else
	{
		DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, kMagicUiBgSize, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);

	}
	//�V�[��3�܂ł͂ǂ�������点�Ȃ��悤�ɂ���
	if (m_pMain->GetSceneNum() < kCantMagicScene)
	{
		DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
	}
	//�����@��Ui�\��
	DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, 0, static_cast<int>(Game::ItemGraph::kFire) * kItemGraphScale, kItemGraphScale, kItemGraphScale,
		kMagicUiRatio, kFireUiAngle, m_magicGraph, true, false, false);
	//�����@��Ui�\��
	DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, 0, static_cast<int>(Game::ItemGraph::kWind) * kItemGraphScale, kItemGraphScale, kItemGraphScale,
		kMagicUiRatio, 0.0, m_magicGraph, true, false, false);
	if (m_pMain->GetSceneNum() > 1)
	{
		//�g���Ă��閂�@���킩��₷������
		if (m_pPrincess->GetMagicKind())
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawCircle(kMagicUiPosX[1], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawCircle(kMagicUiPosX[0], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	//�V�[���R�܂ł͂ǂ�����D�F�ɕ\������
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawCircle(kMagicUiPosX[0], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
		DrawCircle(kMagicUiPosX[1], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//�{�胂�[�h�̃Q�[�W��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(kAngryGaugePosX, kAngryGaugePosY,//�n�_
		kAngryGaugePosX + (int)(kMaxBarWidth / m_pMain->GetSpecialGaugeRate()),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		kAngryGaugePosY + (int)kBarHeight, GetColor(255, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//�{�胂�[�h�̃Q�[�W�̃t���[����\������
	DrawGraph(kAngryGaugeFramePosX + m_angryGaugeUiShiftPosX, kAngryGaugeFramePosY, m_angryGaugeUiGraph, true);
	//�X�e�[�W�R�܂ł͓{�胂�[�h�̃Q�[�W��\�����Ȃ�
	if (m_pMain->GetSceneNum() < kCantAngryModeScene)
	{
		DrawGraph(kAngryGaugeFramePosX + m_angryGaugeUiShiftPosX, kAngryGaugeFramePosY, m_stoneAngryGaugeGraph, true);
	}
	if (m_pMain->GetSpecialGaugeRate() == 1)
	{
		DrawRotaGraph(kAngryGaugeFramePosX, kAngryGaugeFramePosY, m_angryButtonRatio, 0.0, m_angryButtonGraph, true, 0, 0);
	}
}
void UI::SceneClearUI()
{
	m_timeCount++;
	//�������������ȃ{�b�N�X��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, kBlackBoxScale, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		
		DrawString(kGameClearPosX[0], kResultPosY[0], "�Q �[ ��", GetColor(255, 255, 255));
		//�v���C���[�̉摜��\������
		DrawRectRotaGraph(kGameClearPlayerPos, kResultPosY[0],
			kPlayerClearAnimFrame, static_cast<int>(Game::Dir::kDirDeath) * kPlayerGraphSize,
			kPlayerGraphSize, kPlayerGraphSize,
			kPlayerScale,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString(kGameClearPosX[1], kResultPosY[0], "�N �� �A", GetColor(255, 255, 255));
		//���݂̃��x����\������
		DrawString(kResultPosX[0], kResultPosY[1], "���݂̃��x��", GetColor(255, 255, 255));
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userMainLevel + 1), kResultPosY[1], GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//�l�������S�[���h�ƌo���l��\������
		DrawString(kResultPosX[0], kResultPosY[2], "�l���o���l", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[3], "���̃��x���܂�", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[4], "�l���S�[���h", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[5], "�����S�[���h", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	//�o���l�̃��U���g��\������
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetExp()), kResultPosY[2], GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pMain->GetNextExp()), kResultPosY[3], GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	//�S�[���h�̃��U���g��\������
	if (m_isShowGold)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetGold()), kResultPosY[4], GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userGold), kResultPosY[5], GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	//A�{�^���Ŗ߂��\������
	if (m_isLeaveButton)
	{
		DrawRotaGraph(kButtonPosX, kResultPosY[6], kButtonSize, 0,m_buttonsGraph, true, 0, 0);
		DrawString(kLeavePosX, kResultPosY[7], "�Ŗ߂�", GetColor(255, 255, 255));
	}
}
void UI::GameOverUI()
{
	m_timeCount++;
	//�������������ȃ{�b�N�X��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, kBlackBoxScale, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		int stringWidth = GetDrawStringWidth("�Q�[���I�[�o�[", -1);
		DrawString(kGameOverPosX[0], kResultPosY[0], "�Q �[ ��", GetColor(255, 255, 255));
		DrawRectRotaGraph(kGameOverPlayerPos, kResultPosY[0],
			kPlayerDeathAnimFrame, static_cast<int>(Game::Dir::kDirDeath) * kPlayerGraphSize,
			kPlayerGraphSize, kPlayerGraphSize,
			kPlayerScale,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString(kGameOverPosX[1], kResultPosY[0], "�I �[ �o �[", GetColor(255, 255, 255));
		//���݂̃��x����\������
		DrawString(kResultPosX[0], kResultPosY[1], "���݂̃��x��", GetColor(255, 255, 255));
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userMainLevel + 1), kResultPosY[1], GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//�l�������S�[���h�ƌo���l��\������
		DrawString(kResultPosX[0], kResultPosY[2], "�l���o���l", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[3], "���̃��x���܂�", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[4], "�l���S�[���h", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[5], "�����S�[���h", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetExp()), kResultPosY[2], GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pMain->GetNextExp()), kResultPosY[3], GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetGold()), kResultPosY[4], GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userGold), kResultPosY[5], GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	if (m_isLeaveButton)
	{
		DrawRotaGraph(kButtonPosX, kResultPosY[6], kButtonSize, 0, m_buttonsGraph, true, 0, 0);
		DrawString(kLeavePosX, kResultPosY[7], "�Ŗ߂�", GetColor(255, 255, 255));
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
	//�X�e�[�W�Q�܂ł͊D�F�ŕ\�����Ă���
	if (m_pMain->GetSceneNum() > 1)
	{
		//�����@�̃��x����\������
		DrawString(kStatusPos, kUiPosYArr[4], "�t�@�C�ALv", GetColor(255, 255, 255));
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4],//���W
			GetColor(255, 255, 255), "%d", UserData::userFireLevel + 1);
		//�����@�̃��x����\������
		DrawString(kStatusPos, kUiPosYArr[5], "�E�B���hLv", GetColor(255, 255, 255));
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5],//���W
			GetColor(255, 255, 255), "%d", UserData::userWindLevel + 1);
	}
	else
	{
		//�����@�̃��x����\������
		DrawString(kStatusPos, kUiPosYArr[4], "�t�@�C�ALv", CantMagicUiColor);
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4],//���W
			CantMagicUiColor, "%d", UserData::userFireLevel + 1);
		//�����@�̃��x����\������
		DrawString(kStatusPos, kUiPosYArr[5], "�E�B���hLv", CantMagicUiColor);
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5],//���W
			CantMagicUiColor, "%d", UserData::userWindLevel + 1);
	}
}
void UI::DrawGetItem()
{
	//�e��\������
	{
		//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
		DrawFormatString(kGetItemPosX + kShiftShadowLange - AlignmentRight(m_pPlayer->GetGold()), kGetItemPosY[0] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());

		//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������	
		DrawFormatString(kGetItemPosX + kShiftShadowLange - AlignmentRight(m_pPlayer->GetExp()), kGetItemPosY[1] + kShiftShadowLange,//���W
			GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());
	}

	//���肵���S�[���h�̗ʂ��v���C���[����擾���ĕ\������
	DrawFormatString(kGetItemPosX - AlignmentRight(m_pPlayer->GetGold()), kGetItemPosY[0],//���W
		GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());

	//���肵���o���l�̗ʂ��v���C���[����擾���ĕ\������	
	DrawFormatString(kGetItemPosX - AlignmentRight(m_pPlayer->GetExp()), kGetItemPosY[1],//���W
		GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
}