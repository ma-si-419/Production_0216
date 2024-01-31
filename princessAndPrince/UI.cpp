#include "UI.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Princess.h"
#include "UserData.h"
#include "SceneMain.h"
#include "DataManager.h"
namespace
{
	constexpr float kMaxBarWidth = 200.0f;
	constexpr float kBarHeight = 50.0f;
	//表示する間隔
	constexpr int kShowTime = 60;
	//Exp表示するタイミング
	constexpr int kExpShowTime = 120;
	//フォントの大きさ(半角)
	constexpr int kFontSize = 24;
	//ステータスを表示する座標
	constexpr int kStatusPos = 1100;
	//ステータスのレベルを表示座標
	constexpr int kStatusLevelPos = 1430;
	//取得したアイテムを表示する座標
	constexpr int kGetItemPos = 1350;
	//フォントの後ろに表示する影のずらす大きさ
	constexpr int kShiftShadowLange = 3;
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false),
	m_isLeaveButton(false),
	m_isClearUIEnd(false)
{
	m_buttonsGraph = m_pMain->GetButtonsGraph();
}

UI::~UI()
{
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw()
{
	//UIの背景(右側)表示
	DrawGraph(960, 0, m_pMain->GetUiBg(), true);


	DrawGetItem();

	DrawStatus();
	//今どちらの魔法を打っているかを表示する
	if (m_pPrincess->GetMagicKind())
	{
		DrawBox(960, Game::kPlayScreenHeight - 100, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(255, 0, 0), true);
	}
	else
	{
		DrawBox(960, Game::kPlayScreenHeight - 100, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(0, 255, 0), true);
	}
	//聖剣モードのゲージを表示する
	//後ろに黒いBoxを出す
	DrawBox(Game::kPlayScreenWidth, 700,//始点
		Game::kPlayScreenWidth + (int)kMaxBarWidth,//始点の位置にバーの長さを足す
		700 + (int)kBarHeight,//終点
		GetColor(0, 0, 0), true);
	DrawBox(Game::kPlayScreenWidth, 700,//始点
		Game::kPlayScreenWidth + (int)(kMaxBarWidth / m_pMain->GetSpecialGaugeRate()),//始点の位置にバーの長さを足す
		700 + (int)kBarHeight,//終点
		GetColor(255, 255, 255), true);

}

void UI::SceneClearUI()
{
	m_timeCount++;
	//黒い少し透明なボックスを表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		int stringWidth = GetDrawStringWidth("ゲームクリア", -1);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 - 150, 200, "ゲ ー ム", GetColor(255, 255, 255));
		DrawRectRotaGraph((Game::kPlayScreenWidth - stringWidth) / 2 + 100, 200,
			48, Game::kDirDeath * 16,
			16, 16,
			6.0,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 + 150, 200, "ク リ ア", GetColor(255, 255, 255));
		//現在のレベルを表示する
		DrawString(100, 400, "現在のレベル", GetColor(255, 255, 255));
		DrawFormatString(700 - AlignmentRight(UserData::userMainLevel + 1), 400, GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//獲得したゴールドと経験値を表示する
		DrawString(100, 500, "獲得経験値", GetColor(255, 255, 255));
		DrawString(100, 600, "次のレベルまで", GetColor(255, 255, 255));
		DrawString(100, 700, "獲得ゴールド", GetColor(255, 255, 255));
		DrawString(100, 800, "所持ゴールド", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetExp()), 500, GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(700 - AlignmentRight(m_pMain->GetNextExp()), 600, GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetGold()), 700, GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(700 - AlignmentRight(UserData::userGold), 800, GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	if (m_isLeaveButton)
	{
		DrawRectRotaGraph(440, 900, 0, 0, 32, 32, 2.4, 0.0, m_buttonsGraph, true, 0, 0);
		DrawString(480, 880, "で戻る", GetColor(255, 255, 255));
	}
}

void UI::GameOverUI()
{
	m_timeCount++;
	//黒い少し透明なボックスを表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		int stringWidth = GetDrawStringWidth("ゲームオーバー", -1);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 - 150, 200, "ゲ ー ム", GetColor(255, 255, 255));
		DrawRectRotaGraph((Game::kPlayScreenWidth - stringWidth) / 2 + 100, 220,
			16, Game::kDirDeath * 16,
			16, 16,
			6.0,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2 + 150, 200, "オ ー バ ー", GetColor(255, 255, 255));
		//現在のレベルを表示する
		DrawString(100, 400, "現在のレベル", GetColor(255, 255, 255));
		DrawFormatString(700 - AlignmentRight(UserData::userMainLevel + 1), 400, GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//獲得したゴールドと経験値を表示する
		DrawString(100, 500, "獲得経験値", GetColor(255, 255, 255));
		DrawString(100, 600, "次のレベルまで", GetColor(255, 255, 255));
		DrawString(100, 700, "獲得ゴールド", GetColor(255, 255, 255));
		DrawString(100, 800, "所持ゴールド", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetExp()), 500, GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(700 - AlignmentRight(m_pMain->GetNextExp()), 600, GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(700 - AlignmentRight(m_pPlayer->GetGold()), 700, GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(700 - AlignmentRight(UserData::userGold), 800, GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	if (m_isLeaveButton)
	{
		DrawCircle(400, 900, 20, GetColor(255, 255, 255), true);
		DrawString(500, 900, "で戻る", GetColor(255, 255, 255));
	}
}

int UI::AlignmentRight(int num)
{
	if (num >= 10000)
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


void UI::DrawStatus()
{
	//影を表示
	{
		DrawString(kStatusPos + kShiftShadowLange, 350 + kShiftShadowLange, "こうげきLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userAtkLevel + 1), 350 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userAtkLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 440 + kShiftShadowLange, "ぼうぎょLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userDefLevel + 1), 440 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userDefLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 530 + kShiftShadowLange, "は や さLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userSpdLevel + 1), 530 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userSpdLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 620 + kShiftShadowLange, "ファイアLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userFireLevel + 1), 620 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userFireLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, 710 + kShiftShadowLange, "ウィンドLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userWindLevel + 1), 710 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userWindLevel + 1);
	}
	//攻撃力のレベルを表示する
	DrawString(kStatusPos, 350, "こうげきLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userAtkLevel + 1), 350,//座標
		GetColor(255, 255, 255), "%d", UserData::userAtkLevel + 1);
	//防御力のレベルを表示する
	DrawString(kStatusPos, 440, "ぼうぎょLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userDefLevel + 1), 440,//座標
		GetColor(255, 255, 255), "%d", UserData::userDefLevel + 1);
	//移動力のレベルを表示する
	DrawString(kStatusPos, 530, "は や さLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userSpdLevel + 1), 530,//座標
		GetColor(255, 255, 255), "%d", UserData::userSpdLevel + 1);
	//風魔法のレベルを表示する
	DrawString(kStatusPos, 620, "ファイアLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), 620,//座標
		GetColor(255, 255, 255), "%d", UserData::userFireLevel + 1);
	//炎魔法のレベルを表示する
	DrawString(kStatusPos, 710, "ウィンドLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), 710,//座標
		GetColor(255, 255, 255), "%d", UserData::userWindLevel + 1);
}

void UI::DrawGetItem()
{
	//影を表示する
	{
		//入手したゴールドの量をプレイヤーから取得して表示する
		DrawFormatString(kGetItemPos + kShiftShadowLange - AlignmentRight(m_pPlayer->GetGold()), 105 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());

		//入手した経験値の量をプレイヤーから取得して表示する	
		DrawFormatString(kGetItemPos + kShiftShadowLange - AlignmentRight(m_pPlayer->GetExp()), 175 + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());
	}

	//入手したゴールドの量をプレイヤーから取得して表示する
	DrawFormatString(kGetItemPos - AlignmentRight(m_pPlayer->GetExp()), 105,//座標
		GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());

	//入手した経験値の量をプレイヤーから取得して表示する	
	DrawFormatString(kGetItemPos - AlignmentRight(m_pPlayer->GetExp()), 175,//座標
		GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
}
