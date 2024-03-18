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
	constexpr int kPrincessPosY = 750;
	//アイテムのサイズ
	constexpr int kItemGraphSize = 32;
	//アイテムのY座標
	constexpr int kItemPosY = 330;
	//まほうのY座標
	constexpr int kMagicPosY = 655;
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
	constexpr int kGPosX = 1450;
	//商品の後ろに表示するボックスの大きさ
	constexpr int kItemBackBoxSize = 180;
	//プレイヤーのアイテムのマークを表示するポジション
	constexpr int kPlayerItemMarkPosY = 180;
	constexpr int kPrincessItemMarkPosY = 510;
	//アイテムのX座標
	constexpr int kItemPosX[5] = { 800,1075,1350,930,1200 };
	//フレームのX座標
	constexpr int kFramePosX[5] = { 700,975,1250,835,1100 };
	//フレームの拡縮のスピード
	constexpr float kFrameZoomSpeed = 0.4f;
	//店主の説明を表示する座標
	constexpr int kExplanationPosX = 250;
	constexpr int kExplanationPosY = 250;
	//商品の後ろにあるボックスの色
	const int kItemBackBoxColor = GetColor(200, 152, 104);
	//商品の後ろにあるボックスを表示する座標
	constexpr int kItemBackBoxPosX[5] = { 710,985,1260,845,1110 };
	constexpr int kItemBackBoxPosY[2] = { 240,565 };
	//所持金を揺らす幅
	constexpr int kGoldShakeLange = 8;
	//プレイヤーの画像サイズ
	constexpr int kPlayerGraphSize = 16;
	//プレイヤーのアニメフレーム
	constexpr int kPlayerAnimFrame = 64;
	//プレイヤーの拡大率
	constexpr double kPlayerScale = 8.0;
	//プリンセスの画像サイズ
	constexpr int kPrincessGraphSize = 24;
	//プリンセスのアニメフレーム
	constexpr int kPrincessAnimFrame = 48;
	//プリンセスの拡大率
	constexpr double kPrincessScale = 6.0;
	//アイテムの拡大率
	constexpr double kItemScale = 4.0;
	//ファイアの回転率(横に向かすため)
	constexpr double kFireAngle = 4.72;
	//戻ると表示する座標
	constexpr int kLeaveButtonPosX = 115;
	constexpr int kLeaveButtonPosY = 800;
	//上下キーを押したときに移動する量
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
		//右キーが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_DPAD_RIGHT] || CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT))
		{
			//右に行けない場所じゃなければ
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
		//左キーが入力されたら
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_LEFT] || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))
		{
			//左に行けない場所でなければ
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
		//下キーが入力されたら
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))
		{
			//上の行にいたら
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
		//上キーが入力されたら
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))
		{
			//下の行にいたら
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
	//上下左右キーが離されたら
	if (!CheckHitKeyAll() && !m_input.Buttons[XINPUT_BUTTON_DPAD_UP]
		&& !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && !m_input.Buttons[XINPUT_BUTTON_DPAD_RIGHT]
		&& !m_input.Buttons[XINPUT_BUTTON_DPAD_LEFT])
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
				StopSoundMem(m_bgm);
				//シーン移行する
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectSceneNum), true);
				m_isKeyDown = false;
				m_isFade = true;
			}
		}
	}
	//Aボタンが押されたら
	if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown || CheckHitKey(KEY_INPUT_RETURN) && m_isKeyDown)
	{
		m_isMoveCursor = false;
		switch (static_cast<ShopItem>(m_itemSelectNum))
		{
		case kSword:
			if (UserData::userAtkLevel < kMaxLevel)
			{
				m_isShowString = false;
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
				//所持金が値段よりも大きかったら
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
		m_itemFrameRatio += kFrameZoomSpeed;
	}
	else
	{
		m_itemFrameRatio -= kFrameZoomSpeed;
	}
	//表示しているゴールドを減らしていく
	if (m_subGold > 0)
	{
		//減らす総量の桁数を取得して減らす量を決める
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
	//背景を表示する
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//戻るボタンを押す
	DrawString(kLeaveButtonPosX, kLeaveButtonPosY, "B：戻る", GetColor(0, 0, 0));
	//商品の後ろにボックスを表示
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
	//商人を表示する
	DrawExtendGraph(kGraphPosX, kGraphPosY, kGraphPosX + kGraphSize, kGraphPosY + kGraphSize, m_traderGraph, true);
	//プレイヤーの背中を表示
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
	
	//店主の言葉を表示する
	DrawWords();
	
	//所持金を表示する
	DrawExtendGraph(kBoxPosX, kBoxPosY, kBoxPosX + kBoxWidth, kBoxPosY + kBoxHeight, m_backBoxGraph, true);
	DrawFormatString(kGoldPosX - ArrRight(m_showGold), kGoldPosY + m_shakeGoldPosY, GetColor(255, 255, 255), "%d", m_showGold);
	DrawString(kGPosX, kGoldPosY, "G", GetColor(255, 255, 255));
	//アイテムの表示
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
	//レベルを表示する
	DrawLevel();
	
	//フレームを表示する
	DrawFrame(m_itemSelectNum);

	//買えるアイテムに目印をつける
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
		DrawFormatString(kAtkItemPosX + kPricePos - ArrRight(m_playerItemPriceList[UserData::userAtkLevel]), kPlayerItemLevelPosY + kFontSize, GetColor(0, 0, 0), " %d", m_playerItemPriceList[UserData::userAtkLevel]);

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
}

void SceneShop::DrawWords()
{
	int stringWidth;
	//最初にいらっしゃいと表示する
	if (m_isShowString)
	{
		stringWidth = GetStringLength("いらっしゃい") * kHalfFontSize;
		DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "いらっしゃい", GetColor(0, 0, 0), true);
	}
	//レベルマックスだったら
	if (m_isMax)
	{
		stringWidth = GetStringLength("もうげんかいだよ") * kHalfFontSize;
		DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "もうげんかいだよ", GetColor(0, 0, 0), true);
	}
	//レベルマックスじゃなかった場合
	else
	{
		//アイテムを買ったとき
		if (m_isBuy && !m_isShowString && !m_isMoveCursor && !m_isMax)
		{
			stringWidth = GetStringLength("まいどあり") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "まいどあり", GetColor(0, 0, 0), true);
		}
		//アイテムが買えなかったとき
		else if (!m_isBuy && !m_isShowString && !m_isMoveCursor && !m_isMax)
		{
			stringWidth = GetStringLength("おかねないね") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "おかねないね", GetColor(0, 0, 0), true);
		}
	}

	if (m_isMoveCursor && !m_isShowString)
	{
		switch (static_cast<ShopItem>(m_itemSelectNum))
		{
		case kSword:
			stringWidth = GetStringLength("こうげきあがるよ") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "こうげきあがるよ", GetColor(0, 0, 0), true);
			break;
		case kArmor:
			stringWidth = GetStringLength("ぼうぎょあがるよ") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "ぼうぎょあがるよ", GetColor(0, 0, 0), true);
			break;
		case kBoots:
			stringWidth = GetStringLength("そくどあがるよ") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "そくどあがるよ", GetColor(0, 0, 0), true);
			break;
		case kFire:
			stringWidth = GetStringLength("ファイアつよくなるよ") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "ファイアつよくなるよ", GetColor(0, 0, 0), true);
			break;
		case kWind:
			stringWidth = GetStringLength("ウィンドつよくなるよ") * kHalfFontSize;
			DrawString(kExplanationPosX - stringWidth / 2, kExplanationPosY, "ウィンドつよくなるよ", GetColor(0, 0, 0), true);
			break;
		default:
			break;
		}
	}
}

