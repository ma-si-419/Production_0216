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
	constexpr int kMaxSceneNum = 2;
	//ポーズを開いた時の項目の数
	constexpr int kMaxPauseNum = 2;
}
SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& DataManager) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(0),
	m_pauseSelectNum(0),
	m_isSelectKeyDown(false),
	m_isPause(false),
	m_isStatus(false),
	m_isSelectScene(false)
{
	m_appSe = DataManager.SearchSound("approveSe");
	m_cursorSe = DataManager.SearchSound("cursorSe");
	m_cancelSe = DataManager.SearchSound("cancelSe");
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
		if (!m_input.Buttons[XINPUT_BUTTON_A] && !m_input.Buttons[XINPUT_BUTTON_B] && //パッド
			!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_ESCAPE))//キーボード
		{
			m_isKeyDown = true;
		}
		//上キーと下キーが離されたら
		if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&//パッド
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
		{
			m_isSelectKeyDown = false;
		}
	}
	//ポーズが開かれていないとき
	if (!m_isPause)
	{
		//連続で押されないための処理
		if (m_isKeyDown)
		{
			if (!m_isSelectScene)
			{
				//Aボタンが押されたら
				if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Bボタンを押したらタイトルに戻る
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Yボタンが押されたら
				else if (m_input.Buttons[XINPUT_BUTTON_Y] && !m_isPause || CheckHitKey(KEY_INPUT_P))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					m_isPause = true;
				}
			}
		}

		if (!m_isSelectKeyDown)
		{

			//上キーが押されたら
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_stageSelectNum--;
				if (m_stageSelectNum < 0)
				{
					m_stageSelectNum = kMaxSceneNum;
				}
				m_isSelectKeyDown = true;
			}
			//下キーが入力されたら
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				m_stageSelectNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				if (m_stageSelectNum > kMaxSceneNum)
				{
					m_stageSelectNum = 0;
				}
				m_isSelectKeyDown = true;
			}
		}
	}
	//ポーズが開かれているとき
	if (m_isPause && !m_isStatus)
	{
		if (!m_isSelectKeyDown)
		{
			//上キーが押されたら
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
			{
				m_pauseSelectNum--;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				if (m_pauseSelectNum < 0)
				{
					m_pauseSelectNum = kMaxPauseNum;
				}
				m_isSelectKeyDown = true;
			}
			//下キーが入力されたら
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				m_pauseSelectNum++;
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				if (m_pauseSelectNum > kMaxPauseNum)
				{
					m_pauseSelectNum = 0;
				}
				m_isSelectKeyDown = true;
			}
		}
		if (m_isKeyDown)
		{

			//Bボタンを押したらポーズが解ける
			if (m_input.Buttons[XINPUT_BUTTON_B] && !m_isStatus ||
				CheckHitKey(KEY_INPUT_ESCAPE) && !m_isStatus)
			{
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isPause = false;
				m_isKeyDown = false;
			}
			//Aボタンが押されたらm_pauseSelectNumに応じて処理を行う
			else if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
			{
				if (!m_isSelectScene)
				{

					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					switch (m_pauseSelectNum)
					{
					case 0://つよさ
						//ステータスをひらく
						m_isStatus = true;
						m_isKeyDown = false;
						break;
					case 1://おみせ
					{
						//ショップシーンに移行する
						m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager));
						m_isSelectScene = true;
					}
					break;
					case 2://タイトル
					{
						//タイトルシーンに移行する
						m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
						m_isSelectScene = true;
					}
					break;
					}
					m_isKeyDown = false;
				}
			}
		}
	}
	//つよさ画面が開かれているとき
	if (m_isStatus)
	{
		if (m_isKeyDown)
		{

			//Bボタンが押されたらポーズ画面に戻る
			if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
			{
				PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
				m_isStatus = false;
				m_isKeyDown = false;
			}
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
