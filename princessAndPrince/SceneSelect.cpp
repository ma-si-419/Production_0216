#include "SceneSelect.h"
#include "SceneManager.h"
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
	constexpr int kMaxSceneNum = 2;
	//ポーズを開いた時の項目の数
	constexpr int kMaxPauseNum = 2;
}
SceneSelect::SceneSelect(SceneManager& manager) :
	Scene(manager),
	m_isKeyDown(false),
	m_stageSelectNum(0),
	m_pauseSelectNum(0),
	m_isSelectKeyDown(false),
	m_isPause(false),
	m_isStatus(false)
{
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
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//ボタンが連続で押されないための処理
	{
		//AボタンとBボタンが連続で押されないように
		if (!m_input.Buttons[XINPUT_BUTTON_A] && !m_input.Buttons[XINPUT_BUTTON_B])
		{
			m_isKeyDown = true;
		}
		//上キーと下キーが離されたら
		if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
		{
			m_isSelectKeyDown = false;
		}
	}
	//ポーズが開かれていないとき
	if (!m_isPause)
	{
		//Aボタンが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown)
		{
			m_manager.ChangeScene(std::make_shared<SceneMain>(m_manager, m_stageSelectNum));
		}
		//Bボタンを押したらタイトルに戻る
		if (m_input.Buttons[XINPUT_BUTTON_B] && m_isKeyDown)
		{
			m_manager.ChangeScene(std::make_shared<SceneTitle>(m_manager));
		}
		//Yボタンが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_Y] && m_isKeyDown && !m_isPause)
		{
			m_isPause = true;
		}

		//上キーが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_isSelectKeyDown)
		{
			m_stageSelectNum--;
			if (m_stageSelectNum < 0)
			{
				m_stageSelectNum = kMaxSceneNum;
			}
			m_isSelectKeyDown = true;
		}
		//下キーが入力されたら
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && !m_isSelectKeyDown)
		{
			m_stageSelectNum++;
			if (m_stageSelectNum > kMaxSceneNum)
			{
				m_stageSelectNum = 0;
			}
			m_isSelectKeyDown = true;
		}
	}
	//ポーズが開かれているとき
	if (m_isPause && !m_isStatus)
	{
		//上キーが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_isSelectKeyDown)
		{
			m_pauseSelectNum--;
			if (m_pauseSelectNum < 0)
			{
				m_pauseSelectNum = kMaxPauseNum;
			}
			m_isSelectKeyDown = true;
		}
		//下キーが入力されたら
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && !m_isSelectKeyDown)
		{
			m_pauseSelectNum++;
			if (m_pauseSelectNum > kMaxPauseNum)
			{
				m_pauseSelectNum = 0;
			}
			m_isSelectKeyDown = true;
		}
		//Bボタンを押したらポーズが解ける
		if (m_input.Buttons[XINPUT_BUTTON_B] && m_isKeyDown && !m_isStatus)
		{
			m_isPause = false;
			m_isKeyDown = false;
		}
		//Aボタンが押されたらm_pauseSelectNumに応じて処理を行う
		if (m_input.Buttons[XINPUT_BUTTON_A] && m_isKeyDown)
		{
			switch (m_pauseSelectNum)
			{
			case 0://つよさ
				//ステータスをひらく
				m_isStatus = true;
				m_isKeyDown = false;
				break;
			case 1://おみせ
				//ショップシーンに移行する
				m_manager.ChangeScene(std::make_shared<SceneShop>(m_manager));
				break;
			case 2://タイトル
				//タイトルシーンに移行する
				m_manager.ChangeScene(std::make_shared<SceneTitle>(m_manager));
				break;
			}
			m_isKeyDown = false;
		}
	}
	//つよさ画面が開かれているとき
	if (m_isStatus)
	{
		//Bボタンが押されたらポーズ画面に戻る
		if (m_input.Buttons[XINPUT_BUTTON_B] && m_isKeyDown)
		{
			m_isStatus = false;
			m_isKeyDown = false;
		}
	}
}

void SceneSelect::Draw()
{
	DrawFormatString(300, 300, GetColor(255, 255, 255), "%d,%d", UserData::userGold, UserData::userExp);
	DrawFormatString(200, 200, GetColor(255, 255, 255), "%d", m_stageSelectNum);
	DrawFormatString(400, 400, GetColor(255, 255, 255), "ステージ%d", m_stageSelectNum + 1);
	//ポーズ中の処理
	if (m_isPause)
	{
		DrawBox(400, 400, 800, 800, GetColor(255, 200, 0), true);
		DrawString(600, 500, "つよさ", GetColor(0, 0, 0));
		DrawString(600, 600, "おみせ", GetColor(0, 0, 0));
		DrawString(600, 700, "タイトル", GetColor(0, 0, 0));

		switch (m_pauseSelectNum)
		{
		case 0:
			DrawString(600, 500, "つよさ", GetColor(255, 0, 0));
			break;
		case 1:
			DrawString(600, 600, "おみせ", GetColor(255, 0, 0));
			break;
		case 2:
			DrawString(600, 700, "タイトル", GetColor(255, 0, 0));
			break;

		}
	}
	//ポーズ画面から強さを押したときの処理
	if (m_isStatus)
	{
		DrawBox(400, 400, 800, 800, GetColor(255, 255, 255), true);
		DrawFormatString(500, 450, GetColor(0, 0, 0), "ゴールド   %d", UserData::userGold);
		DrawFormatString(500, 500, GetColor(0, 0, 0), "けいけん   %d", UserData::userExp);
		DrawFormatString(500, 550, GetColor(0, 0, 0), "こうげきLv %d", UserData::userAtkLevel + 1);
		DrawFormatString(500, 600, GetColor(0, 0, 0), "ぼうぎょLv %d", UserData::userDefLevel + 1);
		DrawFormatString(500, 650, GetColor(0, 0, 0), "い ど うLv %d", UserData::userSpdLevel + 1);
		DrawFormatString(500, 700, GetColor(0, 0, 0), "ファイアLv %d", UserData::userFireLevel + 1);
		DrawFormatString(500, 750, GetColor(0, 0, 0), "ウィンドLv %d", UserData::userWindLevel + 1);

	}

}
