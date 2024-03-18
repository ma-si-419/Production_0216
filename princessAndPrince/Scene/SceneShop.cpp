#include "SceneShop.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSelect.h"
#include <memory>
#include "MyString.h"
#include "UserData.h"
namespace
{
	//������A�C�e���̐�
	constexpr int kMaxItemNum = 4;
	//�z��̃T�C�Y
	constexpr int kArraySize = 81;
	//�}�b�N�X���x��
	constexpr int kMaxLevel = 19;
	//�X��̂�����W
	constexpr int kGraphPosX = 200;
	constexpr int kGraphPosY = 300;
	//�X��̑傫��
	constexpr int kGraphSize = 48 * 8;
	//Player�̂�����W
	constexpr int kPlayerPosX = 350;
	constexpr int kPlayerPosY = 750;
	//Princess�̂�����W
	constexpr int kPrincessPosX = 450;
	constexpr int kPrincessPosY = 750;
	//�A�C�e���̃T�C�Y
	constexpr int kItemGraphSize = 32;
	//�A�C�e����Y���W
	constexpr int kItemPosY = 330;
	//�܂ق���Y���W
	constexpr int kMagicPosY = 655;
	//�t�H���g�̑傫��
	constexpr int kFontSize = 48;
	//�t�H���g�̔��p�̑傫��(����)
	constexpr int kHalfFontSize = 24;
	//�t���[���̑傫��
	constexpr int kFrameSize = 200;
	//�t���[���̑傫���Ȃ���E�l
	constexpr int kFrameMaxRatio = 15;
	//�v���C���[�A�C�e���̃t���[����Y���W
	constexpr int kPlayerItemFramePosY = 230;
	//Princess�A�C�e���̃t���[����Y���W
	constexpr int kPrincessItemFramePosY = 555;
	//�v���C���[�A�C�e���̃��x����\������|�W�V����
	constexpr int kPlayerItemLevelPosY = 440;
	//Princess�A�C�e���̃��x����\������|�W�V����
	constexpr int kPrincessItemLevelPosY = 760;
	//�U���̓A�C�e���̃��x����\������|�W�V����
	constexpr int kAtkItemPosX = 745;
	//�h��̓A�C�e���̃��x����\������|�W�V����
	constexpr int kDefItemPosX = 1025;
	//�ړ��̓A�C�e���̃��x����\������|�W�V����
	constexpr int kSpdItemPosX = 1295;
	//�t�@�C�A�A�C�e���̃��x����\������|�W�V����
	constexpr int kFireItemPosX = 875;
	//�E�B���h�A�C�e���̃��x����\������|�W�V����
	constexpr int kWindItemPosX = 1145;
	//�l�i��\������|�W�V����
	constexpr int kPricePos = 72;
	//��������\������{�b�N�X�̉���
	constexpr int kBoxWidth = 500;
	//��������\������{�b�N�X�̏c��
	constexpr int kBoxHeight = 100;
	//��������\������{�b�N�X�̍��W
	constexpr int kBoxPosX = 1000;
	constexpr int kBoxPosY = 100;
	//��������\��������W
	constexpr int kGoldPosX = 1415;
	constexpr int kGoldPosY = 130;
	//��������G��\��������W
	constexpr int kGPosX = 1450;
	//���i�̌��ɕ\������{�b�N�X�̑傫��
	constexpr int kItemBackBoxSize = 180;
	//�v���C���[�̃A�C�e���̃}�[�N��\������|�W�V����
	constexpr int kPlayerItemMarkPosY = 180;
	constexpr int kPrincessItemMarkPosY = 510;
	//�A�C�e����X���W
	constexpr int kItemPosX[5] = { 800,1075,1350,930,1200 };
	//�t���[����X���W
	constexpr int kFramePosX[5] = { 700,975,1250,835,1100 };
	//�t���[���̊g�k�̃X�s�[�h
	constexpr float kFrameZoomSpeed = 0.4f;
	//�X��̐�����\��������W
	constexpr int kExplanationPosX = 250;
	constexpr int kExplanationPosY = 250;
	//���i�̌��ɂ���{�b�N�X�̐F
	const int kItemBackBoxColor = GetColor(200, 152, 104);
	//���i�̌��ɂ���{�b�N�X��\��������W
	constexpr int kItemBackBoxPosX[5] = { 710,985,1260,845,1110 };
	constexpr int kItemBackBoxPosY[2] = { 240,565 };
	//��������h�炷��
	constexpr int kGoldShakeLange = 8;
	//�v���C���[�̉摜�T�C�Y
	constexpr int kPlayerGraphSize = 16;
	//�v���C���[�̃A�j���t���[��
	constexpr int kPlayerAnimFrame = 64;
	//�v���C���[�̊g�嗦
	constexpr double kPlayerScale = 8.0;
	//�v�����Z�X�̉摜�T�C�Y
	constexpr int kPrincessGraphSize = 24;
	//�v�����Z�X�̃A�j���t���[��
	constexpr int kPrincessAnimFrame = 48;
	//�v�����Z�X�̊g�嗦
	constexpr double kPrincessScale = 6.0;
	//�A�C�e���̊g�嗦
	constexpr double kItemScale = 4.0;
	//�t�@�C�A�̉�]��(���Ɍ���������)
	constexpr double kFireAngle = 4.72;
	//�߂�ƕ\��������W
	constexpr int kLeaveButtonPosX = 115;
	constexpr int kLeaveButtonPosY = 800;
	//�㉺�L�[���������Ƃ��Ɉړ������
	constexpr int kMoveUpDownLange = 3;
}
SceneShop::SceneShop(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_itemSelectNum(0),
	m_isSelectKeyDown(false),
	m_loopCount(0),
	m_isFade(false),
	m_isShowString(true),
	m_isBuy(false),
	m_isFrameRatio(true),
	m_itemFrameRatio(0),
	m_selectSceneNum(selectSceneNum),
	m_isMax(false),
	m_showGold(UserData::userGold),
	m_subGold(0),
	m_shakeGoldPosY(0),
	m_isMoveCursor(false),
	m_playerItemPriceList(),
	m_princessItemPriceList()
{
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
	m_buySe = DataManager.SearchSound("buySe");
	m_missBuySe = DataManager.SearchSound("missBuySe");
	m_bgm = DataManager.SearchSound("shopBgm");
	m_traderGraph = DataManager.SearchGraph("traderGraph");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
	m_bgGraph = DataManager.SearchGraph("shopBgGraph");
	m_itemGraph = DataManager.SearchGraph("itemGraph");
	m_itemFrameGraph = DataManager.SearchGraph("itemFrameGraph");
	m_backBoxGraph = DataManager.SearchGraph("backBoxGraph");
	m_canBuyItemMark = DataManager.SearchGraph("canBuyItemMarkGraph");
	m_backButtonGraph = DataManager.SearchGraph("backButtonGraph");
}

SceneShop::~SceneShop()
{
}

void SceneShop::Init()
{

	//�t�@�C�����J��
	std::ifstream ifs("./data/ItemPriceTable.txt");
	//�A���Ă����l��Ԃ��z��
	vector<string> tempS;
	//�z����쐬
	char str[kArraySize];
	//�����������s���ǂݍ���
	while (ifs.getline(str, kArraySize))
	{
		//����
		tempS = MyString::split(str, ",");
		ItemPrice tempItem;
		tempItem.playerItemPrice = std::stoi(tempS[0]);
		tempItem.princessItemPrice = std::stoi(tempS[1]);
		m_playerItemPriceList[m_loopCount] = tempItem.playerItemPrice;
		m_princessItemPriceList[m_loopCount] = tempItem.princessItemPrice;
		m_loopCount++;
	}
	ifs.close();

}

void SceneShop::Update(Pad& pad)
{
	if (!CheckSoundMem(m_bgm) && !m_isFade)
	{
		PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
	}
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	if (!m_isSelectKeyDown)
	{
		//�E�L�[�������ꂽ��
		if (m_input.Buttons[XINPUT_BUTTON_DPAD_RIGHT] || CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT))
		{
			//�E�ɍs���Ȃ��ꏊ����Ȃ����
			if (m_itemSelectNum != kBoots && m_itemSelectNum != kWind)
			{
				m_itemSelectNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
			if (m_itemSelectNum < 0)
			{
				m_itemSelectNum = kMaxItemNum;
			}
			m_isSelectKeyDown = true;
			m_isMoveCursor = true;
			m_isShowString = false;
			m_isMax = false;
		}
		//���L�[�����͂��ꂽ��
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_LEFT] || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))
		{
			//���ɍs���Ȃ��ꏊ�łȂ����
			if (m_itemSelectNum != kSword && m_itemSelectNum != kFire)
			{
				m_itemSelectNum--;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
			if (m_itemSelectNum > kMaxItemNum)
			{
				m_itemSelectNum = 0;

			}
			m_isSelectKeyDown = true;
			m_isMoveCursor = true;
			m_isShowString = false;
			m_isMax = false;

		}
		//���L�[�����͂��ꂽ��
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))
		{
			//��̍s�ɂ�����
			if (m_itemSelectNum < kFire)
			{
				m_itemSelectNum += kMoveUpDownLange;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
			if (m_itemSelectNum > kMaxItemNum)
			{
				m_itemSelectNum = kMaxItemNum;

			}
			m_isSelectKeyDown = true;
			m_isMoveCursor = true;
			m_isShowString = false;
			m_isMax = false;

		}
		//��L�[�����͂��ꂽ��
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))
		{
			//���̍s�ɂ�����
			if (m_itemSelectNum > kBoots)
			{
				m_itemSelectNum -= kMoveUpDownLange;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
			if (m_itemSelectNum > kMaxItemNum)
			{
				m_itemSelectNum = 0;

			}
			m_isSelectKeyDown = true;
			m_isMoveCursor = true;
			m_isShowString = false;
			m_isMax = false;

		}
	}
	//�㉺���E�L�[�������ꂽ��
	if (!CheckHitKeyAll() && !m_input.Buttons[XINPUT_BUTTON_DPAD_UP]
		&& !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && !m_input.Buttons[XINPUT_BUTTON_DPAD_RIGHT]
		&& !m_input.Buttons[XINPUT_BUTTON_DPAD_LEFT])
	{
		m_isSelectKeyDown = false;
	}
	if (m_isKeyDown)
	{
		//B�{�^���������ꂽ��
		if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
		{
			//�t�F�[�h���Ă���ԂɘA�ł���Ȃ����߂�if
			if (!m_isFade)
			{
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				StopSoundMem(m_bgm);
				//�V�[���ڍs����
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectSceneNum), true);
				m_isKeyDown = false;
				m_isFade = true;
			}
		}
	}
	//A�{�^���������ꂽ��
	if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown || CheckHitKey(KEY_INPUT_RETURN) && m_isKeyDown)
	{
		m_isMoveCursor = false;
		switch (static_cast<ShopItem>(m_itemSelectNum))
		{
		case kSword:
			if (UserData::userAtkLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userAtkLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_playerItemPriceList[UserData::userAtkLevel];
					m_subGold += m_playerItemPriceList[UserData::userAtkLevel];
					UserData::userAtkLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			if (UserData::userAtkLevel == kMaxLevel)
			{
				m_isMax = true;
				PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);

			}
			m_isKeyDown = false;
			break;
		case kArmor:
			if (UserData::userDefLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userDefLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_playerItemPriceList[UserData::userDefLevel];
					m_subGold += m_playerItemPriceList[UserData::userDefLevel];
					UserData::userDefLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			if (UserData::userDefLevel == kMaxLevel)
			{
				m_isMax = true;
				PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);

			}
			m_isKeyDown = false;
			break;
		case kBoots:
			if (UserData::userSpdLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userSpdLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_playerItemPriceList[UserData::userSpdLevel];
					m_subGold += m_playerItemPriceList[UserData::userSpdLevel];
					UserData::userSpdLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			if (UserData::userSpdLevel == kMaxLevel)
			{
				m_isMax = true;
				PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);

			}
			m_isKeyDown = false;
			break;
		case kFire:
			if (UserData::userFireLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_princessItemPriceList[UserData::userFireLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_princessItemPriceList[UserData::userFireLevel];
					m_subGold += m_princessItemPriceList[UserData::userFireLevel];
					UserData::userFireLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			if (UserData::userFireLevel == kMaxLevel)
			{
				m_isMax = true;
				PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);

			}
			m_isKeyDown = false;
			break;
		case kWind:
			if (UserData::userWindLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_princessItemPriceList[UserData::userWindLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_princessItemPriceList[UserData::userWindLevel];
					m_subGold += m_princessItemPriceList[UserData::userWindLevel];
					UserData::userWindLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			if (UserData::userWindLevel == kMaxLevel)
			{
				m_isMax = true;
				PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);


			}
			m_isKeyDown = false;
			break;
		}
	}
	else if (!m_input.Buttons[XINPUT_BUTTON_A] && !CheckHitKey(KEY_INPUT_ESCAPE) &&
		!CheckHitKey(KEY_INPUT_RETURN))
	{
		m_isKeyDown = true;
	}
	//�A�C�e���̃t���[�����g��k������
	if (m_itemFrameRatio > kFrameMaxRatio)
	{
		m_isFrameRatio = false;
	}
	else if (m_itemFrameRatio < 0)
	{
		m_isFrameRatio = true;
	}
	if (m_isFrameRatio)
	{
		m_itemFrameRatio += kFrameZoomSpeed;
	}
	else
	{
		m_itemFrameRatio -= kFrameZoomSpeed;
	}
	//�\�����Ă���S�[���h�����炵�Ă���
	if (m_subGold > 0)
	{
		//���炷���ʂ̌������擾���Č��炷�ʂ����߂�
		int temp = GetDigits(m_subGold);
		m_showGold -= temp;
		m_subGold -= temp;
		m_shakeGoldPosY = GetRand(kGoldShakeLange) - kGoldShakeLange / 2;
	}
	else
	{
		m_shakeGoldPosY = 0;
	}
}

void SceneShop::Draw()
{
	//�w�i��\������
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//�߂�{�^��������
	DrawString(kLeaveButtonPosX, kLeaveButtonPosY, "B�F�߂�", GetColor(0, 0, 0));
	//���i�̌��Ƀ{�b�N�X��\��
	DrawBox(kItemBackBoxPosX[kSword], kItemBackBoxPosY[0],
		kItemBackBoxPosX[kSword] + kItemBackBoxSize, kPlayerItemFramePosY + kItemBackBoxSize, kItemBackBoxColor, true);
	DrawBox(kItemBackBoxPosX[kArmor], kItemBackBoxPosY[0],
		kItemBackBoxPosX[kArmor] + kItemBackBoxSize, kPlayerItemFramePosY + kItemBackBoxSize, kItemBackBoxColor, true);
	DrawBox(kItemBackBoxPosX[kBoots], kItemBackBoxPosY[0],
		kItemBackBoxPosX[kBoots] + kItemBackBoxSize, kPlayerItemFramePosY + kItemBackBoxSize, kItemBackBoxColor, true);
	DrawBox(kItemBackBoxPosX[kFire], kItemBackBoxPosY[1],
		kItemBackBoxPosX[kFire] + kItemBackBoxSize, kPrincessItemFramePosY + kItemBackBoxSize, kItemBackBoxColor, true);
	DrawBox(kItemBackBoxPosX[kWind], kItemBackBoxPosY[1],
		kItemBackBoxPosX[kWind] + kItemBackBoxSize, kPrincessItemFramePosY + kItemBackBoxSize, kItemBackBoxColor, true);
	//���l��\������
	DrawExtendGraph(kGraphPosX, kGraphPosY, kGraphPosX + kGraphSize, kGraphPosY + kGraphSize, m_traderGraph, true);
	//�v���C���[�̔w����\��
	DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
		0, kPlayerAnimFrame,
		kPlayerGraphSize, kPlayerGraphSize,
		kPlayerScale,
		0.0,
		m_playerGraph, true, false);
	DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
		0, kPrincessAnimFrame,
		kPrincessGraphSize, kPrincessGraphSize,
		kPrincessScale,
		0.0,
		m_princessGraph, true, false);
	
	//�X��̌��t��\������
	DrawWords();
	
	//��������\������
	DrawExtendGraph(kBoxPosX, kBoxPosY, kBoxPosX + kBoxWidth, kBoxPosY + kBoxHeight, m_backBoxGraph, true);
	DrawFormatString(kGoldPosX - ArrRight(m_showGold), kGoldPosY + m_shakeGoldPosY, GetColor(255, 255, 255), "%d", m_showGold);
	DrawString(kGPosX, kGoldPosY, "G", GetColor(255, 255, 255));
	//�A�C�e���̕\��
	DrawRectRotaGraph(kItemPosX[kSword], kItemPosY,
		0, static_cast<int>(Game::ItemGraph::kSword) * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		kItemScale,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(kItemPosX[kArmor], kItemPosY,
		0, static_cast<int>(Game::ItemGraph::kArmor) * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		kItemScale,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(kItemPosX[kBoots], kItemPosY,
		0, static_cast<int>(Game::ItemGraph::kBoots) * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		kItemScale,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(kItemPosX[kFire], kMagicPosY,
		0, static_cast<int>(Game::ItemGraph::kFire) * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		kItemScale,
		kFireAngle,
		m_itemGraph, true, false);
	DrawRectRotaGraph(kItemPosX[kWind], kMagicPosY,
		0, static_cast<int>(Game::ItemGraph::kWind) * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		kItemScale,
		0.0,
		m_itemGraph, true, false);
	//���x����\������
	DrawLevel();
	
	//�t���[����\������
	DrawFrame(m_itemSelectNum);

	//������A�C�e���ɖڈ������
	DrawRedCircle();
}

int SceneShop::ArrRight(int num)
{
	if (num >= 1000000)
	{
		return 6 * kHalfFontSize;
	}
	else if (num >= 100000)
	{
		return 5 * kHalfFontSize;
	}
	else if (num >= 10000)
	{
		return 4 * kHalfFontSize;
	}
	else if (num >= 1000)
	{
		return 3 * kHalfFontSize;
	}
	else if (num >= 100)
	{
		return 2 * kHalfFontSize;
	}
	else if (num >= 10)
	{
		return 1 * kHalfFontSize;
	}
	else
	{
		return 0;
	}
}

int SceneShop::GetDigits(int num)
{
	if (num > 1000)
	{
		return GetRand(1000);
	}
	else if (num > 100)
	{
		return GetRand(100);
	}
	else if (num > 10)
	{
		return GetRand(10);
	}
	else if (num > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void SceneShop::DrawRedCircle()
{
	if (UserData::userGold >= m_playerItemPriceList[UserData::userAtkLevel])
	{
		DrawGraph(kItemPosX[kSword], kPlayerItemMarkPosY, m_canBuyItemMark, true);
	}
	if (UserData::userGold >= m_playerItemPriceList[UserData::userDefLevel])
	{
		DrawGraph(kItemPosX[kArmor], kPlayerItemMarkPosY, m_canBuyItemMark, true);
	}
	if (UserData::userGold >= m_playerItemPriceList[UserData::userSpdLevel])
	{
		DrawGraph(kItemPosX[kBoots], kPlayerItemMarkPosY, m_canBuyItemMark, true);
	}
	if (UserData::userGold >= m_princessItemPriceList[UserData::userFireLevel])
	{
		DrawGraph(kItemPosX[kFire], kPrincessItemMarkPosY, m_canBuyItemMark, true);
	}
	if (UserData::userGold >= m_princessItemPriceList[UserData::userWindLevel])
	{
		DrawGraph(kItemPosX[kWind], kPrincessItemMarkPosY, m_canBuyItemMark, true);
	}
}

void SceneShop::DrawFrame(int selectNum)
{
	switch (static_cast<ShopItem>(selectNum))
	{
	case kSword:
		DrawExtendGraph(kFramePosX[static_cast<int>(kSword)] - static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY - static_cast<int>(m_itemFrameRatio),
			kFramePosX[static_cast<int>(kSword)] + kFrameSize + static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kArmor], kPlayerItemFramePosY,
			kFramePosX[kArmor] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kBoots], kPlayerItemFramePosY,
			kFramePosX[kBoots] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kFire], kPrincessItemFramePosY,
			kFramePosX[kFire] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kWind], kPrincessItemFramePosY,
			kFramePosX[kWind] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		break;
	case kArmor:
		DrawExtendGraph(kFramePosX[static_cast<int>(kSword)], kPlayerItemFramePosY,
			kFramePosX[static_cast<int>(kSword)] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kArmor] - static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY - static_cast<int>(m_itemFrameRatio),
			kFramePosX[kArmor] + kFrameSize + static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kBoots], kPlayerItemFramePosY,
			kFramePosX[kBoots] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kFire], kPrincessItemFramePosY,
			kFramePosX[kFire] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kWind], kPrincessItemFramePosY,
			kFramePosX[kWind] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		break;
	case kBoots:
		DrawExtendGraph(kFramePosX[static_cast<int>(kSword)], kPlayerItemFramePosY,
			kFramePosX[static_cast<int>(kSword)] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kArmor], kPlayerItemFramePosY,
			kFramePosX[kArmor] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kBoots] - static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY - static_cast<int>(m_itemFrameRatio),
			kFramePosX[kBoots] + kFrameSize + static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kFire], kPrincessItemFramePosY,
			kFramePosX[kFire] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kWind], kPrincessItemFramePosY,
			kFramePosX[kWind] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		break;
	case kFire:
		DrawExtendGraph(kFramePosX[static_cast<int>(kSword)], kPlayerItemFramePosY,
			kFramePosX[static_cast<int>(kSword)] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kArmor], kPlayerItemFramePosY,
			kFramePosX[kArmor] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kBoots], kPlayerItemFramePosY,
			kFramePosX[kBoots] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kFire] - static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY - static_cast<int>(m_itemFrameRatio),
			kFramePosX[kFire] + kFrameSize + static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kWind], kPrincessItemFramePosY,
			kFramePosX[kWind] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		break;
	case kWind:
		DrawExtendGraph(kFramePosX[static_cast<int>(kSword)], kPlayerItemFramePosY,
			kFramePosX[static_cast<int>(kSword)] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kArmor], kPlayerItemFramePosY,
			kFramePosX[kArmor] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kBoots], kPlayerItemFramePosY,
			kFramePosX[kBoots] + kFrameSize, kPlayerItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kFire], kPrincessItemFramePosY,
			kFramePosX[kFire] + kFrameSize, kPrincessItemFramePosY + kFrameSize, m_itemFrameGraph, true);
		DrawExtendGraph(kFramePosX[kWind] - static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY - static_cast<int>(m_itemFrameRatio),
			kFramePosX[kWind] + kFrameSize + static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		break;
	}
}

void SceneShop::DrawLevel()
{
	//�U�����x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userAtkLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(kAtkItemPosX, kPlayerItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//���x���̕\��
		DrawString(kAtkItemPosX, kPlayerItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kAtkItemPosX + kPricePos - ArrRight(UserData::userAtkLevel + 1), kPlayerItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userAtkLevel + 1);
		//�l�i�̕\��
		DrawString(kAtkItemPosX, kPlayerItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kAtkItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userAtkLevel]), kPlayerItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userAtkLevel]);

	}
	//�h�䃌�x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userDefLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(kDefItemPosX, kPlayerItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//���x���̕\��
		DrawString(kDefItemPosX, kPlayerItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kDefItemPosX + kPricePos - ArrRight(UserData::userDefLevel + 1), kPlayerItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userDefLevel + 1);
		//�l�i�̕\��
		DrawString(kDefItemPosX, kPlayerItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kDefItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userDefLevel]), kPlayerItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userDefLevel]);
	}
	//�ړ����x�̃��x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userSpdLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(kSpdItemPosX, kPlayerItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//���x���̕\��
		DrawString(kSpdItemPosX, kPlayerItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kSpdItemPosX + kPricePos - ArrRight(UserData::userSpdLevel + 1), kPlayerItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userSpdLevel + 1);
		//�l�i�̕\��
		DrawString(kSpdItemPosX, kPlayerItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kSpdItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userSpdLevel]), kPlayerItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userSpdLevel]);
	}
	//�����@�̃��x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userFireLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(kFireItemPosX, kPrincessItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//���x���̕\��
		DrawString(kFireItemPosX, kPrincessItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kFireItemPosX + kPricePos - ArrRight(UserData::userFireLevel + 1), kPrincessItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userFireLevel + 1);
		//�l�i�̕\��
		DrawString(kFireItemPosX, kPrincessItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kFireItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userFireLevel]), kPrincessItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_princessItemPriceList[UserData::userFireLevel]);
	}
	//�����@�̃��x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userWindLevel == kMaxLevel)


	{
		//MAX�ƕ\������
		DrawString(kWindItemPosX, kPrincessItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//���x���̕\��
		DrawString(kWindItemPosX, kPrincessItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kWindItemPosX + kPricePos - ArrRight(UserData::userWindLevel + 1), kPrincessItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userWindLevel + 1);
		//�l�i�̕\��
		DrawString(kWindItemPosX, kPrincessItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kWindItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userWindLevel]), kPrincessItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_princessItemPriceList[UserData::userWindLevel]);
	}
}

void SceneShop::DrawWords()
{
	int stringWidth;
	//�ŏ��ɂ�������Ⴂ�ƕ\������
	if (m_isShowString)
	{
		stringWidth = GetStringLength("��������Ⴂ") * kHalfFontSize;
		DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "��������Ⴂ", GetColor(0, 0, 0), true);
	}
	//���x���}�b�N�X��������
	if (m_isMax)
	{
		stringWidth = GetStringLength("�������񂩂�����") * kHalfFontSize;
		DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�������񂩂�����", GetColor(0, 0, 0), true);
	}
	//���x���}�b�N�X����Ȃ������ꍇ
	else
	{
		//�A�C�e���𔃂����Ƃ�
		if (m_isBuy && !m_isShowString && !m_isMoveCursor && !m_isMax)
		{
			stringWidth = GetStringLength("�܂��ǂ���") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�܂��ǂ���", GetColor(0, 0, 0), true);
		}
		//�A�C�e���������Ȃ������Ƃ�
		else if (!m_isBuy && !m_isShowString && !m_isMoveCursor && !m_isMax)
		{
			stringWidth = GetStringLength("�����˂Ȃ���") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�����˂Ȃ���", GetColor(0, 0, 0), true);
		}
	}

	if (m_isMoveCursor && !m_isShowString)
	{
		switch (static_cast<ShopItem>(m_itemSelectNum))
		{
		case kSword:
			stringWidth = GetStringLength("���������������") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "���������������", GetColor(0, 0, 0), true);
			break;
		case kArmor:
			stringWidth = GetStringLength("�ڂ����傠�����") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�ڂ����傠�����", GetColor(0, 0, 0), true);
			break;
		case kBoots:
			stringWidth = GetStringLength("�����ǂ������") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�����ǂ������", GetColor(0, 0, 0), true);
			break;
		case kFire:
			stringWidth = GetStringLength("�t�@�C�A�悭�Ȃ��") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�t�@�C�A�悭�Ȃ��", GetColor(0, 0, 0), true);
			break;
		case kWind:
			stringWidth = GetStringLength("�E�B���h�悭�Ȃ��") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "�E�B���h�悭�Ȃ��", GetColor(0, 0, 0), true);
			break;
		default:
			break;
		}
	}
}

