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
	//店主のいる座標
	constexpr int kGraphPosX = 200;
	constexpr int kGraphPosY = 300;
	//店主の大きさ
	constexpr int kGraphSize = 48 * 8;
	//Playerのいる座標
	constexpr int kPlayerPosX = 350;
	constexpr int kPlayerPosY = 750;
	//Princessのいる座標
	constexpr int kPrincessPosX = 450;
	constexpr int kPrincessPosY = 740;
}
SceneShop::SceneShop(SceneManager& sceneManager, DataManager& DataManager) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(0),
	m_isSelectKeyDown(false),
	m_loopCount(0),
	m_isFade(false),
	m_isShowString(true),
	m_isBuy(false)
{
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
	m_buySe = DataManager.SearchSound("buySe");
	m_missBuySe = DataManager.SearchSound("missBuySe");
	m_traderGraph = DataManager.SearchGraph("traderGraph");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
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
				m_isShowString = false;
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
}

void SceneShop::Draw()
{
	//商人を表示する
	DrawExtendGraph(kGraphPosX, kGraphPosY, kGraphPosX + kGraphSize, kGraphPosY + kGraphSize, m_traderGraph,true);
	//プレイヤーの背中を表示
	DrawRectRotaGraph(kPlayerPosX,kPlayerPosY,
		0, 64,
		16,16,
		8.0,
		0.0,
		m_playerGraph, true, false);
	DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
		0, 40,
		14, 20,
		8.0,
		0.0,
		m_princessGraph, true, false);
	//最初にいらっしゃいと表示する
	if (m_isShowString)
	{
		DrawString(230, 250, "いらっしゃい", GetColor(255, 255, 255), true);
	}
	if (m_isBuy && !m_isShowString)
	{
		DrawString(230, 250, "まいどあり", GetColor(255, 255, 255), true);
	}
	else if(!m_isBuy && !m_isShowString)
	{
		DrawString(230, 250, "おかねないね", GetColor(255, 255, 255), true);
	}
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
