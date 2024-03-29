#include "SceneSelect.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneSave.h"
#include "SceneShop.h"
#include "UserData.h"
#include "Player.h"
#include "MyString.h"
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
	constexpr float kPlayerScale = 6.0f;
	constexpr float kPrincessScale = 5.0f;
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
	//ボスのアニメーションの長さ
	constexpr int kBossAnimFrameNum = 32;
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
	constexpr int kShopMaxSizePosX = -150;
	//キャラの動くスピード
	constexpr int kCharMoveSpeed = 10;
	//背景の上辺の高さ
	constexpr int kBgTopEndPos = 125;
	//ボスのグラフィックの大きさ
	constexpr int kBossGraphSize = 32;
	//ボスを表示する座標
	constexpr int kBossPosY = 300;
	//ボスのフェードの速さ
	constexpr int kBossFadeSpeed = 10;
	//配列の大きさ
	constexpr int kArraySize = 81;
	//ビックリマークの初期座標
	constexpr int kExclamationMarkPosX = 1100;
	constexpr int kExclamationMarkPosY = 450;
	//ビックリマークが揺れる幅
	constexpr float kZoomExclamationMarkLange = 1.5f;
	//ビックリマークが動く速さ
	constexpr float kExclamationMarkZoomSpeed = 0.02f;
	//三角形を動かす長さ
	constexpr float kShakeTriangleLange = 15.0f;
	//各ステージのボスの順番
	constexpr int kStageBossKind[8] = { 0,5,1,3,6,2,4,7 };
	//姫のアニメフレーム
	constexpr int kPrincessAnimFrame = 24; 
	//シーン移動時のプレイヤーのアニメフレーム
	constexpr int kPlayerAnimFrame = 24;
	//ステージのタイトルを表示する座標
	constexpr int kStageTitleStartPosX = 70;
	constexpr int kStageTitleStartPosY = 90;
	constexpr int kStageTitleEndPosX = 730;
	constexpr int kStageTitleEndPosY = 180;
	//シーン移動時の黒いボックスの回転速度
	constexpr float kBoxRotaSpeed = 0.15f;
	//シーン移動時の黒いボックスのズームの速度
	constexpr float kBoxZoomSpeed = 0.4f;
	//各ステージのボスの大きさ
	constexpr double kBossScale = 8.0;
	//ストーリーを表示する座標
	constexpr int kStoryPosX = 130;
	constexpr int kStoryPosY = 200;
	//操作説明を表示する座標
	constexpr int kExplanationPosX = 130;
	constexpr int kExplanationPosY = 650;
	//フレームの座標
	constexpr int kFrameStartPosX = 480;
	constexpr int kFrameStartPosY = 180;
	constexpr int kFrameEndPosX = 1090;
	constexpr int kFrameEndPosY = 795;
}
SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum) :
	Scene(sceneManager, DataManager),
	m_isKeyDown(false),
	m_stageSelectNum(selectSceneNum),
	m_isSelectKeyDown(false),
	m_isSelectScene(false),
	m_animFrame(kAnimFrameNum),
	m_dir(Game::Dir::kDirDown),
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
	m_isMoveMainScene(false),
	m_clearStageNum(0),
	m_charPosY(0),
	m_isCharArrTopEnd(false),
	m_isCharArrBottomEnd(false),
	m_isCharMoveDown(false),
	m_bossSrcY(selectSceneNum),
	m_bossSrcX(0),
	m_bossAnimFrame(0),
	m_bossAlpha(0),
	m_boxRatio(0),
	m_boxAngle(0),
	m_isBuy(false),
	m_exclamationMarkRatio(1.0f),
	m_princessItemPriceList(),
	m_playerItemPriceList(),
	m_exclamationMarkZoomSpeed(kExclamationMarkZoomSpeed),
	m_shakeTrianglePosY(0),
	m_isMoveUpTriangle(false)

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
	m_enemyGraph = DataManager.SearchGraph("enemyGraph");
	m_exclamationMarkGraph = DataManager.SearchGraph("exclamationMarkGraph");
	m_boxGraph = DataManager.SearchGraph("boxGraph");
	//選択不可能ステージにならないように
	if (m_stageSelectNum > kMaxSceneNum)
	{
		m_stageSelectNum = kMaxSceneNum;
		m_bossSrcY = kMaxSceneNum;
		m_cutBgPosY = kBgGraphSize * kMaxSceneNum - kMaxSceneNum * kBgGraphSize;
	}
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
	int loopCount = 0;
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
		m_playerItemPriceList[loopCount] = tempItem.playerItemPrice;
		m_princessItemPriceList[loopCount] = tempItem.princessItemPrice;
		loopCount++;
	}
	ifs.close();
	//ショップに変えるアイテムがあるかを取得する
	m_isBuy = GetCanBuyItem();

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
					m_animFrame = kPlayerAnimFrame;
					m_dir = Game::Dir::kDirDeath;
					m_isMoveMainScene = true;
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Bボタンを押したらタイトルに戻る
				else if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
				{
					StopSoundMem(m_bgm);
					PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
					SetSaveData(UserData::userSaveDataNum);
					m_sceneManager.ChangeScene(std::make_shared<SceneSave>(m_sceneManager, m_dataManager, m_stageSelectNum), true);
					m_isKeyDown = false;
					m_isSelectScene = true;
				}
				//Yボタンが押されたら
				else if (m_input.Buttons[XINPUT_BUTTON_Y] || CheckHitKey(KEY_INPUT_P))
				{
					PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
					//ショップに入る演出を入れる
					m_shopSrcY = kShopGraphSize;
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
				//移動不可だったら
				if (m_stageSelectNum >= kMaxSceneNum || m_stageSelectNum >= UserData::userClearStageNum)
				{

				}
				//移動可能だったら
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = true;
					m_bossSrcY++;
				}
				m_isSelectKeyDown = true;
			}
			//下キーが入力されたら
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))
			{
				//移動不可だったら
				if (m_stageSelectNum <= 0)
				{
					m_stageSelectNum = 0;
				}
				//移動可能だったら
				else
				{
					m_isChangeStage = true;
					PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
					m_isStaging = true;
					m_isSceneUp = false;
					m_isCharArrBottomEnd = true;
					m_isCharMoveDown = true;
					m_bossSrcY--;
				}
				m_isSelectKeyDown = true;

			}
		}
	}

	//移動の演出中
	if (m_isStaging)
	{
		MoveCharcter();
	}
	else
	{
		//移動が終わったらボスを表示する
		m_bossAlpha += kBossFadeSpeed;
		if (m_bossAlpha > 255)
		{
			m_bossAlpha = 255;
		}
	}

	//キャラが上の端に到達したら
	if (m_charPosY + kPlayerPosY < kBgTopEndPos && m_isSceneUp)
	{
		m_isCharArrTopEnd = true;
	}
	//キャラが下の端に到達したら
	if (m_charPosY > 0 && !m_isCharMoveDown)
	{
		m_isStaging = false;
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
	//ショップボタンが押されたらショップを拡大する
	if (m_isShopButton)
	{
		ZoomShop();
	}
	//ボスのアニメーションを回し続ける
	{
		m_bossAnimFrame++;
		if (m_bossAnimFrame > kBossAnimFrameNum)
		{
			m_bossSrcX = kBossGraphSize;
		}
		if (m_bossAnimFrame > kBossAnimFrameNum * 2)
		{
			m_bossAnimFrame = 0;
			m_bossSrcX = 0;
		}
	}

	//メインシーンに移動する
	if (m_isMoveMainScene)
	{
		m_boxAngle += kBoxRotaSpeed;
		m_boxRatio += kBoxZoomSpeed;
		//音が鳴りやんだらフェードしていく
		if (!CheckSoundMem(m_moveMainSceneSe))
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager, m_dataManager, m_stageSelectNum), false);
		}
	}
}

void SceneSelect::Draw()
{
	//背景の下に今選んでいるステージを表示する
	DrawRectExtendGraph(kSelectSceneStartPosX, kSelectSceneStartPosY, kSelectSceneEndPosX, kSelectSceneEndPosY,
		0, m_cutBgPosY, kBgGraphSize, kBgGraphSize, m_selectSceneBgGraph, true);
	//プレイヤーと魔女を表示する
	int animEle = m_animFrame / kAnimFrameNum;
	//画像のどこを切り取るか計算
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * static_cast<int>(m_dir);

	//サルと姫の描画
	DrawRectRotaGraph(kPlayerPosX, kPlayerPosY + m_charPosY,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		kPlayerScale,
		0.0,
		m_playerGraph, true, false);
	DrawRectRotaGraph(kPrincessPosX, kPrincessPosY + m_charPosY,
		0, kPrincessAnimFrame,
		kPrincessAnimFrame, kPrincessAnimFrame,
		kPrincessScale,
		0.0,
		m_princessGraph, true, false);
	//ステージを移動中じゃなかったら
	if (!m_isStaging)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_bossAlpha);
		//各ステージのボス表示
		DrawRectRotaGraph(Game::kScreenWidth / 2, kBossPosY,
			m_bossSrcX, kStageBossKind[m_bossSrcY] * kBossGraphSize,
			kBossGraphSize, kBossGraphSize,
			kBossScale, 0.0, m_enemyGraph, true, 0, 0);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//背景の表示
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kPlayScreenHeight, m_bgGraph, true);
	//選んでいるシーンのフレーム
	DrawExtendGraph(kFrameStartPosX, kFrameStartPosY, kFrameEndPosX, kFrameEndPosY,
		m_selectSceneFrame, true);
	//三角形の表示
	DrawSceneSrideTriangle();
	//ステージ名の表示
	DrawExtendGraph(kStageTitleStartPosX, kStageTitleStartPosY, kStageTitleEndPosX, kStageTitleEndPosY, m_backBoxGraph, true);
	//左上にストーリー表示
	DrawGraph(kStoryPosX, kStoryPosY, m_storyGraph[m_stageSelectNum], true);
	//左下に操作方法表示
	DrawGraph(kExplanationPosX, kExplanationPosY, m_buttonsUiGraph, true);
	//変えるアイテムがあったら
	if (m_isBuy)
	{
		//お店の左上にビックリマーク表示
		DrawRotaGraph(kExclamationMarkPosX, kExclamationMarkPosY, m_exclamationMarkRatio, 0.0, m_exclamationMarkGraph, true);
	}
	//今選んでいるステージの名前を表示する
	DrawStageName(m_stageSelectNum);
	//ショップの表示
	DrawRectExtendGraph(static_cast<int>(m_shopStartPosX), static_cast<int>(m_shopStartPosY),
		m_shopEndPosX, m_shopEndPosY, m_shopSrcX, m_shopSrcY, kShopGraphSize, kShopGraphSize, m_shopGraph, true);
	//メインシーンに移動時に出てくるボックス
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kPlayScreenHeight / 2,//座標
		static_cast<double>(m_boxRatio), static_cast<double>(m_boxAngle), m_boxGraph, true, 0, 0);
}

void SceneSelect::MoveScene(bool up)
{
	float speed;
	speed = kBgGraphSize / kBgMoveSpeed;
	if (up)
	{
		m_cutBgPosY -= static_cast<int>(speed);
		m_animFrame++;
		if (m_animFrame >= kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
		m_dir = Game::Dir::kDirUp;
	}
	else
	{
		m_cutBgPosY += static_cast<int>(speed);
		m_animFrame++;
		if (m_animFrame >= kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
		m_dir = Game::Dir::kDirDown;
	}
}

void SceneSelect::DrawSceneSrideTriangle()
{
	if (m_stageSelectNum < kMaxSceneNum && m_stageSelectNum < UserData::userClearStageNum)
	{
		m_shakeTrianglePosY = 0;
		//上の三角形表示
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleFrameScale, kHighTrianglePosY + kTriangleFrameScale + static_cast<int>(m_shakeTrianglePosY),
			Game::kScreenWidth / 2, kHighTrianglePosY + static_cast<int>(m_shakeTrianglePosY),
			Game::kScreenWidth / 2 - kTriangleFrameScale, kHighTrianglePosY + kTriangleFrameScale + static_cast<int>(m_shakeTrianglePosY),
			GetColor(0, 0, 0), true);
		DrawTriangle(Game::kScreenWidth / 2 + kTriangleScale, kHighTrianglePosY + kTriangleScale + kTriangleFrameShiftPosY + static_cast<int>(m_shakeTrianglePosY),
			Game::kScreenWidth / 2, kHighTrianglePosY + kTriangleFrameShiftPosY + static_cast<int>(m_shakeTrianglePosY),
			Game::kScreenWidth / 2 - kTriangleScale, kHighTrianglePosY + kTriangleScale + kTriangleFrameShiftPosY + static_cast<int>(m_shakeTrianglePosY),
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

bool SceneSelect::GetCanBuyItem()
{
	if (UserData::userGold >= m_playerItemPriceList[UserData::userAtkLevel])
	{
		return true;
	}
	else if (UserData::userGold >= m_playerItemPriceList[UserData::userDefLevel])
	{
		return true;
	}
	else if (UserData::userGold >= m_playerItemPriceList[UserData::userSpdLevel])
	{
		return true;
	}
	else if (UserData::userGold >= m_princessItemPriceList[UserData::userFireLevel])
	{
		return true;
	}
	else if (UserData::userGold >= m_princessItemPriceList[UserData::userWindLevel])
	{
		return true;
	}
	else
	{
		return false;
	}
}
void SceneSelect::SetSaveData(int num)
{
	if (num == 1)
	{
		std::string tempS;
		ofstream outputfile("./data/save/firstSaveData.txt");
		tempS = to_string(UserData::userMainLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userAtkLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userDefLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSpdLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userFireLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userWindLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
		outputfile.close();
	}
	if (num == 2)
	{
		std::string tempS;
		ofstream outputfile("./data/save/secondSaveData.txt");
		tempS = to_string(UserData::userMainLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userAtkLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userDefLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSpdLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userFireLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userWindLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
	if (num == 3)
	{
		std::string tempS;
		ofstream outputfile("./data/save/thirdSaveData.txt");
		tempS = to_string(UserData::userMainLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userAtkLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userDefLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSpdLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userFireLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userWindLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
	if (num == 4)
	{
		std::string tempS;
		ofstream outputfile("./data/save/fourthSaveData.txt");
		tempS = to_string(UserData::userMainLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userAtkLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userDefLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSpdLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userFireLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userWindLevel);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userGold);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userExp);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userClearStageNum);
		outputfile << tempS + "\n";
		tempS = to_string(UserData::userSaveDataNum);
		outputfile << tempS;
	}
}

void SceneSelect::DrawStageName(int num)
{
	int stringWidth;
	switch (num)
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

void SceneSelect::ZoomShop()
{
	m_shopStartPosX -= kShopStartPosXSpeed;
	m_shopStartPosY -= kShopStartPosYSpeed;
	m_shopEndPosX += kShopEndPosXSpeed;
	m_shopEndPosY += kShopEndPosYSpeed;

	if (m_shopStartPosX < kShopMaxSizePosX)
	{
		StopSoundMem(m_bgm);
		m_sceneManager.ChangeScene(std::make_shared<SceneShop>(m_sceneManager, m_dataManager, m_stageSelectNum), true);
	}
}

void SceneSelect::MoveCharcter()
{
	//ボスを消す
	m_bossAlpha = 0;
	//アニメーションを動かす
	m_animFrame++;
	if (m_animFrame >= kAnimFrameCycle)
	{
		m_animFrame = 0;
	}

	//キャラが上の端までついていなかったら
	if (!m_isCharArrTopEnd)
	{
		//上入力がされていたら
		if (m_isSceneUp)
		{
			//キャラを上に移動させる
			m_charPosY -= kCharMoveSpeed;
			m_dir = Game::Dir::kDirUp;
		}
	}
	//キャラが上の端についたら
	else
	{
		//キャラのポジションを下げる
		m_charPosY += kBgMoveSpeed;
		//キャラのポジションが下がりすぎないように
		if (m_charPosY > 0)
		{
			m_charPosY = 0;
		}
		m_cutBgPosY -= kBgMoveSpeed;
		//背景を切り取る制御
		if (m_cutBgPosY % kBgGraphSize == 0 && !m_isMoveMainScene)
		{
			m_isStaging = false;
			m_isCharArrTopEnd = false;
			m_dir = Game::Dir::kDirDown;
			m_animFrame = kAnimFrameNum;
			if (m_isChangeStage)
			{
				m_stageSelectNum++;
				m_isChangeStage = false;
			}
		}
	}
	//キャラが下の端までついていなかったら

		//下入力がされていたら
	if (!m_isSceneUp)
	{
		if (m_isCharMoveDown)
		{
			//キャラを下に移動させる
			m_charPosY += kCharMoveSpeed;
			m_dir = Game::Dir::kDirDown;
			if (m_charPosY > 0)
			{
				m_isCharMoveDown = false;
				m_animFrame = kAnimFrameNum;
			}
		}
	}

	//キャラが下の端についたら
	if (m_isCharArrBottomEnd)
	{
		//キャラのポジションをあげる
		m_charPosY -= kBgMoveSpeed;
		//キャラのポジションがあがりすぎないように
		if (m_charPosY + kPlayerPosY < kBgTopEndPos)
		{
			m_charPosY = kBgTopEndPos - kPlayerPosY;
		}
		m_cutBgPosY += kBgMoveSpeed;
		//背景を切り取る制御
		if (m_cutBgPosY % kBgGraphSize == 0 && !m_isMoveMainScene)
		{
			m_isCharMoveDown = true;
			m_isCharArrBottomEnd = false;
			m_dir = Game::Dir::kDirDown;

			if (m_isChangeStage)
			{
				m_stageSelectNum--;
				m_isChangeStage = false;
			}
		}
	}
}

void SceneSelect::ZoomMark()
{
	//ビックリマーク拡縮する
	m_exclamationMarkRatio += m_exclamationMarkZoomSpeed;
	//動きを反転させる処理
	if (m_exclamationMarkRatio > kZoomExclamationMarkLange)
	{
		m_exclamationMarkZoomSpeed *= -1;
	}
	if (1.0f > m_exclamationMarkRatio)
	{
		m_exclamationMarkZoomSpeed *= -1;
	}
}


