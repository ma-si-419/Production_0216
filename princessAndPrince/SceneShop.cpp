#include "SceneShop.h"
#include "SceneManager.h"
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
}
SceneShop::SceneShop(SceneManager& manager) :
	Scene(manager),
	m_isKeyDown(false),
	m_selectNum(0),
	m_isSelectKeyDown(false),
	m_loopCount(0),
	m_isAKeyDown(true)
{
}

SceneShop::~SceneShop()
{
}

void SceneShop::Init()
{
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
	}
}

void SceneShop::Update(Pad& pad)
{
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//上キーが押されたら
	if (m_input.Buttons[0] && !m_isSelectKeyDown)
	{
		m_selectNum--;
		if (m_selectNum < 0)
		{
			m_selectNum = kMaxItemNum;
		}
		m_isSelectKeyDown = true;
	}
	//下キーが入力されたら
	else if (m_input.Buttons[1] && !m_isSelectKeyDown)
	{
		m_selectNum++;
		if (m_selectNum > kMaxItemNum)
		{
			m_selectNum = 0;
		}
		m_isSelectKeyDown = true;
	}
	//上キーと下キーが離されたら
	else if (!m_input.Buttons[0] && !m_input.Buttons[1])
	{
		m_isSelectKeyDown = false;
	}
	//Bボタンが押されたら
	if (m_input.Buttons[13])
	{
		m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
	}
	//Aボタンが押されたら
	if (m_input.Buttons[12] && m_isAKeyDown)
	{
		switch (m_selectNum)
		{
			switch (m_selectNum)
			{
			case 0:
				UserData::userAtkLevel++;
				m_isAKeyDown = false;
				break;
			case 1:
				UserData::userDefLevel++;
				m_isAKeyDown = false;
				break;
			case 2:
				UserData::userSpdLevel++;
				m_isAKeyDown = false;
				break;
			case 3:
				UserData::userFireLevel++;
				m_isAKeyDown = false;
				break;
			case 4:
				UserData::userWindLevel++;
				m_isAKeyDown = false;
				break;
			}
		}
	}
	else
	{
		m_isAKeyDown = true;
	}
}

void SceneShop::Draw()
{
	DrawString(200, 100, "攻撃力", GetColor(255, 255, 255), true);
	DrawString(200, 200, "防御力", GetColor(255, 255, 255), true);
	DrawString(200, 300, "移動速度", GetColor(255, 255, 255), true);
	DrawString(200, 400, "ファイア", GetColor(255, 255, 255), true);
	DrawString(200, 500, "タイフーン", GetColor(255, 255, 255), true);
	
	DrawFormatString(500, 100, GetColor(255, 255, 255),"%d", m_playerItemPriceList[UserData::userAtkLevel]);
	DrawFormatString(500, 200, GetColor(255, 255, 255),"%d", m_playerItemPriceList[UserData::userDefLevel]);
	DrawFormatString(500, 300, GetColor(255, 255, 255),"%d", m_playerItemPriceList[UserData::userSpdLevel]);
	DrawFormatString(500, 400, GetColor(255, 255, 255),"%d", m_princessItemPriceList[UserData::userFireLevel]);
	DrawFormatString(500, 500, GetColor(255, 255, 255),"%d", m_princessItemPriceList[UserData::userWindLevel]);
	switch (m_selectNum)
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
