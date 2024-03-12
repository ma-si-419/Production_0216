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
	constexpr float kMaxBarWidth = 189.0f;
	constexpr float kBarHeight = 100.0f;
	//表示する間隔
	constexpr int kShowTime = 60;
	//Exp表示するタイミング
	constexpr int kExpShowTime = 120;
	//フォントの大きさ(半角)
	constexpr int kFontSize = 24;
	//ステータスを表示する座標
	constexpr int kStatusPos = 1100;
	//プレイヤーのレベルをを表示する座標
	constexpr int kMainStatusPos = 1210;
	//ステータスのレベルを表示座標
	constexpr int kStatusLevelPos = 1430;
	//メインレベルの数を表示する座標
	constexpr int kMainLevelPos = 1325;
	//取得したアイテムを表示する座標
	constexpr int kGetItemPosX = 1350;
	//フォントの後ろに表示する影のずらす大きさ
	constexpr int kShiftShadowLange = 3;
	//アイテムグラフの大きさ
	constexpr int kItemGraphScale = 32;
	//ゲームクリア時のプレイヤーのアニメフレーム
	constexpr int kPlayerClearAnimFrame = 48;
	//ゲームオーバー時のプレイヤーのアニメフレーム
	constexpr int kPlayerDeathAnimFrame = 16;
	//プレイヤーの画像の大きさ
	constexpr int kPlayerGraphSize = 16;
	//プレイヤーの拡大率
	constexpr double kPlayerScale = 6.0;
	//Uiを揺らす幅
	constexpr int kShakeLange = 1;
	//入手した経験値とゴールドを表示する座標
	constexpr int kGetItemPosY[2] = { 105,175 };
	//Uiを表示する座標
	constexpr int kUiPosYArr[6] = { 315,390,465,540,615,690 };
	//ボタンがどこまで大きくなるか
	constexpr double kAngryButtonMaxRatio = 0.8;
	//ボタンがどこまで小さくなるか
	constexpr double kAngryButtonMinRatio = 0.4;
	//ボタンが大きくなるスピード
	constexpr double kAngryButtonZoomSpeed = 0.01;
	//魔法のアイコンを表示する座標
	constexpr int kMagicUiPosX[2] = { 1320,1495 };
	constexpr int kMagicUiPosY = 855;
	//魔法のアイコンを表示する背景画像のサイズ
	constexpr int kMagicUiBgSize = 106;
	//魔法のアイコンの大きさ倍率
	constexpr double kMagicUiRatio = 2.7;
	//ファイアのアイコンの角度
	constexpr double kFireUiAngle = 4.72;
	//怒りゲージの座標
	constexpr int kAngryGaugePosX = 1060;
	constexpr int kAngryGaugePosY = 805;
	//怒りゲージのフレームの座標
	constexpr int kAngryGaugeFramePosX = 1050;
	constexpr int kAngryGaugeFramePosY = 800;
	//魔法のUIを表示するときに出す円の大きさ
	constexpr int kMagicUiCircleRadius = 50;
	//ゲームクリアと表示する座標
	constexpr int kGameClearPosX[2] = { 230,530 };
	//ゲームオーバーと表示する座標
	constexpr int kGameOverPosX[2] = { 200,500 };
	//ゲームオーバーの文字の中に出すプレイヤーの座標
	constexpr int kGameOverPlayerPos = 450;
	//ゲームクリアの文字の中に出すプレイヤーの座標
	constexpr int kGameClearPlayerPos = 480;
	//ゲームクリア時のリザルトを表示する座標
	constexpr int kResultPosX[2] = {200,750};
	constexpr int kResultPosY[8] = { 210,380,480,580,680,780,900,880 };
	//ボタンのサイズ
	constexpr double kButtonSize = 2.5;
	//ボタンを表示するポジション
	constexpr int kButtonPosX = 440;
	//戻るを表示するポジション
	constexpr int kLeavePosX = 480;
	//黒いボックスの大きさ
	constexpr int kBlackBoxScale = 965;
	//魔法が使えないシーン
	constexpr int kCantMagicScene = 2;
	//怒りモードが使えないシーン
	constexpr int kCantAngryModeScene = 3;
	//魔法が使えないときのUIの色
	const int CantMagicUiColor = GetColor(128, 128, 128);
}
UI::UI(Player* pPlayer, Princess* pPrincess, SceneMain* pMain) :
	m_pPlayer(pPlayer),
	m_pPrincess(pPrincess),
	m_pMain(pMain),
	m_timeCount(0),
	m_isShowGold(false),
	m_isLeaveButton(false),
	m_isClearUIEnd(false),
	m_angryGaugeUiShiftPosX(0),
	m_stoneAngryGaugeGraph(0),
	m_angryButtonGraph(0),
	m_angryButtonRatio(1.0),
	m_angryGaugeUiGraph(0),
	m_isAngryGaugeUiShake(false),
	m_isButtonZoom(false),
	m_magicGraph(0),
	m_magicUiBgGraph(0)


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
	if (m_pMain->GetSpecialGaugeRate() == 1)
	{
		if (m_isAngryGaugeUiShake)
		{
			m_angryGaugeUiShiftPosX++;
		}
		else if (!m_isAngryGaugeUiShake)
		{
			m_angryGaugeUiShiftPosX--;
		}
	}
	if (m_angryGaugeUiShiftPosX > kShakeLange)
	{
		m_isAngryGaugeUiShake = false;
	}
	else if (m_angryGaugeUiShiftPosX < kShakeLange)
	{
		m_isAngryGaugeUiShake = true;
	}
	if (m_isButtonZoom)
	{
		m_angryButtonRatio += kAngryButtonZoomSpeed;
	}
	else
	{
		m_angryButtonRatio -= kAngryButtonZoomSpeed;
	}
	if (m_angryButtonRatio > kAngryButtonMaxRatio)
	{
		m_isButtonZoom = false;
		m_angryButtonRatio = kAngryButtonMaxRatio;
	}
	else if (m_angryButtonRatio < kAngryButtonMinRatio)
	{
		m_isButtonZoom = true;
		m_angryButtonRatio = kAngryButtonMinRatio;
	}

}
void UI::Draw()
{
	//UIの背景(右側)表示
	DrawGraph(Game::kPlayScreenWidth, 0, m_pMain->GetUiBg(), true);
	//入手したアイテムを表示する
	DrawGetItem();
	//ステータスを表示する
	DrawStatus();
	//今どちらの魔法を打っているかを表示する
	if (m_pPrincess->GetMagicKind())
	{
		DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, kMagicUiBgSize, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);

	}
	else
	{
		DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, kMagicUiBgSize, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);

	}
	//シーン3まではどちらも光らせないようにする
	if (m_pMain->GetSceneNum() < kCantMagicScene)
	{
		DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
		DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, 0, 0, kMagicUiBgSize, kMagicUiBgSize, 1.0, 0.0, m_magicUiBgGraph, true, false, false);
	}
	//炎魔法のUi表示
	DrawRectRotaGraph(kMagicUiPosX[0], kMagicUiPosY, 0, static_cast<int>(Game::ItemGraph::kFire) * kItemGraphScale, kItemGraphScale, kItemGraphScale,
		kMagicUiRatio, kFireUiAngle, m_magicGraph, true, false, false);
	//風魔法のUi表示
	DrawRectRotaGraph(kMagicUiPosX[1], kMagicUiPosY, 0, static_cast<int>(Game::ItemGraph::kWind) * kItemGraphScale, kItemGraphScale, kItemGraphScale,
		kMagicUiRatio, 0.0, m_magicGraph, true, false, false);
	if (m_pMain->GetSceneNum() > 1)
	{
		//使っている魔法をわかりやすくする
		if (m_pPrincess->GetMagicKind())
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawCircle(kMagicUiPosX[1], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawCircle(kMagicUiPosX[0], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	//シーン３まではどちらも灰色に表示する
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawCircle(kMagicUiPosX[0], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
		DrawCircle(kMagicUiPosX[1], kMagicUiPosY, kMagicUiCircleRadius, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//怒りモードのゲージを表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(kAngryGaugePosX, kAngryGaugePosY,//始点
		kAngryGaugePosX + (int)(kMaxBarWidth / m_pMain->GetSpecialGaugeRate()),//始点の位置にバーの長さを足す
		kAngryGaugePosY + (int)kBarHeight, GetColor(255, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//怒りモードのゲージのフレームを表示する
	DrawGraph(kAngryGaugeFramePosX + m_angryGaugeUiShiftPosX, kAngryGaugeFramePosY, m_angryGaugeUiGraph, true);
	//ステージ３までは怒りモードのゲージを表示しない
	if (m_pMain->GetSceneNum() < kCantAngryModeScene)
	{
		DrawGraph(kAngryGaugeFramePosX + m_angryGaugeUiShiftPosX, kAngryGaugeFramePosY, m_stoneAngryGaugeGraph, true);
	}
	if (m_pMain->GetSpecialGaugeRate() == 1)
	{
		DrawRotaGraph(kAngryGaugeFramePosX, kAngryGaugeFramePosY, m_angryButtonRatio, 0.0, m_angryButtonGraph, true, 0, 0);
	}
}
void UI::SceneClearUI()
{
	m_timeCount++;
	//黒い少し透明なボックスを表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, kBlackBoxScale, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		
		DrawString(kGameClearPosX[0], kResultPosY[0], "ゲ ー ム", GetColor(255, 255, 255));
		//プレイヤーの画像を表示する
		DrawRectRotaGraph(kGameClearPlayerPos, kResultPosY[0],
			kPlayerClearAnimFrame, static_cast<int>(Game::Dir::kDirDeath) * kPlayerGraphSize,
			kPlayerGraphSize, kPlayerGraphSize,
			kPlayerScale,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString(kGameClearPosX[1], kResultPosY[0], "ク リ ア", GetColor(255, 255, 255));
		//現在のレベルを表示する
		DrawString(kResultPosX[0], kResultPosY[1], "現在のレベル", GetColor(255, 255, 255));
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userMainLevel + 1), kResultPosY[1], GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//獲得したゴールドと経験値を表示する
		DrawString(kResultPosX[0], kResultPosY[2], "獲得経験値", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[3], "次のレベルまで", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[4], "獲得ゴールド", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[5], "所持ゴールド", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	//経験値のリザルトを表示する
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetExp()), kResultPosY[2], GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pMain->GetNextExp()), kResultPosY[3], GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	//ゴールドのリザルトを表示する
	if (m_isShowGold)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetGold()), kResultPosY[4], GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userGold), kResultPosY[5], GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	//Aボタンで戻るを表示する
	if (m_isLeaveButton)
	{
		DrawRotaGraph(kButtonPosX, kResultPosY[6], kButtonSize, 0,m_buttonsGraph, true, 0, 0);
		DrawString(kLeavePosX, kResultPosY[7], "で戻る", GetColor(255, 255, 255));
	}
}
void UI::GameOverUI()
{
	m_timeCount++;
	//黒い少し透明なボックスを表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, kBlackBoxScale, Game::kPlayScreenHeight,
		GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if (m_timeCount > kShowTime)
	{
		int stringWidth = GetDrawStringWidth("ゲームオーバー", -1);
		DrawString(kGameOverPosX[0], kResultPosY[0], "ゲ ー ム", GetColor(255, 255, 255));
		DrawRectRotaGraph(kGameOverPlayerPos, kResultPosY[0],
			kPlayerDeathAnimFrame, static_cast<int>(Game::Dir::kDirDeath) * kPlayerGraphSize,
			kPlayerGraphSize, kPlayerGraphSize,
			kPlayerScale,
			0.0,
			m_pPlayer->GetHandle(), true, false);
		DrawString(kGameOverPosX[1], kResultPosY[0], "オ ー バ ー", GetColor(255, 255, 255));
		//現在のレベルを表示する
		DrawString(kResultPosX[0], kResultPosY[1], "現在のレベル", GetColor(255, 255, 255));
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userMainLevel + 1), kResultPosY[1], GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
		//獲得したゴールドと経験値を表示する
		DrawString(kResultPosX[0], kResultPosY[2], "獲得経験値", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[3], "次のレベルまで", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[4], "獲得ゴールド", GetColor(255, 255, 255));
		DrawString(kResultPosX[0], kResultPosY[5], "所持ゴールド", GetColor(255, 255, 255));
		m_pMain->SetEnd();
	}
	if (m_timeCount > kExpShowTime)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetExp()), kResultPosY[2], GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pMain->GetNextExp()), kResultPosY[3], GetColor(255, 255, 255), "%d", m_pMain->GetNextExp());
		m_pMain->StartExpLoop();
	}
	if (m_isShowGold)
	{
		DrawFormatString(kResultPosX[1] - AlignmentRight(m_pPlayer->GetGold()), kResultPosY[4], GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());
		DrawFormatString(kResultPosX[1] - AlignmentRight(UserData::userGold), kResultPosY[5], GetColor(255, 255, 255), "%d", UserData::userGold);
	}
	if (m_isLeaveButton)
	{
		DrawRotaGraph(kButtonPosX, kResultPosY[6], kButtonSize, 0, m_buttonsGraph, true, 0, 0);
		DrawString(kLeavePosX, kResultPosY[7], "で戻る", GetColor(255, 255, 255));
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
		DrawString(kMainStatusPos + kShiftShadowLange, kUiPosYArr[0] + kShiftShadowLange, "Lv", GetColor(0, 0, 0));
		DrawFormatString(kMainLevelPos + kShiftShadowLange - AlignmentRight(UserData::userMainLevel + 1), kUiPosYArr[0] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userMainLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[1] + kShiftShadowLange, "こうげきLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userAtkLevel + 1), kUiPosYArr[1] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userAtkLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[2] + kShiftShadowLange, "ぼうぎょLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userDefLevel + 1), kUiPosYArr[2] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userDefLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[3] + kShiftShadowLange, "は や さLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userSpdLevel + 1), kUiPosYArr[3] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userSpdLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[4] + kShiftShadowLange, "ファイアLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userFireLevel + 1);
		DrawString(kStatusPos + kShiftShadowLange, kUiPosYArr[5] + kShiftShadowLange, "ウィンドLv", GetColor(0, 0, 0));
		DrawFormatString(kStatusLevelPos + kShiftShadowLange - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", UserData::userWindLevel + 1);
	}
	//メインレベルを表示する
	DrawString(kMainStatusPos, kUiPosYArr[0], "Lv", GetColor(255, 255, 255));
	DrawFormatString(kMainLevelPos - AlignmentRight(UserData::userMainLevel + 1), kUiPosYArr[0],//座標
		GetColor(255, 255, 255), "%d", UserData::userMainLevel + 1);
	//攻撃力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[1], "こうげきLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userAtkLevel + 1), kUiPosYArr[1],//座標
		GetColor(255, 255, 255), "%d", UserData::userAtkLevel + 1);
	//防御力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[2], "ぼうぎょLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userDefLevel + 1), kUiPosYArr[2],//座標
		GetColor(255, 255, 255), "%d", UserData::userDefLevel + 1);
	//移動力のレベルを表示する
	DrawString(kStatusPos, kUiPosYArr[3], "は や さLv", GetColor(255, 255, 255));
	DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userSpdLevel + 1), kUiPosYArr[3],//座標
		GetColor(255, 255, 255), "%d", UserData::userSpdLevel + 1);
	//ステージ２までは灰色で表示しておく
	if (m_pMain->GetSceneNum() > 1)
	{
		//風魔法のレベルを表示する
		DrawString(kStatusPos, kUiPosYArr[4], "ファイアLv", GetColor(255, 255, 255));
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4],//座標
			GetColor(255, 255, 255), "%d", UserData::userFireLevel + 1);
		//炎魔法のレベルを表示する
		DrawString(kStatusPos, kUiPosYArr[5], "ウィンドLv", GetColor(255, 255, 255));
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5],//座標
			GetColor(255, 255, 255), "%d", UserData::userWindLevel + 1);
	}
	else
	{
		//風魔法のレベルを表示する
		DrawString(kStatusPos, kUiPosYArr[4], "ファイアLv", CantMagicUiColor);
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userFireLevel + 1), kUiPosYArr[4],//座標
			CantMagicUiColor, "%d", UserData::userFireLevel + 1);
		//炎魔法のレベルを表示する
		DrawString(kStatusPos, kUiPosYArr[5], "ウィンドLv", CantMagicUiColor);
		DrawFormatString(kStatusLevelPos - AlignmentRight(UserData::userWindLevel + 1), kUiPosYArr[5],//座標
			CantMagicUiColor, "%d", UserData::userWindLevel + 1);
	}
}
void UI::DrawGetItem()
{
	//影を表示する
	{
		//入手したゴールドの量をプレイヤーから取得して表示する
		DrawFormatString(kGetItemPosX + kShiftShadowLange - AlignmentRight(m_pPlayer->GetGold()), kGetItemPosY[0] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());

		//入手した経験値の量をプレイヤーから取得して表示する	
		DrawFormatString(kGetItemPosX + kShiftShadowLange - AlignmentRight(m_pPlayer->GetExp()), kGetItemPosY[1] + kShiftShadowLange,//座標
			GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());
	}

	//入手したゴールドの量をプレイヤーから取得して表示する
	DrawFormatString(kGetItemPosX - AlignmentRight(m_pPlayer->GetGold()), kGetItemPosY[0],//座標
		GetColor(255, 255, 255), "%d", m_pPlayer->GetGold());

	//入手した経験値の量をプレイヤーから取得して表示する	
	DrawFormatString(kGetItemPosX - AlignmentRight(m_pPlayer->GetExp()), kGetItemPosY[1],//座標
		GetColor(255, 255, 255), "%d", m_pPlayer->GetExp());
}