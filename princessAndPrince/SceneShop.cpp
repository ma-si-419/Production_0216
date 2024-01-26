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
	constexpr int kPrincessPosY = 740;
	//�A�C�e���̃T�C�Y
	constexpr int kItemGraphSize = 32;
	//�A�C�e����Y���W
	constexpr int kItemPosY = 330;
	//�܂ق���Y���W
	constexpr int kMagicPosY = 650;
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
}
SceneShop::SceneShop(SceneManager& sceneManager, DataManager& DataManager) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_itemSelectNum(0),
	m_isSelectKeyDown(false),
	m_loopCount(0),
	m_isFade(false),
	m_isShowString(true),
	m_isBuy(false),
	m_isFrameRatio(true),
	m_itemFrameRatio(0)
{
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
	m_buySe = DataManager.SearchSound("buySe");
	m_missBuySe = DataManager.SearchSound("missBuySe");
	m_traderGraph = DataManager.SearchGraph("traderGraph");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
	m_bgGraph = DataManager.SearchGraph("shopBgGraph");
	m_itemGraph = DataManager.SearchGraph("itemGraph");
	m_itemFrameGraph = DataManager.SearchGraph("itemFrameGraph");
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
		tempItem.princessItemPrice = std::stof(tempS[1]);
		m_playerItemPriceList[m_loopCount] = tempItem.playerItemPrice;
		m_princessItemPriceList[m_loopCount] = tempItem.princessItemPrice;
		m_loopCount++;
	}
	ifs.close();

}

void SceneShop::Update(Pad& pad)
{
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	if (!m_isSelectKeyDown)
	{
		//��L�[�������ꂽ��
		if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
		{
			m_itemSelectNum--;
			PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			if (m_itemSelectNum < 0)
			{
				m_itemSelectNum = kMaxItemNum;
			}
			m_isSelectKeyDown = true;
		}
		//���L�[�����͂��ꂽ��
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
		{
			m_itemSelectNum++;
			PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			if (m_itemSelectNum > kMaxItemNum)
			{
				m_itemSelectNum = 0;

			}
			m_isSelectKeyDown = true;
		}
	}
	//��L�[�Ɖ��L�[�������ꂽ��
	if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&
		!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
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
				//�V�[���ڍs����
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));
				m_isKeyDown = false;
				m_isFade = true;
			}
		}
	}
	//A�{�^���������ꂽ��
	if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown || CheckHitKey(KEY_INPUT_RETURN) && m_isKeyDown)
	{
		switch (m_itemSelectNum)
		{
		case 0:
			if (UserData::userAtkLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userAtkLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_playerItemPriceList[UserData::userAtkLevel];
					UserData::userAtkLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			m_isKeyDown = false;
			break;
		case 1:
			if (UserData::userDefLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userDefLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_playerItemPriceList[UserData::userDefLevel];
					UserData::userDefLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			m_isKeyDown = false;
			break;
		case 2:
			if (UserData::userSpdLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userSpdLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_playerItemPriceList[UserData::userSpdLevel];
					UserData::userSpdLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			m_isKeyDown = false;
			break;
		case 3:
			if (UserData::userFireLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_princessItemPriceList[UserData::userFireLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_princessItemPriceList[UserData::userFireLevel];
					UserData::userFireLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
			}
			m_isKeyDown = false;
			break;
		case 4:
			if (UserData::userWindLevel < kMaxLevel)
			{
				m_isShowString = false;
				//���������l�i�����傫��������
				if (UserData::userGold >= m_princessItemPriceList[UserData::userWindLevel])
				{
					m_isBuy = true;
					PlaySoundMem(m_buySe, DX_PLAYTYPE_BACK);
					UserData::userGold -= m_princessItemPriceList[UserData::userWindLevel];
					UserData::userWindLevel++;
				}
				else
				{
					m_isBuy = false;
					PlaySoundMem(m_missBuySe, DX_PLAYTYPE_BACK);
				}
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
		m_itemFrameRatio += 0.4f;
	}
	else
	{
		m_itemFrameRatio -= 0.4f;
	}
}

void SceneShop::Draw()
{
	//�w�i��\������
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//���l��\������
	DrawExtendGraph(kGraphPosX, kGraphPosY, kGraphPosX + kGraphSize, kGraphPosY + kGraphSize, m_traderGraph, true);
	//�v���C���[�̔w����\��
	DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
		0, 64,
		16, 16,
		8.0,
		0.0,
		m_playerGraph, true, false);
	DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
		0, 40,
		14, 20,
		8.0,
		0.0,
		m_princessGraph, true, false);
	//�ŏ��ɂ�������Ⴂ�ƕ\������
	if (m_isShowString)
	{
		DrawString(230, 250, "��������Ⴂ", GetColor(0, 0, 0), true);
	}
	if (m_isBuy && !m_isShowString)
	{
		DrawString(230, 250, "�܂��ǂ���", GetColor(0, 0, 0), true);
	}
	else if (!m_isBuy && !m_isShowString)
	{
		DrawString(230, 250, "�����˂Ȃ���", GetColor(0, 0, 0), true);
	}
	//��������\������
	DrawFormatString(Game::kPlayScreenWidth, 50, GetColor(0, 0, 0), "%d", UserData::userGold);
	//������
	DrawRectRotaGraph(800, kItemPosY,
		0, Game::kSword * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		4.0,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(1075, kItemPosY,
		0, Game::kArmor * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		4.0,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(1350, kItemPosY,
		0, Game::kBoots * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		4.0,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(930, kMagicPosY,
		0, Game::kFire * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		4.0,
		0.0,
		m_itemGraph, true, false);
	DrawRectRotaGraph(1200, kMagicPosY,
		0, Game::kTyphoon * kItemGraphSize,
		kItemGraphSize, kItemGraphSize,
		4.0,
		0.0,
		m_itemGraph, true, false);

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
		DrawFormatString(kAtkItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userAtkLevel]), kPlayerItemLevelPosY + kFontSize,GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userAtkLevel]);

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
		DrawFormatString(kFireItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userFireLevel]), kPrincessItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userFireLevel]);
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
		DrawFormatString(kWindItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userWindLevel]), kPrincessItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userWindLevel]);
	}
	switch (m_itemSelectNum)
	{
	case 0:
		DrawExtendGraph(700 - static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY - static_cast<int>(m_itemFrameRatio),
			700 + kFrameSize + static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		break;
	case 1:
		DrawExtendGraph(975 - static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY - static_cast<int>(m_itemFrameRatio),
			975 + kFrameSize + static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		break;
	case 2:
		DrawExtendGraph(1250 - static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY - static_cast<int>(m_itemFrameRatio),
			1250 + kFrameSize + static_cast<int>(m_itemFrameRatio), kPlayerItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		break;
	case 3:
		DrawExtendGraph(835 - static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY - static_cast<int>(m_itemFrameRatio),
			835 + kFrameSize + static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		break;
	case 4:
		DrawExtendGraph(1100 - static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY - static_cast<int>(m_itemFrameRatio),
			1100 + kFrameSize + static_cast<int>(m_itemFrameRatio), kPrincessItemFramePosY + kFrameSize + static_cast<int>(m_itemFrameRatio), m_itemFrameGraph, true);
		break;
	}
}

int SceneShop::ArrRight(int num)
{
	if (num >= 1000)
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

