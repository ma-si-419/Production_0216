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
	//アイテムのサイズ
	constexpr int kItemGraphSize = 32;
	//アイテムのY座標
	constexpr int kItemPosY = 330;
	//まほうのY座標
	constexpr int kMagicPosY = 650;
	//フォントの大きさ
	constexpr int kFontSize = 48;
	//フォントの半角の大きさ(数字)
	constexpr int kHalfFontSize = 24;
	//フレームの大きさ
	constexpr int kFrameSize = 200;
	//フレームの大きくなる限界値
	constexpr int kFrameMaxRatio = 15;
	//プレイヤーアイテムのフレームのY座標
	constexpr int kPlayerItemFramePosY = 230;
	//PrincessアイテムのフレームのY座標
	constexpr int kPrincessItemFramePosY = 555;
	//プレイヤーアイテムのレベルを表示するポジション
	constexpr int kPlayerItemLevelPosY = 440;
	//Princessアイテムのレベルを表示するポジション
	constexpr int kPrincessItemLevelPosY = 760;
	//攻撃力アイテムのレベルを表示するポジション
	constexpr int kAtkItemPosX = 745;
	//防御力アイテムのレベルを表示するポジション
	constexpr int kDefItemPosX = 1025;
	//移動力アイテムのレベルを表示するポジション
	constexpr int kSpdItemPosX = 1295;
	//ファイアアイテムのレベルを表示するポジション
	constexpr int kFireItemPosX = 875;
	//ウィンドアイテムのレベルを表示するポジション
	constexpr int kWindItemPosX = 1145;
	//値段を表示するポジション
	constexpr int kPricePos = 72;
	//所持金を表示するボックスの横幅
	constexpr int kBoxWidth = 500;
	//所持金を表示するボックスの縦幅
	constexpr int kBoxHeight = 100;
	//所持金を表示するボックスの座標
	constexpr int kBoxPosX = 1000;
	constexpr int kBoxPosY = 100;
	//所持金を表示する座標
	constexpr int kGoldPosX = 1415;
	constexpr int kGoldPosY = 130;
	//所持金のGを表示する座標
	constexpr int kGPosX= 1450;
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
	m_backBoxGraph = DataManager.SearchGraph("backBoxGraph");
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
			m_itemSelectNum--;
			PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			if (m_itemSelectNum < 0)
			{
				m_itemSelectNum = kMaxItemNum;
			}
			m_isSelectKeyDown = true;
		}
		//下キーが入力されたら
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
		switch (m_itemSelectNum)
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
	//アイテムのフレームを拡大縮小する
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
	//背景を表示する
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//商人を表示する
	DrawExtendGraph(kGraphPosX, kGraphPosY, kGraphPosX + kGraphSize, kGraphPosY + kGraphSize, m_traderGraph, true);
	//プレイヤーの背中を表示
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
	//最初にいらっしゃいと表示する
	if (m_isShowString)
	{
		DrawString(230, 250, "いらっしゃい", GetColor(0, 0, 0), true);
	}
	if (m_isBuy && !m_isShowString)
	{
		DrawString(230, 250, "まいどあり", GetColor(0, 0, 0), true);
	}
	else if (!m_isBuy && !m_isShowString)
	{
		DrawString(230, 250, "おかねないね", GetColor(0, 0, 0), true);
	}
	//所持金を表示する
	DrawExtendGraph(kBoxPosX, kBoxPosY,kBoxPosX + kBoxWidth,kBoxPosY + kBoxHeight, m_backBoxGraph, true);
	DrawFormatString(kGoldPosX - ArrRight(UserData::userGold), kGoldPosY, GetColor(255, 255, 255), "%d", UserData::userGold);
	DrawString(kGPosX, kGoldPosY, "G", GetColor(255, 255, 255));
	//アイテムの表示
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

	//攻撃レベルがマックスだったらMAXと表示する
	if (UserData::userAtkLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(kAtkItemPosX, kPlayerItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//レベルの表示
		DrawString(kAtkItemPosX, kPlayerItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kAtkItemPosX + kPricePos - ArrRight(UserData::userAtkLevel + 1), kPlayerItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userAtkLevel + 1);
		//値段の表示
		DrawString(kAtkItemPosX, kPlayerItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kAtkItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userAtkLevel]), kPlayerItemLevelPosY + kFontSize,GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userAtkLevel]);

	}
	//防御レベルがマックスだったらMAXと表示する
	if (UserData::userDefLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(kDefItemPosX, kPlayerItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//レベルの表示
		DrawString(kDefItemPosX, kPlayerItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kDefItemPosX + kPricePos - ArrRight(UserData::userDefLevel + 1), kPlayerItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userDefLevel + 1);
		//値段の表示
		DrawString(kDefItemPosX, kPlayerItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kDefItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userDefLevel]), kPlayerItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userDefLevel]);
	}
	//移動速度のレベルがマックスだったらMAXと表示する
	if (UserData::userSpdLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(kSpdItemPosX, kPlayerItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//レベルの表示
		DrawString(kSpdItemPosX, kPlayerItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kSpdItemPosX + kPricePos - ArrRight(UserData::userSpdLevel + 1), kPlayerItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userSpdLevel + 1);
		//値段の表示
		DrawString(kSpdItemPosX, kPlayerItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kSpdItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userSpdLevel]), kPlayerItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userSpdLevel]);
	}
	//炎魔法のレベルがマックスだったらMAXと表示する
	if (UserData::userFireLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(kFireItemPosX, kPrincessItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//レベルの表示
		DrawString(kFireItemPosX, kPrincessItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kFireItemPosX + kPricePos - ArrRight(UserData::userFireLevel + 1), kPrincessItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userFireLevel + 1);
		//値段の表示
		DrawString(kFireItemPosX, kPrincessItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kFireItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userFireLevel]), kPrincessItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_princessItemPriceList[UserData::userFireLevel]);
	}
	//風魔法のレベルがマックスだったらMAXと表示する
	if (UserData::userWindLevel == kMaxLevel)
	{
		//MAXと表示する
		DrawString(kWindItemPosX, kPrincessItemLevelPosY, "M A X", GetColor(0, 0, 0), false);
	}
	else
	{
		//レベルの表示
		DrawString(kWindItemPosX, kPrincessItemLevelPosY, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kWindItemPosX + kPricePos - ArrRight(UserData::userWindLevel + 1), kPrincessItemLevelPosY, GetColor(0, 0, 0), " %d", UserData::userWindLevel + 1);
		//値段の表示
		DrawString(kWindItemPosX, kPrincessItemLevelPosY + kFontSize, "G", GetColor(0, 0, 0));
		DrawFormatString(kWindItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userWindLevel]), kPrincessItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_princessItemPriceList[UserData::userWindLevel]);
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

