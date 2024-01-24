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
	//買えるアイテムの数
	constexpr int kMaxItemNum = 4;
	//配列のサイズ
	constexpr int kArraySize = 81;
	//マックスレベル
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

	//ファイルを開く
	std::ifstream ifs("./data/ItemPriceTable.txt");
	//帰ってきた値を返す配列
	vector<string> tempS;
	//配列を作成
	char str[kArraySize];
	//成功したら一行ずつ読み込む
	while (ifs.getline(str, kArraySize))
	{
		//分割
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
		//上キーが押されたら
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
		//下キーが入力されたら
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
	//上キーと下キーが離されたら
	if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&
		!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
	{
		m_isSelectKeyDown = false;
	}
	if (m_isKeyDown)
	{
		//Bボタンが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
		{
			//フェードしている間に連打されないためのif
			if (!m_isFade)
			{
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				//シーン移行する
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));
				m_isKeyDown = false;
				m_isFade = true;
			}
		}
	}
	//Aボタンが押されたら
	if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown || CheckHitKey(KEY_INPUT_RETURN) && m_isKeyDown)
	{
		switch (m_stageSelectNum)
		{
		case 0:
			if (UserData::userAtkLevel < kMaxLevel)
			{
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
	//所持金を表示する
	DrawFormatString(Game::kPlayScreenWidth, 50, GetColor(255, 255, 255), "%d", UserData::userGold);
	//仮実装
	DrawString(200, 100, "攻撃力", GetColor(255, 255, 255), true);
	DrawString(200, 200, "防御力", GetColor(255, 255, 255), true);
	DrawString(200, 300, "移動速度", GetColor(255, 255, 255), true);
	DrawString(200, 400, "ファイア", GetColor(255, 255, 255), true);
	DrawString(200, 500, "タイフーン", GetColor(255, 255, 255), true);

	//攻撃レベルがマックスだったらMAXと表示する
	if (UserData::userAtkLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(500, 100, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 100, GetColor(255, 255, 255), "%d", m_playerItemPriceList[UserData::userAtkLevel]);
	}
	//防御レベルがマックスだったらMAXと表示する
	if (UserData::userDefLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(500, 200, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 200, GetColor(255, 255, 255), "%d", m_playerItemPriceList[UserData::userDefLevel]);
	}
	//移動速度のレベルがマックスだったらMAXと表示する
	if (UserData::userSpdLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(500, 300, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 300, GetColor(255, 255, 255), "%d", m_playerItemPriceList[UserData::userSpdLevel]);
	}
	//炎魔法のレベルがマックスだったらMAXと表示する
	if (UserData::userFireLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(500, 400, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 400, GetColor(255, 255, 255), "%d", m_princessItemPriceList[UserData::userFireLevel]);
	}
	//風魔法のレベルがマックスだったらMAXと表示する
	if (UserData::userWindLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(500, 500, "MAX", GetColor(255, 255, 255), false);
	}
	else
	{
		DrawFormatString(500, 500, GetColor(255, 255, 255), "%d", m_princessItemPriceList[UserData::userWindLevel]);
	}
	switch (m_stageSelectNum)
	{
	case 0:
		DrawString(200, 100, "攻撃力", GetColor(255, 0, 0), true);
		break;
	case 1:
		DrawString(200, 200, "防御力", GetColor(255, 0, 0), true);
		break;
	case 2:
		DrawString(200, 300, "移動速度", GetColor(255, 0, 0), true);
		break;
	case 3:
		DrawString(200, 400, "ファイア", GetColor(255, 0, 0), true);
		break;
	case 4:
		DrawString(200, 500, "タイフーン", GetColor(255, 0, 0), true);
		break;
	}
}
