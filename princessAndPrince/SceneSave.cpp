#include "SceneSave.h"
#include "DataManager.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "Game.h"
#include "UserData.h"
#include "MyString.h"
namespace
{
	//レベルを表示する座標
	constexpr int kMainStatusPos = 1120;
	constexpr int kMainLevelPos = 1260;
	//ステータスのレベルを表示する座標
	constexpr int kStatusPos = 950;
	constexpr int kStatusLevelPos = 1430;
	//影を表示するときにずらす座標
	constexpr int kShiftShadowLange = 3;
	//UIを表示する座標
	constexpr int kUiPosYArr[8] = { 170,270,350,430,510,590,670,750 };
	//フォントの大きさ(半角)
	constexpr int kFontSize = 24;
	//配列の大きさ
	constexpr int kArraySize = 81;
	//セーブデータのフレームを表示する座標
	constexpr int kFramePosX[2] = { 115,521 };
	constexpr int kFramePosY[2] = { 500,675 };
	//フレームの大きさ
	constexpr int kFrameWidth = 360;
	constexpr int kFrameHeight = 130;
	//フレームの拡縮の幅
	constexpr int kFrameRatioLangeX = 6;
	constexpr int kFrameRatioLangeY = 3;
	//フレームの拡縮の速さ
	constexpr float kFrameSpeedX = 0.6f;
	constexpr float kFrameSpeedY = 0.3f;
	//ステージの名前を表示する座標
	constexpr int kStageNamePosX[2] = { 480,620 };
	constexpr int kStageNamePosY[2] = { 485,730 };
	//データのナンバーを表示する座標
	constexpr int kSaveDataNumPosX[2] = { 205,610 };
	constexpr int kSaveDataNumPosY[2] = { 540,715 };
	//エネミーの頭を表示する間隔
	constexpr int kEnemyHeadDistance = 40;
	//エネミーの頭画像のグラフィックの大きさ
	constexpr int kEnemyHeadGraphSize = 30;
	//ウィンドウを開いた時の文字の座標
	constexpr int kWindowStringPosX = 480;
	constexpr int kWindowStringPosY[3] = { 180,270,360 };
	//ウィンドウを開いた時のカーソルの座標
	constexpr int kWindowCursorPosX[3] = { 265,262,290 };
	//カーソルの揺れる幅
	constexpr int kWindowCursorShakeLange = 5;
	//カーソルを揺らす速さ
	constexpr float kShakeSpeed = 0.5f;
	//警告を表示する座標
	constexpr int kDeleteStringPosX = 510;
	constexpr int kDeleteStringPosY[2] = { 230,320 };
	//デリートウィンドウを開いているときのカーソルのポジション
	constexpr int kDeleteCursorPosX[2] = { 275,465 };
	constexpr int kDeleteCursorPosY = 320;
}
SceneSave::SceneSave(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_bgGraph(0),
	m_frameGraph(0),
	m_isLastKey(true),
	m_selectSaveNum(0),
	m_isZoom(true),
	m_frameRatioX(0),
	m_frameRatioY(0),
	m_cursorSe(0),
	m_isMoveScene(false),
	m_firstLoadData(),
	m_secondLoadData(),
	m_thirdLoadData(),
	m_fourthLoadData(),
	m_isShowSaveWindow(false),
	m_selectDeleteNum(false),
	m_selectWindowNum(0),
	m_cursorShakeLange(0),
	m_isCursorMoveRight(true),
	m_isShowDeleteWindow(false),
	m_cancelSe(0)
{

	m_frameGraph = DataManager.SearchGraph("saveDataFrameGraph");
	m_bgGraph = DataManager.SearchGraph("sceneSaveGraph");
	m_enemyHeadGraph = DataManager.SearchGraph("enemyHeadGraph");
	m_cursorGraph = DataManager.SearchGraph("saveSceneCursorGraph");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_appSe = DataManager.SearchSound("approveSe");
	m_bgm = DataManager.SearchSound("saveSceneBgm");
	m_cancelSe = DataManager.SearchSound("cancelSe");
}

SceneSave::~SceneSave()
{
}

void SceneSave::Init()
{
	//セーブデータをロードする
	LoadFile();
	//音楽を鳴らす
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
}

void SceneSave::Update(Pad& pad)
{
	XINPUT_STATE input;
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	//ボタンが押せるタイミングだったら
	if (!m_isLastKey && !m_isMoveScene)
	{
		//ボタンが押されたらカーソルを動かす
		MoveCursor(input);
	}
	//すべてのボタンが離されたとき
	if (!CheckHitKeyAll() && !GetPushButton(input))
	{
		//ボタンを押せるようにする
		m_isLastKey = false;
	}

	//フレームを動かす
	ScalingFrame();


	if (m_cursorShakeLange > kWindowCursorShakeLange)
	{
		m_isCursorMoveRight = false;
	}
	else if (m_cursorShakeLange < -kWindowCursorShakeLange)
	{
		m_isCursorMoveRight = true;
	}

	if (m_isCursorMoveRight)
	{
		m_cursorShakeLange += kShakeSpeed;
	}
	else
	{
		m_cursorShakeLange -= kShakeSpeed;
	}

}

void SceneSave::Draw()
{
	//背景の表示
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);

	//ステータスを表示する
	DrawStatus();

	//データの番号を表示する
	DrawSaveDataNum();

	//セーブデータのフレームを表示する
	DrawFrame(m_selectSaveNum);

	//セーブデータが選ばれたとき
	if (m_isShowSaveWindow)
	{
		int stringWidth;
		//影を表示する
		stringWidth = GetStringLength("スタートする") * kFontSize;
		DrawString(kWindowStringPosX + kShiftShadowLange - stringWidth, kWindowStringPosY[0] + kShiftShadowLange, "スタートする", GetColor(0, 0, 0));
		stringWidth = GetStringLength("データをけす") * kFontSize;
		DrawString(kWindowStringPosX + kShiftShadowLange - stringWidth, kWindowStringPosY[1] + kShiftShadowLange, "データをけす", GetColor(0, 0, 0));
		stringWidth = GetStringLength("キャンセル") * kFontSize;
		DrawString(kWindowStringPosX + kShiftShadowLange - stringWidth, kWindowStringPosY[2] + kShiftShadowLange, "キャンセル", GetColor(0, 0, 0));
		//文字を表示する
		stringWidth = GetStringLength("スタートする") * kFontSize;
		DrawString(kWindowStringPosX - stringWidth, kWindowStringPosY[0], "スタートする", GetColor(255, 255, 255));
		stringWidth = GetStringLength("データをけす") * kFontSize;
		DrawString(kWindowStringPosX - stringWidth, kWindowStringPosY[1], "データをけす", GetColor(255, 255, 255));
		stringWidth = GetStringLength("キャンセル") * kFontSize;
		DrawString(kWindowStringPosX - stringWidth, kWindowStringPosY[2], "キャンセル", GetColor(255, 255, 255));

		DrawGraph(kWindowCursorPosX[m_selectWindowNum] + static_cast<int>(m_cursorShakeLange), kWindowStringPosY[m_selectWindowNum], m_cursorGraph, true);
	}
	if (m_isShowDeleteWindow)
	{
		//影を表示する
		int stringWidth = GetStringLength("本当ににデータをけしますか？") * kFontSize;
		DrawString(kDeleteStringPosX + kShiftShadowLange - stringWidth, kDeleteStringPosY[0] + kShiftShadowLange, "本当にデータをけしますか？", GetColor(0, 0, 0));
		stringWidth = GetStringLength("はい　　いいえ") * kFontSize;
		DrawString(kDeleteStringPosX + kShiftShadowLange - stringWidth, kDeleteStringPosY[1] + kShiftShadowLange, "はい　　いいえ", GetColor(0, 0, 0));
		//文字を表示する
		stringWidth = GetStringLength("本当ににデータをけしますか？") * kFontSize;
		DrawString(kDeleteStringPosX - stringWidth, kDeleteStringPosY[0] + kShiftShadowLange, "本当にデータをけしますか？", GetColor(255, 255, 255));
		stringWidth = GetStringLength("はい　　いいえ") * kFontSize;
		DrawString(kDeleteStringPosX - stringWidth, kDeleteStringPosY[1] + kShiftShadowLange, "はい　　いいえ", GetColor(255, 255, 255));

		DrawGraph(kDeleteCursorPosX[m_selectDeleteNum] + static_cast<int>(m_cursorShakeLange), kDeleteCursorPosY, m_cursorGraph, true);
	}

}

int SceneSave::GetDrawData(int selectNum, dataKind kind)
{
	//選択しているデータで返す値を変える
	switch (selectNum)
	{
	case 0:
		//メインレベル
		if (kind == dataKind::main)
		{
			return m_firstLoadData[static_cast<int>(dataKind::main)];
		}
		//こうげきレベル
		if (kind == dataKind::atk)
		{
			return m_firstLoadData[static_cast<int>(dataKind::atk)];
		}
		//防御レベル
		if (kind == dataKind::def)
		{
			return m_firstLoadData[static_cast<int>(dataKind::def)];
		}
		//いどうレベル
		if (kind == dataKind::spd)
		{
			return m_firstLoadData[static_cast<int>(dataKind::spd)];
		}
		//ファイアレベル
		if (kind == dataKind::fire)
		{
			return m_firstLoadData[static_cast<int>(dataKind::fire)];
		}
		//ウィンドレベル
		if (kind == dataKind::wind)
		{
			return m_firstLoadData[static_cast<int>(dataKind::wind)];
		}
		//所持ゴールド
		if (kind == dataKind::gold)
		{
			return m_firstLoadData[static_cast<int>(dataKind::gold)];
		}
		//総獲得経験値
		if (kind == dataKind::exp)
		{
			return m_firstLoadData[static_cast<int>(dataKind::exp)];
		}
		break;
	case 1:
		//メインレベル
		if (kind == dataKind::main)
		{
			return m_secondLoadData[static_cast<int>(dataKind::main)];
		}
		//こうげきレベル
		if (kind == dataKind::atk)
		{
			return m_secondLoadData[static_cast<int>(dataKind::atk)];
		}
		//防御レベル
		if (kind == dataKind::def)
		{
			return m_secondLoadData[static_cast<int>(dataKind::def)];
		}
		//いどうレベル
		if (kind == dataKind::spd)
		{
			return m_secondLoadData[static_cast<int>(dataKind::spd)];
		}
		//ファイアレベル
		if (kind == dataKind::fire)
		{
			return m_secondLoadData[static_cast<int>(dataKind::fire)];
		}
		//ウィンドレベル
		if (kind == dataKind::wind)
		{
			return m_secondLoadData[static_cast<int>(dataKind::wind)];
		}
		//所持ゴールド
		if (kind == dataKind::gold)
		{
			return m_secondLoadData[static_cast<int>(dataKind::gold)];
		}
		//総獲得経験値
		if (kind == dataKind::exp)
		{
			return m_secondLoadData[static_cast<int>(dataKind::exp)];
		}
		break;
	case 2:
		//メインレベル
		if (kind == dataKind::main)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::main)];
		}
		//こうげきレベル
		if (kind == dataKind::atk)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::atk)];
		}
		//防御レベル
		if (kind == dataKind::def)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::def)];
		}
		//いどうレベル
		if (kind == dataKind::spd)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::spd)];
		}
		//ファイアレベル
		if (kind == dataKind::fire)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::fire)];
		}
		//ウィンドレベル
		if (kind == dataKind::wind)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::wind)];
		}
		//所持ゴールド
		if (kind == dataKind::gold)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::gold)];
		}
		//総獲得経験値
		if (kind == dataKind::exp)
		{
			return m_thirdLoadData[static_cast<int>(dataKind::exp)];
		}
	case 3:
		//メインレベル
		if (kind == dataKind::main)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::main)];
		}
		//こうげきレベル
		if (kind == dataKind::atk)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::atk)];
		}
		//防御レベル
		if (kind == dataKind::def)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::def)];
		}
		//いどうレベル
		if (kind == dataKind::spd)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::spd)];
		}
		//ファイアレベル
		if (kind == dataKind::fire)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::fire)];
		}
		//ウィンドレベル
		if (kind == dataKind::wind)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::wind)];
		}
		//所持ゴールド
		if (kind == dataKind::gold)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::gold)];
		}
		//総獲得経験値
		if (kind == dataKind::exp)
		{
			return m_fourthLoadData[static_cast<int>(dataKind::exp)];
		}
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

int SceneSave::AlignmentRight(int num)
{
	if (num >= 1000000)
	{
		return kFontSize * 6;
	}
	else if (num >= 100000)
	{
		return kFontSize * 5;
	}
	else if (num >= 10000)
	{
		return kFontSize * 4;
	}
	else if (num >= 1000)
	{
		return kFontSize * 3;
	}
	else if (num >= 100)
	{
		return kFontSize * 2;
	}
	else if (num >= 10)
	{
		return kFontSize * 1;
	}
	else
	{
		return kFontSize * 0;
	}
	return 0;
}

void SceneSave::DrawFrame(int selectNum)
{
	switch (selectNum)
	{
	case 0:
		//拡縮するフレーム
		DrawExtendGraph(kFramePosX[0] - static_cast<int>(m_frameRatioX), kFramePosY[0] - static_cast<int>(m_frameRatioY),//始点
			kFramePosX[0] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[0] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[1], kFramePosY[0],//始点
			kFramePosX[1] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[0], kFramePosY[1],//始点
			kFramePosX[0] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[1], kFramePosY[1],//始点
			kFramePosX[1] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		break;
	case 1:
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[0], kFramePosY[0],//始点
			kFramePosX[0] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//拡縮するフレーム
		DrawExtendGraph(kFramePosX[1] - static_cast<int>(m_frameRatioX), kFramePosY[0] - static_cast<int>(m_frameRatioY),//始点
			kFramePosX[1] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[0] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[0], kFramePosY[1],//始点
			kFramePosX[0] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[1], kFramePosY[1],//始点
			kFramePosX[1] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		break;
	case 2:
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[0], kFramePosY[0],//始点
			kFramePosX[0] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[1], kFramePosY[0],//始点
			kFramePosX[1] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//拡縮するフレーム
		DrawExtendGraph(kFramePosX[0] - static_cast<int>(m_frameRatioX), kFramePosY[1] - static_cast<int>(m_frameRatioY),//始点
			kFramePosX[0] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[1] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[1], kFramePosY[1],//始点
			kFramePosX[1] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		break;
	case 3:
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[0], kFramePosY[0],//始点
			kFramePosX[0] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[1], kFramePosY[0],//始点
			kFramePosX[1] + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		//動かさないフレーム
		DrawExtendGraph(kFramePosX[0], kFramePosY[1],//始点
			kFramePosX[0] + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		//拡縮するフレーム
		DrawExtendGraph(kFramePosX[1] - static_cast<int>(m_frameRatioX), kFramePosY[1] - static_cast<int>(m_frameRatioY),//始点
			kFramePosX[1] + kFrameWidth + static_cast<int>(m_frameRatioX), kFramePosY[1] + kFrameHeight + static_cast<int>(m_frameRatioY), m_frameGraph, true);
		break;
	default:
		break;
	}
}

void SceneSave::SetSaveData(int selectNum)
{
	m_isMoveScene = true;
	StopSoundMem(m_bgm);
	switch (selectNum)
	{
	case 0:
		UserData::userMainLevel = m_firstLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_firstLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_firstLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_firstLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_firstLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_firstLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_firstLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_firstLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_firstLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_firstLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_firstLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	case 1:
		UserData::userMainLevel = m_secondLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_secondLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_secondLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_secondLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_secondLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_secondLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_secondLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_secondLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_secondLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_secondLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_secondLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	case 2:
		UserData::userMainLevel = m_thirdLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_thirdLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_thirdLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_thirdLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_thirdLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_thirdLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_thirdLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_thirdLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_thirdLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_thirdLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_thirdLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	case 3:
		UserData::userMainLevel = m_fourthLoadData[static_cast<int>(dataKind::main)];
		UserData::userAtkLevel = m_fourthLoadData[static_cast<int>(dataKind::atk)];
		UserData::userDefLevel = m_fourthLoadData[static_cast<int>(dataKind::def)];
		UserData::userSpdLevel = m_fourthLoadData[static_cast<int>(dataKind::spd)];
		UserData::userFireLevel = m_fourthLoadData[static_cast<int>(dataKind::fire)];
		UserData::userWindLevel = m_fourthLoadData[static_cast<int>(dataKind::wind)];
		UserData::userGold = m_fourthLoadData[static_cast<int>(dataKind::gold)];
		UserData::userExp = m_fourthLoadData[static_cast<int>(dataKind::exp)];
		UserData::userClearStageNum = m_fourthLoadData[static_cast<int>(dataKind::stage)];
		UserData::userSaveDataNum = m_fourthLoadData[static_cast<int>(dataKind::dataNum)];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_fourthLoadData[static_cast<int>(dataKind::stage)]), true);
		break;
	default:
		break;
	}
}

void SceneSave::DrawSaveDataNum()
{
	//影を表示
	DrawString(kSaveDataNumPosX[0] + kShiftShadowLange, kSaveDataNumPosY[0] + kShiftShadowLange, "データ1", GetColor(0, 0, 0));
	DrawString(kSaveDataNumPosX[1] + kShiftShadowLange, kSaveDataNumPosY[0] + kShiftShadowLange, "データ2", GetColor(0, 0, 0));
	DrawString(kSaveDataNumPosX[0] + kShiftShadowLange, kSaveDataNumPosY[1] + kShiftShadowLange, "データ3", GetColor(0, 0, 0));
	DrawString(kSaveDataNumPosX[1] + kShiftShadowLange, kSaveDataNumPosY[1] + kShiftShadowLange, "データ4", GetColor(0, 0, 0));
	//データの番号を表示
	DrawString(kSaveDataNumPosX[0], kSaveDataNumPosY[0], "データ1", GetColor(255, 255, 255));
	DrawString(kSaveDataNumPosX[1], kSaveDataNumPosY[0], "データ2", GetColor(255, 255, 255));
	DrawString(kSaveDataNumPosX[0], kSaveDataNumPosY[1], "データ3", GetColor(255, 255, 255));
	DrawString(kSaveDataNumPosX[1], kSaveDataNumPosY[1], "データ4", GetColor(255, 255, 255));
}

void SceneSave::DrawStatus()
{
	//影を表示
	{
		DrawString(kMainStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::main)] + kShiftShadowLange, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kMainLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::main) + 1), kUiPosYArr[0] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::main) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::atk)] + kShiftShadowLange, "こうげきLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::atk) + 1), kUiPosYArr[1] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::atk) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::def)] + kShiftShadowLange, "ぼうぎょLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::def) + 1), kUiPosYArr[2] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::def) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::spd)] + kShiftShadowLange, "は や さLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::spd) + 1), kUiPosYArr[3] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::spd) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::fire)] + kShiftShadowLange, "ファイアLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::fire) + 1), kUiPosYArr[4] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::fire) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::wind)] + kShiftShadowLange, "ウィンドLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::wind) + 1), kUiPosYArr[5] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::wind) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::gold)] + kShiftShadowLange, "所持ゴールド", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::gold)), kUiPosYArr[6] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::gold));
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[static_cast<int>(dataKind::exp)] + kShiftShadowLange, "総獲得経験値", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::exp)), kUiPosYArr[7] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, dataKind::exp));
	}
	//メインレベルを表示する
	DrawString(kMainStatusPos, kUiPosYArr[static_cast<int>(dataKind::main)], "Lv", GetColor(255, 255, 255));
	DrawFormatString(kMainLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::main) + 1), kUiPosYArr[0],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::main) + 1);
	//攻撃力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::atk)], "こうげきLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::atk) + 1), kUiPosYArr[1],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::atk) + 1);
	//防御力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::def)], "ぼうぎょLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::def) + 1), kUiPosYArr[2],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::def) + 1);
	//移動力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::spd)], "は や さLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::spd) + 1), kUiPosYArr[3],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::spd) + 1);
	//風魔法のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::fire)], "ファイアLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::fire) + 1), kUiPosYArr[4],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::fire) + 1);
	//炎魔法のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::wind)], "ウィンドLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::wind) + 1), kUiPosYArr[5],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::wind) + 1);
	//ゴールドの量
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::gold)], "所持ゴールド", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::gold)), kUiPosYArr[6],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::gold));
	//総獲得量の表示
	DrawString(kStatusPos, kUiPosYArr[static_cast<int>(dataKind::exp)], "総獲得経験値", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, dataKind::exp)), kUiPosYArr[7],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, dataKind::exp));
}

void SceneSave::LoadFile()
{
	{
		//ファイルを開く
		std::ifstream ifs("./data/save/firstSaveData.txt");
		int loopCount = 0;
		//配列を作成
		char str[kArraySize];
		//成功したら一行ずつ読み込む
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_firstLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	{
		//ファイルを開く
		std::ifstream ifs("./data/save/secondSaveData.txt");
		int loopCount = 0;
		//配列を作成
		char str[kArraySize];
		//成功したら一行ずつ読み込む
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_secondLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	{
		//ファイルを開く
		std::ifstream ifs("./data/save/thirdSaveData.txt");
		int loopCount = 0;
		//配列を作成
		char str[kArraySize];
		//成功したら一行ずつ読み込む
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_thirdLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	{
		//ファイルを開く
		std::ifstream ifs("./data/save/fourthSaveData.txt");
		int loopCount = 0;
		//配列を作成
		char str[kArraySize];
		//成功したら一行ずつ読み込む
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_fourthLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
}

bool SceneSave::GetPushButton(XINPUT_STATE& input)
{
	//十字キーが押されているかどうか
	if (input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && input.Buttons[XINPUT_BUTTON_DPAD_UP]
		&& input.Buttons[XINPUT_BUTTON_DPAD_RIGHT] && input.Buttons[XINPUT_BUTTON_DPAD_LEFT])
	{
		return true;
	}
	//Aボタンが押されているか
	else if (input.Buttons[XINPUT_BUTTON_A])
	{
		return true;
	}
	return false;
}

void SceneSave::ScalingFrame()
{
	//規定値よりも小さくなったら
	if (m_frameRatioX > kFrameRatioLangeX || m_frameRatioY > kFrameRatioLangeY)
	{
		//拡大を始める
		m_isZoom = true;
	}
	//規定値よりも大きくなったら
	else if (m_frameRatioX < -kFrameRatioLangeX || m_frameRatioY < -kFrameRatioLangeY)
	{
		//縮小を始める
		m_isZoom = false;
	}
	//拡大しているかどうか
	if (m_isZoom)
	{
		//フレームの拡大
		m_frameRatioX -= kFrameSpeedX;
		m_frameRatioY -= kFrameSpeedY;
	}
	else
	{
		//フレームの縮小
		m_frameRatioX += kFrameSpeedX;
		m_frameRatioY += kFrameSpeedY;
	}
}

void SceneSave::MoveCursor(XINPUT_STATE& input)
{
	if (CheckHitKey(KEY_INPUT_RETURN) || input.Buttons[XINPUT_BUTTON_A])
	{
		m_isLastKey = true;
		//デリートウィンドウが表示されているとき
		if (m_isShowDeleteWindow)
		{
			switch (m_selectDeleteNum)
			{
			case 0:
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				DeleteSaveData(m_selectSaveNum);
				m_isShowSaveWindow = false;
				m_isShowDeleteWindow = false;
				return;
				break;
			case 1:
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isShowDeleteWindow = false;
				m_isShowSaveWindow = true;
				return;
				break;
			default:
				break;
			}
		}

		//セーブウィンドウが表示されている時
		if (m_isShowSaveWindow)
		{
			switch (m_selectWindowNum)
			{
			case 0:
				SetSaveData(m_selectSaveNum);
				m_selectWindowNum = 0;
				break;
			case 1:
				m_selectWindowNum = 0;
				m_isShowSaveWindow = false;
				m_selectDeleteNum = 1;
				m_isShowDeleteWindow = true;
				break;
			case 2:
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isShowSaveWindow = false;
				m_selectWindowNum = 0;
				return;
				break;
			default:
				break;
			}
		}
		//デリートウィンドウが表示されていなかったら
		if (!m_isShowDeleteWindow)
		{
			m_isShowSaveWindow = true;
		}

		PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
	}
	//上ボタンが押されたら
	else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W) || input.Buttons[XINPUT_BUTTON_DPAD_UP])
	{
		//デリートウィンドウが開かれていないとき
		if (!m_isShowDeleteWindow)
		{
			//セーブウィンドウが開かれていないとき
			if (!m_isShowSaveWindow)
			{
				//下の行にある場合
				if (m_selectSaveNum > 1)
				{
					//上の行に動かす
					m_selectSaveNum -= 2;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				//動かせる場所にカーソルがあったら
				if (m_selectWindowNum > 0)
				{
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_selectWindowNum--;
				}
			}
		}
		m_isLastKey = true;
	}
	//下ボタンが押されたら
	else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S) || input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
	{
		//デリートウィンドウが表示されていないとき
		if (!m_isShowDeleteWindow)
		{
			//セーブウィンドウが開かれていないとき
			if (!m_isShowSaveWindow)
			{

				//上の行にある場合
				if (m_selectSaveNum < 2)
				{
					//下の行に動かす
					m_selectSaveNum += 2;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				//動かせる場所にカーソルがあったら
				if (m_selectWindowNum < 2)
				{
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_selectWindowNum++;
				}
			}
		}
		m_isLastKey = true;
	}
	//右ボタンが押されたら
	else if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D) || input.Buttons[XINPUT_BUTTON_DPAD_RIGHT])
	{
		//ウィンドウが開かれていなかったら
		if (!m_isShowDeleteWindow && !m_isShowSaveWindow)
		{
			//左列にいたら
			if (m_selectSaveNum == 0 || m_selectSaveNum == 2)
			{
				m_selectSaveNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
		}
		if (m_isShowDeleteWindow)
		{
			if (m_selectDeleteNum == 0)
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_selectDeleteNum++;
			}
		}
		m_isLastKey = true;
	}
	//左ボタンが押されたら
	else if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A) || input.Buttons[XINPUT_BUTTON_DPAD_LEFT])
	{
		//ウィンドウが開かれていなかったら
		if (!m_isShowDeleteWindow && !m_isShowSaveWindow)
		{
			//右列にいたら
			if (m_selectSaveNum == 1 || m_selectSaveNum == 3)
			{
				m_selectSaveNum--;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
		}
		if (m_isShowDeleteWindow)
		{
			if (m_selectDeleteNum == 1)
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_selectDeleteNum--;
			}
		}
		m_isLastKey = true;
	}
	else if (CheckHitKey(KEY_INPUT_ESCAPE) || input.Buttons[XINPUT_BUTTON_B])
	{
		PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
		if (m_isShowSaveWindow)
		{
			m_isShowSaveWindow = false;
		}
		else
		{
			StopSoundMem(m_bgm);
			m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager, 0), true);
		}
		m_isLastKey = true;
	}
}

void SceneSave::DeleteSaveData(int dataNum)
{
	if (dataNum == 0)
	{
		std::string tempS;
		ofstream outputfile("./data/save/firstSaveData.txt");
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();

		//ファイルを開く
		std::ifstream ifs("./data/save/firstSaveData.txt");
		int loopCount = 0;
		//配列を作成
		char str[kArraySize];
		//成功したら一行ずつ読み込む
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_firstLoadData[loopCount] = temp;
			loopCount++;
		}
		ifs.close();

	}
	else if (dataNum == 1)
	{
		std::string tempS;
		ofstream outputfile("./data/save/secondSaveData.txt");

		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();
		{
			//ファイルを開く
			std::ifstream ifs("./data/save/secondSaveData.txt");
			int loopCount = 0;
			//配列を作成
			char str[kArraySize];
			//成功したら一行ずつ読み込む
			while (ifs.getline(str, kArraySize))
			{
				int temp;
				temp = std::stoi(str);
				m_secondLoadData[loopCount] = temp;
				loopCount++;
			}
			ifs.close();
		}
	}
	else if (dataNum == 2)
	{
		std::string tempS;
		ofstream outputfile("./data/save/thirdSaveData.txt");
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();
		{
			//ファイルを開く
			std::ifstream ifs("./data/save/thirdSaveData.txt");
			int loopCount = 0;
			//配列を作成
			char str[kArraySize];
			//成功したら一行ずつ読み込む
			while (ifs.getline(str, kArraySize))
			{
				int temp;
				temp = std::stoi(str);
				m_thirdLoadData[loopCount] = temp;
				loopCount++;
			}
			ifs.close();
		}
	}
	else if (dataNum == 3)
	{
		std::string tempS;
		ofstream outputfile("./data/save/fourthSaveData.txt");
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		outputfile << "0\n";
		tempS = to_string(dataNum + 1);
		outputfile << tempS;
		outputfile.close();
		{
			//ファイルを開く
			std::ifstream ifs("./data/save/fourthSaveData.txt");
			int loopCount = 0;
			//配列を作成
			char str[kArraySize];
			//成功したら一行ずつ読み込む
			while (ifs.getline(str, kArraySize))
			{
				int temp;
				temp = std::stoi(str);
				m_fourthLoadData[loopCount] = temp;
				loopCount++;
			}
			ifs.close();
		}
	}
	UserData::userMainLevel = 0;
	UserData::userAtkLevel = 0;
	UserData::userDefLevel = 0;
	UserData::userSpdLevel = 0;
	UserData::userFireLevel = 0;
	UserData::userWindLevel = 0;
	UserData::userGold = 0;
	UserData::userExp = 0;
	UserData::userClearStageNum = 0;
	UserData::userSaveDataNum = dataNum;

}

