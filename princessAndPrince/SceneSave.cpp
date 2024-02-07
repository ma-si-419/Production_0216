#include "SceneSave.h"
#include "DataManager.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "DxLib.h"
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
	constexpr int kFramePosX = 115;
	constexpr int kFramePosY[3] = { 125,370,615 };
	//フレームの縦の大きさ
	constexpr int kFrameWidth = 735;
	constexpr int kFrameHeight = 225;
	//フレームの拡縮の幅
	constexpr int kFrameRatioLangeX = 6;
	constexpr int kFrameRatioLangeY = 3;
	//フレームの拡縮の速さ
	constexpr float kFrameSpeedX = 0.6f;
	constexpr float kFrameSpeedY = 0.3f;
	//ステージの名前を表示する座標
	constexpr int kStageNamePosX = 480;
	constexpr int kStageNamePosY[3] = { 240,485,730 };
	//データのナンバーを表示する座標
	constexpr int kSaveDataNumPosX = 160;
	constexpr int kSaveDataNumPosY[3] = { 180,420,660 };
	//エネミーの頭を表示する間隔
	constexpr int kEnemyHeadDistance = 40;
	//エネミーの頭画像のグラフィックの大きさ
	constexpr int kEnemyHeadGraphSize = 30;

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
	m_cursorSe(0)
{
	m_frameGraph = DataManager.SearchGraph("saveDataFrameGraph");
	m_bgGraph = DataManager.SearchGraph("sceneSaveGraph");
	m_enemyHeadGraph = DataManager.SearchGraph("enemyHeadGraph");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_appSe = DataManager.SearchSound("approveSe");
}

SceneSave::~SceneSave()
{
}

void SceneSave::Init()
{
	{
		//ファイルを開く
		std::ifstream ifs("./data/firstSaveData.txt");
		int loopCount = 0;
		//帰ってきた値を返す配列
		vector<string> tempS;
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
		std::ifstream ifs("./data/secondSaveData.txt");
		int loopCount = 0;
		//帰ってきた値を返す配列
		vector<string> tempS;
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
		std::ifstream ifs("./data/thirdSaveData.txt");
		int loopCount = 0;
		//帰ってきた値を返す配列
		vector<string> tempS;
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

void SceneSave::Update(Pad& pad)
{
	XINPUT_STATE input;
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	if (!m_isLastKey)
	{
		if (CheckHitKey(KEY_INPUT_RETURN) || input.Buttons[XINPUT_BUTTON_A])
		{
			SetSaveData(m_selectSaveNum);
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
			m_isLastKey = true;
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W) || input.Buttons[XINPUT_BUTTON_DPAD_UP])
		{
			if (m_selectSaveNum > 0)
			{
				m_selectSaveNum--;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
			m_isLastKey = true;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S) || input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
		{
			if (m_selectSaveNum < 2)
			{
				m_selectSaveNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
			}
			m_isLastKey = true;
		}
	}
	if (!CheckHitKeyAll() && !input.Buttons[XINPUT_BUTTON_A] &&
		!input.Buttons[XINPUT_BUTTON_DPAD_UP] && !input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
	{
		m_isLastKey = false;
	}
	if (m_frameRatioX > kFrameRatioLangeX || m_frameRatioY > kFrameRatioLangeY)
	{

		m_isZoom = true;
	}
	else if (m_frameRatioX < -kFrameRatioLangeX || m_frameRatioY < -kFrameRatioLangeY)
	{

		m_isZoom = false;
	}
	if (m_isZoom)
	{
		m_frameRatioX -= kFrameSpeedX;
		m_frameRatioY -= kFrameSpeedY;
	}
	else
	{
		m_frameRatioX += kFrameSpeedX;
		m_frameRatioY += kFrameSpeedY;
	}


}

void SceneSave::Draw()
{
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//影を表示
	{
		DrawString(kMainStatusPos + kShiftShadowLange, kUiPosYArr[0] + kShiftShadowLange, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kMainLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, main) + 1), kUiPosYArr[0] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, main) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[1] + kShiftShadowLange, "こうげきLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, atk) + 1), kUiPosYArr[1] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, atk) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[2] + kShiftShadowLange, "ぼうぎょLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, def) + 1), kUiPosYArr[2] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, def) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[3] + kShiftShadowLange, "は や さLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, spd) + 1), kUiPosYArr[3] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, spd) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[4] + kShiftShadowLange, "ファイアLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, fire) + 1), kUiPosYArr[4] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, fire) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[5] + kShiftShadowLange, "ウィンドLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, wind) + 1), kUiPosYArr[5] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, wind) + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[6] + kShiftShadowLange, "所持ゴールド", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, gold)), kUiPosYArr[6] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, gold));
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[7] + kShiftShadowLange, "総獲得経験値", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(GetDrawData(m_selectSaveNum, exp)), kUiPosYArr[7] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", GetDrawData(m_selectSaveNum, exp));
	}
	//メインレベルを表示する
	DrawString(kMainStatusPos, kUiPosYArr[0], "Lv", GetColor(255, 255, 255));
	DrawFormatString(kMainLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, main) + 1), kUiPosYArr[0],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, main) + 1);
	//攻撃力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[1], "こうげきLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, atk) + 1), kUiPosYArr[1],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, atk) + 1);
	//防御力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[2], "ぼうぎょLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, def) + 1), kUiPosYArr[2],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, def) + 1);
	//移動力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[3], "は や さLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, spd) + 1), kUiPosYArr[3],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, spd) + 1);
	//風魔法のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[4], "ファイアLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, fire) + 1), kUiPosYArr[4],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, fire) + 1);
	//炎魔法のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[5], "ウィンドLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, wind) + 1), kUiPosYArr[5],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, wind) + 1);
	//ゴールドの量
	DrawString(kStatusPos, kUiPosYArr[6], "所持ゴールド", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, gold)), kUiPosYArr[6],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, gold));
	//総獲得量の表示
	DrawString(kStatusPos, kUiPosYArr[7], "総獲得経験値", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(GetDrawData(m_selectSaveNum, exp)), kUiPosYArr[7],//座標
		GetColor(255, 255, 255), "%d", GetDrawData(m_selectSaveNum, exp));

	//データの番号を表示する
	DrawSaveDataNum();
	//今どのシーンにいるかをを表示する
	DrawSceneName(m_firstLoadData[static_cast<int>(stage)], kStageNamePosX, kStageNamePosY[0]);
	DrawSceneName(m_secondLoadData[static_cast<int>(stage)], kStageNamePosX, kStageNamePosY[1]);
	DrawSceneName(m_thirdLoadData[static_cast<int>(stage)], kStageNamePosX, kStageNamePosY[2]);
	//倒したボスの顔を表示する
	DrawDefeatedBoss(0, 600, 200);
	DrawDefeatedBoss(2, 470, 445);
	DrawDefeatedBoss(7, 600, 690);

	//セーブデータのフレームを表示する
	DrawFrame(m_selectSaveNum);
#ifdef _DEBUG
	DrawFormatString(100, 100, GetColor(0, 0, 0), "%d", m_selectSaveNum);
#endif // DEBUG

}

void SceneSave::DrawSceneName(int sceneNum, int x, int y)
{
	int stringWidth;
	////影の表示
	//DrawFormatString(x + kShiftShadowLange, y + kShiftShadowLange, GetColor(0, 0, 0), "ステージ %d", sceneNum + 1);
	////ステージ名の表示
	//DrawFormatString(x,y,GetColor(255,255,255),"ステージ %d",sceneNum + 1);
	{

		switch (sceneNum)
		{
		case 0:
			stringWidth = GetStringLength("冒険の始まり") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "冒険の始まり", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "冒険の始まり", GetColor(255, 255, 255));
			break;
		case 1:
			stringWidth = GetStringLength("賢者を訪ねて") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "賢者を訪ねて", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "賢者を訪ねて", GetColor(255, 255, 255));
			break;
		case 2:
			stringWidth = GetStringLength("よみがえれ姫") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "よみがえれ姫", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "よみがえれ姫", GetColor(255, 255, 255));
			break;
		case 3:
			stringWidth = GetStringLength("怒りのパワー") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "怒りのパワー", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "怒りのパワー", GetColor(255, 255, 255));
			break;
		case 4:
			stringWidth = GetStringLength("アブナイ肝試し") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "アブナイ肝試し", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "アブナイ肝試し", GetColor(255, 255, 255));
			break;
		case 5:
			stringWidth = GetStringLength("ドラゴンの巣窟") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "ドラゴンの巣窟", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "ドラゴンの巣窟", GetColor(255, 255, 255));
			break;
		case 6:
			stringWidth = GetStringLength("雪に潜む者たち") * kFontSize;
			//ステージ名の表示
			DrawString(x - stringWidth, y, "雪に潜む者たち", GetColor(255, 255, 255));
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "雪に潜む者たち", GetColor(0, 0, 0));
			break;
		case 7:
			stringWidth = GetStringLength("俺たちの戦いはこれからだ") * kFontSize;
			//影の表示
			DrawString(x - stringWidth + kShiftShadowLange, y + kShiftShadowLange, "俺たちの戦いはこれからだ", GetColor(0, 0, 0));
			//ステージ名の表示
			DrawString(x - stringWidth, y, "俺たちの戦いはこれからだ", GetColor(255, 255, 255));
			break;
		}
	}
}
int SceneSave::GetDrawData(int selectNum, dataKind kind)
{
	//選択しているデータで返す値を変える
	switch (selectNum)
	{
	case 0:
		//メインレベル
		if (static_cast<int>(kind) == 0)
		{
			return m_firstLoadData[0];
		}
		//こうげきレベル
		if (static_cast<int>(kind) == 1)
		{
			return m_firstLoadData[1];
		}
		//防御レベル
		if (static_cast<int>(kind) == 2)
		{
			return m_firstLoadData[2];
		}
		//いどうレベル
		if (static_cast<int>(kind) == 3)
		{
			return m_firstLoadData[3];
		}
		//ファイアレベル
		if (static_cast<int>(kind) == 4)
		{
			return m_firstLoadData[4];
		}
		//ウィンドレベル
		if (static_cast<int>(kind) == 5)
		{
			return m_firstLoadData[5];
		}
		//所持ゴールド
		if (static_cast<int>(kind) == 6)
		{
			return m_firstLoadData[6];
		}
		//総獲得経験値
		if (static_cast<int>(kind) == 7)
		{
			return m_firstLoadData[7];
		}
		break;
	case 1:
		//メインレベル
		if (static_cast<int>(kind) == 0)
		{
			return m_secondLoadData[0];
		}
		//こうげきレベル
		if (static_cast<int>(kind) == 1)
		{
			return m_secondLoadData[1];
		}
		//防御レベル
		if (static_cast<int>(kind) == 2)
		{
			return m_secondLoadData[2];
		}
		//いどうレベル
		if (static_cast<int>(kind) == 3)
		{
			return m_secondLoadData[3];
		}
		//ファイアレベル
		if (static_cast<int>(kind) == 4)
		{
			return m_secondLoadData[4];
		}
		//ウィンドレベル
		if (static_cast<int>(kind) == 5)
		{
			return m_secondLoadData[5];
		}
		//所持ゴールド
		if (static_cast<int>(kind) == 6)
		{
			return m_secondLoadData[6];
		}
		//総獲得経験値
		if (static_cast<int>(kind) == 7)
		{
			return m_secondLoadData[7];
		}
		break;
	case 2:
		//メインレベル
		if (static_cast<int>(kind) == 0)
		{
			return m_thirdLoadData[0];
		}
		//こうげきレベル
		if (static_cast<int>(kind) == 1)
		{
			return m_thirdLoadData[1];
		}
		//防御レベル
		if (static_cast<int>(kind) == 2)
		{
			return m_thirdLoadData[2];
		}
		//いどうレベル
		if (static_cast<int>(kind) == 3)
		{
			return m_thirdLoadData[3];
		}
		//ファイアレベル
		if (static_cast<int>(kind) == 4)
		{
			return m_thirdLoadData[4];
		}
		//ウィンドレベル
		if (static_cast<int>(kind) == 5)
		{
			return m_thirdLoadData[5];
		}
		//所持ゴールド
		if (static_cast<int>(kind) == 6)
		{
			return m_thirdLoadData[6];
		}
		//総獲得経験値
		if (static_cast<int>(kind) == 7)
		{
			return m_thirdLoadData[7];
		}
		break;
	}
}
int SceneSave::AlignmentRight(int num)
{
	if (num >= 1000000)
	{
		return kFontSize * 7;
	}
	else if (num >= 100000)
	{
		return kFontSize * 6;
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
		DrawExtendGraph(kFramePosX - m_frameRatioX, kFramePosY[0] - m_frameRatioY,//始点
			kFramePosX + kFrameWidth + m_frameRatioX, kFramePosY[0] + kFrameHeight + m_frameRatioY, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[1],//始点
			kFramePosX + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[2],//始点
			kFramePosX + kFrameWidth, kFramePosY[2] + kFrameHeight, m_frameGraph, true);		break;
	case 1:
		DrawExtendGraph(kFramePosX, kFramePosY[0],//始点
			kFramePosX + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX - m_frameRatioX, kFramePosY[1] - m_frameRatioY,//始点
			kFramePosX + kFrameWidth + m_frameRatioX, kFramePosY[1] + kFrameHeight + m_frameRatioY, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[2],//始点
			kFramePosX + kFrameWidth, kFramePosY[2] + kFrameHeight, m_frameGraph, true);		break;
	case 2:
		DrawExtendGraph(kFramePosX, kFramePosY[0],//始点
			kFramePosX + kFrameWidth, kFramePosY[0] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX, kFramePosY[1],//始点
			kFramePosX + kFrameWidth, kFramePosY[1] + kFrameHeight, m_frameGraph, true);
		DrawExtendGraph(kFramePosX - m_frameRatioX, kFramePosY[2] - m_frameRatioY,//始点
			kFramePosX + kFrameWidth + m_frameRatioX, kFramePosY[2] + kFrameHeight + m_frameRatioY, m_frameGraph, true);
		break;
	default:
		break;
	}
}

void SceneSave::SetSaveData(int selectNum)
{
	switch (selectNum)
	{
	case 0:
		UserData::userMainLevel = m_firstLoadData[static_cast<int>(main)];
		UserData::userAtkLevel = m_firstLoadData[static_cast<int>(atk)];
		UserData::userDefLevel = m_firstLoadData[static_cast<int>(def)];
		UserData::userSpdLevel = m_firstLoadData[static_cast<int>(spd)];
		UserData::userFireLevel = m_firstLoadData[static_cast<int>(fire)];
		UserData::userWindLevel = m_firstLoadData[static_cast<int>(wind)];
		UserData::userGold = m_firstLoadData[static_cast<int>(gold)];
		UserData::userExp = m_firstLoadData[static_cast<int>(exp)];
		UserData::userClearStageNum = m_firstLoadData[stage];
		UserData::userSaveDataNum = m_firstLoadData[dataNum];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_firstLoadData[static_cast<int>(stage)]), true);
		break;
	case 1:
		UserData::userMainLevel = m_secondLoadData[static_cast<int>(main)];
		UserData::userAtkLevel = m_secondLoadData[static_cast<int>(atk)];
		UserData::userDefLevel = m_secondLoadData[static_cast<int>(def)];
		UserData::userSpdLevel = m_secondLoadData[static_cast<int>(spd)];
		UserData::userFireLevel = m_secondLoadData[static_cast<int>(fire)];
		UserData::userWindLevel = m_secondLoadData[static_cast<int>(wind)];
		UserData::userGold = m_secondLoadData[static_cast<int>(gold)];
		UserData::userExp = m_secondLoadData[static_cast<int>(exp)];
		UserData::userClearStageNum = m_secondLoadData[stage];
		UserData::userSaveDataNum = m_secondLoadData[dataNum];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_secondLoadData[static_cast<int>(stage)]), true);
		break;
	case 2:
		UserData::userMainLevel = m_thirdLoadData[static_cast<int>(main)];
		UserData::userAtkLevel = m_thirdLoadData[static_cast<int>(atk)];
		UserData::userDefLevel = m_thirdLoadData[static_cast<int>(def)];
		UserData::userSpdLevel = m_thirdLoadData[static_cast<int>(spd)];
		UserData::userFireLevel = m_thirdLoadData[static_cast<int>(fire)];
		UserData::userWindLevel = m_thirdLoadData[static_cast<int>(wind)];
		UserData::userGold = m_thirdLoadData[static_cast<int>(gold)];
		UserData::userExp = m_thirdLoadData[static_cast<int>(exp)];
		UserData::userClearStageNum = m_thirdLoadData[stage];
		UserData::userSaveDataNum = m_thirdLoadData[dataNum];
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager,
			m_thirdLoadData[static_cast<int>(stage)]), true);
		break;
	default:
		break;
	}
}

void SceneSave::DrawSaveDataNum()
{
	////影を表示
	//DrawString(kSaveDataNumPosX + kShiftShadowLange, kSaveDataNumPosY[0] + kShiftShadowLange, "データ1", GetColor(0, 0, 0));
	//DrawString(kSaveDataNumPosX + kShiftShadowLange, kSaveDataNumPosY[1] + kShiftShadowLange, "データ2", GetColor(0, 0, 0));
	//DrawString(kSaveDataNumPosX + kShiftShadowLange, kSaveDataNumPosY[2] + kShiftShadowLange, "データ3", GetColor(0, 0, 0));
	////データの番号を表示
	//DrawString(kSaveDataNumPosX, kSaveDataNumPosY[0], "データ1", GetColor(255, 255, 255));
	//DrawString(kSaveDataNumPosX, kSaveDataNumPosY[1], "データ2", GetColor(255, 255, 255));
	//DrawString(kSaveDataNumPosX, kSaveDataNumPosY[2], "データ3", GetColor(255, 255, 255));
	//影を表示
	DrawFormatString(kSaveDataNumPosX + kShiftShadowLange, kSaveDataNumPosY[0] + kShiftShadowLange, GetColor(0, 0, 0), "ステージ%d", m_firstLoadData[static_cast<int>(stage)] + 1);
	DrawFormatString(kSaveDataNumPosX + kShiftShadowLange, kSaveDataNumPosY[1] + kShiftShadowLange, GetColor(0, 0, 0), "ステージ%d", m_secondLoadData[static_cast<int>(stage)] + 1);
	DrawFormatString(kSaveDataNumPosX + kShiftShadowLange, kSaveDataNumPosY[2] + kShiftShadowLange, GetColor(0, 0, 0), "ステージ%d", m_thirdLoadData[static_cast<int>(stage)] + 1);
	//ステージ名の表示
	DrawFormatString(kSaveDataNumPosX, kSaveDataNumPosY[0], GetColor(255, 255, 255), "ステージ%d", m_firstLoadData[static_cast<int>(stage)] + 1);
	DrawFormatString(kSaveDataNumPosX, kSaveDataNumPosY[1], GetColor(255, 255, 255), "ステージ%d", m_secondLoadData[static_cast<int>(stage)] + 1);
	DrawFormatString(kSaveDataNumPosX, kSaveDataNumPosY[2], GetColor(255, 255, 255), "ステージ%d", m_thirdLoadData[static_cast<int>(stage)] + 1);
}

void SceneSave::DrawDefeatedBoss(int clearStageNum,int x ,int y)
{
	DrawRectRotaGraph(x , y, 0, 0, kEnemyHeadGraphSize * clearStageNum, kEnemyHeadGraphSize, 1.7, 0.0, m_enemyHeadGraph, true, 0, 0);
}
