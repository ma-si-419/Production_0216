#include "SceneSelect.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneShop.h"
#include "UserData.h"
#include "Player.h"
namespace
{
	//ステージの数
	constexpr int kMaxSceneNum = 7;
	//ポーズを開いた時の項目の数
	constexpr int kMaxPauseNum = 2;
	//選んでいるステージを表示する座標(始点と終点まで)
	constexpr int kSelectSceneStartPosX = 480;
	constexpr int kSelectSceneStartPosY = 180;
	constexpr int kSelectSceneEndPosX = 1090;
	constexpr int kSelectSceneEndPosY = 845;
	//プレイヤーのグラフィックの大きさ
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	//プレイヤーを表示する座標
	constexpr int kPlayerPosX = Game::kScreenWidth / 2;
	constexpr int kPlayerPosY = Game::kPlayScreenHeight * 0.5f;
	//魔女を表示する座標
	constexpr int kPrincessPosX = Game::kScreenWidth / 2;
	constexpr int kPrincessPosY = Game::kPlayScreenHeight * 0.5f - 30;
	//背景画像の大きさ
	constexpr int kBgGraphSize = 640;
	//キャラクターの大きさ
	constexpr float kCharcterScale = 6.0f;
	//アニメーションの使う場所
	constexpr int kUseFrame[] = { 1,2,3,2 };
	//アニメーション１コマのフレーム数
	constexpr int kAnimFrameNum = 12;
	// アニメーションの１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//シーンの背景移動のスピード(小さいほうが早い)
	constexpr int kBgMoveSpeed = 128;
	//ショップのグラフィックの大きさ
	constexpr int kShopGraphSize = 64;
	//ショップのアニメーションの長さ
	constexpr int kShopAnimFrameNum = 48;
	//フォントの半分の大きさ
	constexpr int kFontHalfSize = 24;
	//タイトルの座標X
	constexpr int kTitlePosX = 390;
	//タイトルの座標Y
	constexpr int kTitlePosY = 115;
}
SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& DataManager,int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(selectSceneNum),
	m_isSelectKeyDown(false),
	m_isSelectScene(false),
	m_animFrame(kAnimFrameNum),
	m_dir(Game::kDirDown),
	m_cutBgPosY(kBgGraphSize* kMaxSceneNum - selectSceneNum * kBgGraphSize),
	m_isStaging(false),
	m_shopAnimFrame(0),
	m_shopSrcX(0),
	m_isChangeStage(false),
	m_isSceneUp(true)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
	m_bgm = DataManager.SearchSound("selectBgm");
	m_playerGraph = DataManager.SearchGraph("playerGraph");
	m_princessGraph = DataManager.SearchGraph("princessGraph");
	m_bgGraph = DataManager.SearchGraph("selectBgGraph");
	m_selectSceneBgGraph = DataManager.SearchGraph("allFieldBgGraph");
	m_shopGraph = DataManager.SearchGraph("shopGraph");
	m_selectSceneFrame = DataManager.SearchGraph("selectSceneFrameGraph");
	m_backBoxGraph = DataManager.SearchGraph("backBoxGraph");
	m_buttonsUiGraph = DataManager.SearchGraph("selectSceneButtonUiGraph");
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
}

void SceneSelect::End()
{
}

void SceneSelect::Update(Pad& pad)
{
	if (!CheckSoundMem(m_bgm) && !m_isSelectScene)
	{
		PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
	}
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//ボタンが連続で押されないための処理
	{
		//AボタンとBボタンが連続で押されないように
		if (!m_input.Buttons[XINPUT_BUTTON_A] && !m_input.Buttons[XINPUT_BUTTON_B] && //パッド
			!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_ESCAPE))//キーボード
		{
			m_isKeyDown = true;
		}
		//上キーと下キーが離されたら
		if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&//パッド
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_UP)&&!CheckHitKey(KEY_INPUT_DOWN) && !m_isStaging)
		{
			m_isSelectKeyDown = false;
		}
	}
	//ポーズが開かれていなく、演出中でない場合
	if (!m_isStaging)
	{
		//連続で押されないための処理
		if (m_isKeyDown)
		{
			if (!m_isSelectScene)
			{
				//Aボタンが押されたら
				if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
				{
					StopSoundMem(m_bgm);
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Bボタンを押したらタイトルに戻る
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					StopSoundMem(m_bgm);
					PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager,m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Yボタンが押されたら
				else if (m_input.Buttons[XINPUT_BUTTON_Y]  ||CheckHitKey(KEY_INPUT_P))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					StopSoundMem(m_bgm);
					//ショップシーンに移行する
					m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager,m_stageSelectNum));
					m_isSelectScene = true;
				}
			}
		}

		if (!m_isSelectKeyDown)
		{

			//上キーが押されたら
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))
			{
				if (m_stageSelectNum >= kMaxSceneNum)
				{
					m_stageSelectNum = kMaxSceneNum;
				}
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = true;
				}
				m_isSelectKeyDown = true;
			}
			//下キーが入力されたら
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))
			{
				if (m_stageSelectNum <= 0)
				{
					m_stageSelectNum = 0;
				}
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = false;
				}
				m_isSelectKeyDown = true;

			}
		}
	}





	if (m_isStaging)
	{
		MoveScene(m_isSceneUp);
	}
	if (m_cutBgPosY % kBgGraphSize == 0)
	{
		m_isStaging = false;
		m_dir = Game::kDirDown;
		m_animFrame = kAnimFrameNum;
		if (m_isChangeStage)
		{
			if (m_isSceneUp)
			{
				m_stageSelectNum++;
			}
			else
			{
				m_stageSelectNum--;
			}
			m_isChangeStage = false;

		}
	}
	//ショップのアニメーションを回し続ける
	m_shopAnimFrame++;
	if (m_shopAnimFrame > kShopAnimFrameNum)
	{
		m_shopSrcX = kShopGraphSize;
	}
	if (m_shopAnimFrame > kShopAnimFrameNum * 2)
	{
		m_shopAnimFrame = 0;
		m_shopSrcX = 0;
	}
}

void SceneSelect::Draw()
{
	//背景の下に今選んでいるステージを表示する
	DrawRectExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY,
		0, m_cutBgPosY, kBgGraphSize, kBgGraphSize, m_selectSceneBgGraph, true);
	//背景の表示
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//選んでいるシーンのフレーム
	DrawExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY - 50,
		m_selectSceneFrame, true);
	//ステージ名の表示
	DrawExtendGraph(70, 90, 730, 180, m_backBoxGraph, true);
	//プレイヤーと魔女を表示する
	int animEle = m_animFrame / kAnimFrameNum;
	//画像のどこを切り取るか計算
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;
	DrawFormatString(500, 500, GetColor(0, 0, 0), "%d", m_animFrame);
	if (m_isStaging && m_isSceneUp)
	{
		DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			kCharcterScale,
			0.0,
			m_playerGraph, true, false);
		DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
			0, 48,
			24, 24,
			5.0,
			0.0,
			m_princessGraph, true, false);
	}
	else
	{
		DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
			0, 24,
			24, 24,
			5.0,
			0.0,
			m_princessGraph, true, false);
		DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			kCharcterScale,
			0.0,
			m_playerGraph, true, false);
	}
	//選んでいるステージを表示する
	{
		int stringWidth;
		switch (m_stageSelectNum)
		{
		case 0:
			stringWidth = GetStringLength("冒険の始まり") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "冒険の始まり", GetColor(255, 255, 255));
			break;
		case 1:
			stringWidth = GetStringLength("よみがえれ姫") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "よみがえれ姫", GetColor(255, 255, 255));
			break;
		case 2:
			stringWidth = GetStringLength("怒りのパワー") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "怒りのパワー", GetColor(255, 255, 255));
			break;
		case 3:
			stringWidth = GetStringLength("イノシシ注意報") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "イノシシ注意報", GetColor(255, 255, 255));
			break;
		case 4:
			stringWidth = GetStringLength("アブナイ肝試し") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "アブナイ肝試し", GetColor(255, 255, 255));
			break;
		case 5:
			stringWidth = GetStringLength("ドラゴンの巣窟") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "ドラゴンの巣窟", GetColor(255, 255, 255));
			break;
		case 6:
			stringWidth = GetStringLength("雪に潜む者たち") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "雪に潜む者たち", GetColor(255, 255, 255));
			break;
		case 7:
			stringWidth = GetStringLength("俺たちの戦いはこれからだ") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "俺たちの戦いはこれからだ", GetColor(255, 255, 255));
			break;
		default:
			break;
		}
	}
	//ショップの表示
	DrawRectExtendGraph(1050, 350, 1550, 850, m_shopSrcX, 0, 64, 64, m_shopGraph, true);
	//左下に操作方法表示
	DrawGraph(100, 650,m_buttonsUiGraph, true);
}

void SceneSelect::MoveScene(bool up)
{
	float speed;
	speed = kBgGraphSize / kBgMoveSpeed;
	if (up)
	{
		m_cutBgPosY -= speed;
		m_animFrame++;
		if (m_animFrame > kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
		m_dir = Game::kDirUp;
	}
	else
	{
		m_cutBgPosY += speed;
		m_animFrame++;
		if (m_animFrame > kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
		m_dir = Game::kDirDown;
	}
}