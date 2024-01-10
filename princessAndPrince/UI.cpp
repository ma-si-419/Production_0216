#include "UI.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Princess.h"
#include "UserData.h"
#include "SceneMain.h"
namespace
{
	constexpr float kMaxBarWidth = 200.0f;
	constexpr float kBarHeight = 50.0f;
	//表示する間隔
	constexpr int kShowTime = 60;
	//Exp表示するタイミング
	constexpr int kExpShowTime = 120;
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false)
{
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
	DrawBox(960, 0, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(100, 100, 100), true);
	//入手した経験値の量をプレイヤーから取得して表示する	
	DrawFormatString(Game::kScreenWidth - 300, 100,//座標
		GetColor(0, 0, 0), "取得経験値%d", m_pPlayer->GetExp());

	//入手したゴールドの量をプレイヤーから取得して表示する
	DrawFormatString(Game::kScreenWidth - 300, 200,//座標
		GetColor(0, 0, 0), "取得ゴールド%d", m_pPlayer->GetGold());

	//攻撃力のレベルを表示する
	DrawFormatString(Game::kScreenWidth - 200, 300,//座標
		GetColor(0, 0, 0), "こうげきLv　%d", UserData::userAtkLevel + 1);
	//防御力のレベルを表示する	
	DrawFormatString(Game::kScreenWidth - 200, 400,//座標
		GetColor(0, 0, 0), "ぼうぎょLv　%d", UserData::userDefLevel + 1);
	//移動速度のレベルを表示する	
	DrawFormatString(Game::kScreenWidth - 200, 500,//座標
		GetColor(0, 0, 0), "は や さLv　%d", UserData::userSpdLevel + 1);
	//炎魔法のレベルを表示する	
	DrawFormatString(Game::kScreenWidth - 200, 600,//座標
		GetColor(0, 0, 0), "ファイアLv　%d", UserData::userFireLevel + 1);
	//風魔法のレベルを表示する
	DrawFormatString(Game::kScreenWidth - 200, 700,//座標
		GetColor(0, 0, 0), "ウィンドLv　%d", UserData::userWindLevel + 1);
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
		DrawString((Game::kPlayScreenWidth - stringWidth) / 2, 200, "ゲームクリア", GetColor(0, 0, 0));
		//獲得したゴールドと経験値を表示する
		DrawString(300, 500, "獲得経験値", GetColor(0, 0, 0));
		DrawString(300, 600, "所持経験値", GetColor(0, 0, 0));
		DrawString(300, 700, "獲得ゴールド", GetColor(0, 0, 0));
		DrawString(300, 800, "所持ゴールド", GetColor(0, 0, 0));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(600, 500, GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());
		DrawFormatString(600, 600, GetColor(0, 0, 0), "%d", UserData::userExp);
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(600, 700, GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());
		DrawFormatString(600, 800, GetColor(0, 0, 0), "%d", UserData::userGold);
	}
}
