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
}
SceneShop::SceneShop(SceneManager& sceneManager, DataManager& DataManager) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(0),
	m_isSelectKeyDown(false),
	m_loopCount(0),
	m_isFade(false)
{
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
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
			m_stageSelectNum--;
			PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			if (m_stageSelectNum < 0)
			{
				m_stageSelectNum = kMaxItemNum;
			}
			m_isSelectKeyDown = true;
		}
		//���L�[�����͂��ꂽ��
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
		{
			m_stageSelectNum++;
			PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			if (m_stageSelectNum > kMaxItemNum)
			{
				m_stageSelectNum = 0;

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
		switch (m_stageSelectNum)
		{
		case 0:
			if (UserData::userAtkLevel < kMaxLevel)
			{
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userAtkLevel])
				{

					UserData::userGold -= m_playerItemPriceList[UserData::userAtkLevel];
					UserData::userAtkLevel++;
				}
			}
			m_isKeyDown = false;
			break;
		case 1:
			if (UserData::userDefLevel < kMaxLevel)
			{
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userDefLevel])
				{
					UserData::userGold -= m_playerItemPriceList[UserData::userDefLevel];
					UserData::userDefLevel++;
				}
			}
			m_isKeyDown = false;
			break;
		case 2:
			if (UserData::userSpdLevel < kMaxLevel)
			{
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userSpdLevel])
				{
					UserData::userGold -= m_playerItemPriceList[UserData::userSpdLevel];
					UserData::userSpdLevel++;
				}
			}
			m_isKeyDown = false;
			break;
		case 3:
			if (UserData::userFireLevel < kMaxLevel)
			{
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userFireLevel])
				{
					UserData::userGold -= m_playerItemPriceList[UserData::userFireLevel];
					UserData::userFireLevel++;
				}
			}
			m_isKeyDown = false;
			break;
		case 4:
			if (UserData::userWindLevel < kMaxLevel)
			{
				//���������l�i�����傫��������
				if (UserData::userGold >= m_playerItemPriceList[UserData::userWindLevel])
				{
					UserData::userGold -= m_playerItemPriceList[UserData::userWindLevel];
					UserData::userWindLevel++;
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
}

void SceneShop::Draw()
{
	//��������\������
	DrawFormatString(Game::kPlayScreenWidth, 50, GetColor(255, 255, 255), "%d", UserData::userGold);
	//������
	DrawString(200, 100, "�U����", GetColor(255, 255, 255), true);
	DrawString(200, 200, "�h���", GetColor(255, 255, 255), true);
	DrawString(200, 300, "�ړ����x", GetColor(255, 255, 255), true);
	DrawString(200, 400, "�t�@�C�A", GetColor(255, 255, 255), true);
	DrawString(200, 500, "�^�C�t�[��", GetColor(255, 255, 255), true);

	//�U�����x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userAtkLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(500, 100, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 100, GetColor(255, 255, 255), "%d", m_playerItemPriceList[UserData::userAtkLevel]);
	}
	//�h�䃌�x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userDefLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(500, 200, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 200, GetColor(255, 255, 255), "%d", m_playerItemPriceList[UserData::userDefLevel]);
	}
	//�ړ����x�̃��x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userSpdLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(500, 300, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 300, GetColor(255, 255, 255), "%d", m_playerItemPriceList[UserData::userSpdLevel]);
	}
	//�����@�̃��x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userFireLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(500, 400, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 400, GetColor(255, 255, 255), "%d", m_princessItemPriceList[UserData::userFireLevel]);
	}
	//�����@�̃��x�����}�b�N�X��������MAX�ƕ\������
	if (UserData::userWindLevel == kMaxLevel)
	{
		//MAX�ƕ\������
		DrawString(500, 500, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 500, GetColor(255, 255, 255), "%d", m_princessItemPriceList[UserData::userWindLevel]);
	}
	switch (m_stageSelectNum)
	{
	case 0:
		DrawString(200, 100, "�U����", GetColor(255, 0, 0), true);
		break;
	case 1:
		DrawString(200, 200, "�h���", GetColor(255, 0, 0), true);
		break;
	case 2:
		DrawString(200, 300, "�ړ����x", GetColor(255, 0, 0), true);
		break;
	case 3:
		DrawString(200, 400, "�t�@�C�A", GetColor(255, 0, 0), true);
		break;
	case 4:
		DrawString(200, 500, "�^�C�t�[��", GetColor(255, 0, 0), true);
		break;
	}
}
