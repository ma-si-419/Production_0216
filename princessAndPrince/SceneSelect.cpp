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
	constexpr int kPlayerPosY = 590;
	//魔女を表示する座標
	constexpr int kPrincessPosX = Game::kScreenWidth / 2;
	constexpr int kPrincessPosY = 700;
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
	constexpr int kBgMoveSpeed = 64;
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
	//上の三角形のY座標
	constexpr int kHighTrianglePosY = 125;
	//下の三角形のY座標
	constexpr int kLowTrianglePosY = 850;
	//三角形の大きさ
	constexpr int kTriangleScale = 60;
	//三角形のフレームの大きさ
	constexpr int kTriangleFrameScale = 80;
	//三角形のフレームのY座標
	constexpr int kTriangleFrameShiftPosY = 12;
	//ショップの基本的な座標
	constexpr int kshopStartPosX = 1100;
	constexpr int kshopStartPosY = 300;
	constexpr int kshopEndPosX = 1620;
	constexpr int kshopEndPosY = 870;
	//ショップが大きくなっていくスピード
	constexpr int kShopStartPosXSpeed = 28;
	constexpr int kShopStartPosYSpeed = 50;
	constexpr int kShopEndPosXSpeed = 20;
	constexpr int kShopEndPosYSpeed = 5;
	//ショップが最終的に大きくなる座標
	constexpr int kShopMaxSizePosX = 500;}
SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
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
	m_shopSrcY(0),
	m_isChangeStage(false),
	m_isSceneUp(true),
	m_isShopButton(false),
	m_shopStartPosX(kshopStartPosX),
	m_shopStartPosY(kshopStartPosY),
	m_shopEndPosX(kshopEndPosX),
	m_shopEndPosY(kshopEndPosY),
	m_isMoveMainScene(false)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_moveMainSceneSe = DataManager.SearchSound("moveMainSceneSe");
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
	m_storyGraph[0] = DataManager.SearchGraph("storyGraph1");
	m_storyGraph[1] = DataManager.SearchGraph("storyGraph2");
	m_storyGraph[2] = DataManager.SearchGraph("storyGraph3");
	m_storyGraph[3] = DataManager.SearchGraph("storyGraph4");
	m_storyGraph[4] = DataManager.SearchGraph("storyGraph5");
	m_storyGraph[5] = DataManager.SearchGraph("storyGraph6");
	m_storyGraph[6] = DataManager.SearchGraph("storyGraph7");
	m_storyGraph[7] = DataManager.SearchGraph("storyGraph8");

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
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN) && !m_isStaging)
		{
			m_isSelectKeyDown = false;
		}
	}
	//ポーズが開かれていなく、演出中でない場合
	if (!m_isStaging && !m_isShopButton && !m_isMoveMainScene)
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
					//別の音に変える
					PlaySoundMem(m_moveMainSceneSe, DX_PLAYTYPE_BACK);
					m_animFrame = 24;
					m_dir = Game::kDirDeath;
					m_isMoveMainScene = true;
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Bボタンを押したらタイトルに戻る
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					StopSoundMem(m_bgm);
					PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager, m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Yボタンが押されたら
				else if (m_input.Buttons[XINPUT_BUTTON_Y] || CheckHitKey(KEY_INPUT_P))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					//ショップに入る演出を入れる
					m_shopSrcY = 64;
					m_isShopButton = true;
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
	if (m_cutBgPosY % kBgGraphSize == 0 && !m_isMoveMainScene)
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
	if (m_isShopButton)
	{
		m_shopStartPosX -= kShopStartPosXSpeed;
		m_shopStartPosY -= kShopStartPosYSpeed;
		m_shopEndPosX += kShopEndPosXSpeed;
		m_shopEndPosY += kShopEndPosYSpeed;

		if (m_shopStartPosX < kShopMaxSizePosX)
		{
			StopSoundMem(m_bgm);
			//ショップシーンに移行する
			m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager, m_stageSelectNum));
		}
	}
	if (m_isMoveMainScene)
	{
		//音が鳴りやんだらフェードしていく
		if (!CheckSoundMem(m_moveMainSceneSe))
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum));
		}
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
	//三角形の表示
	DrawSceneSrideTriangle();
	//ステージ名の表示
	DrawExtendGraph(70, 90, 730, 180, m_backBoxGraph, true);
	//プレイヤーと魔女を表示する
	int animEle = m_animFrame / kAnimFrameNum;
	//画像のどこを切り取るか計算
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;
	DrawFormatString(500, 500, GetColor(0, 0, 0), "%d", m_animFrame);

	//サルと姫の描画
		DrawRectRotaGraph(kPlayerPosX, kPlayerPosY,
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			kCharcterScale,
			0.0,
			m_playerGraph, true, false);
		DrawRectRotaGraph(kPrincessPosX, kPrincessPosY,
			0, 24,
			24, 24,
			5.0,
			0.0,
			m_princessGraph, true, false);
	//左上にストーリー表示
	DrawGraph(130, 200, m_storyGraph[m_stageSelectNum], true);
	//左下に操作方法表示
	DrawGraph(130, 650, m_buttonsUiGraph, true);
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
			stringWidth = GetStringLength("賢者を訪ねて") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "賢者を訪ねて", GetColor(255, 255, 255));
			break;
		case 2:
			stringWidth = GetStringLength("よみがえれ姫") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "よみがえれ姫", GetColor(255, 255, 255));
			break;
		case 3:
			stringWidth = GetStringLength("怒りのパワー") * kFontHalfSize;
			DrawString(kTitlePosX - stringWidth, kTitlePosY, "怒りのパワー", GetColor(255, 255, 255));
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
	DrawRectExtendGraph(static_cast<int>(m_shopStartPosX), static_cast<int>(m_shopStartPosY),
		m_shopEndPosX, m_shopEndPosY, m_shopSrcX, m_shopSrcY, 64, 64, m_shopGraph, true);
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

void SceneSelect::DrawSceneSrideTriangle()
{
	if (m_stageSelectNum != kMaxSceneNum)
	{
		//上の三角形表示
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleFrameScale, kHighTrianglePosY + kTriangleFrameScale,
			Game::kScreenWidth / 2, kHighTrianglePosY,
			Game::kScreenWidth / 2 - kTriangleFrameScale, kHighTrianglePosY + kTriangleFrameScale,
			GetColor(0, 0, 0), true);
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleScale, kHighTrianglePosY + kTriangleScale + kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2, kHighTrianglePosY + kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2 - kTriangleScale, kHighTrianglePosY + kTriangleScale + kTriangleFrameShiftPosY,
			GetColor(255, 255, 255), true);
	}

	if (m_stageSelectNum != 0)
	{
		//下の三角形表示
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleFrameScale, kLowTrianglePosY - kTriangleFrameScale,
			Game::kScreenWidth / 2, kLowTrianglePosY,
			Game::kScreenWidth / 2 - kTriangleFrameScale, kLowTrianglePosY - kTriangleFrameScale,
			GetColor(0, 0, 0), true);
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleScale, kLowTrianglePosY - kTriangleScale - kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2, kLowTrianglePosY - kTriangleFrameShiftPosY,
			Game::kScreenWidth / 2 - kTriangleScale, kLowTrianglePosY - kTriangleScale - kTriangleFrameShiftPosY,
			GetColor(255, 255, 255), true);
	}

}

