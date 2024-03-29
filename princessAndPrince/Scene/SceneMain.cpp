#include "DxLib.h"
#include "Pad.h"
#include "CircleCol.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "SceneSelect.h"
#include "MyString.h"

#include "MagicBase.h"
#include "Blood.h"
#include "TreasureBox.h"
#include "Player.h"
#include "Princess.h"
#include "Particle.h"
#include "Enemy.h"
#include "UI.h"
#include "UserData.h"
namespace
{
	//アイテムの最大数
	constexpr int kMaxItem = 512;
	//パーティクルの最大数
	constexpr int kMaxParticle = 512;
	//同時に存在する魔法の最大数
	constexpr int kMaxMagicValue = 32;
	//持てる血の量の最大数
	constexpr float kMaxBlood = 10;
	//エネミーが生まれるインターバル(仮処理)
	constexpr int kEnemyPopInterval = 50;
	//宝箱の同時に存在する最大数
	constexpr int kMaxTreasureBox = 16;
	//敵の最大数
	constexpr int kMaxEnemy = 256;
	//配列のサイズ
	constexpr int kArraySize = 81;
	//クリアした後の時間
	constexpr int kClearTime = 240;
	//倒すボスの数(仮実装)
	constexpr int kBossCount = 1;
	//聖剣モードのゲージの最大量
	constexpr float kMaxSpecialGauge = 100.0f;
	//パーティクルが出てくる間隔
	constexpr int kParticleInterval = 5;
	//ポーズ画面の項目数
	constexpr int kMaxPauseNum = 1;
	//クリア時のループが始まるまでの時間
	constexpr int kStartLoopTime = 60;
	//クリア時のゴールドのループが始まるまでの時間
	constexpr int kStartGoldLoopTime = 60 + kStartLoopTime;
	//出すパーティクルの数
	constexpr int kParticleVol = 30;
	//クリア時に止める時間
	constexpr int kResultTime = 30 + kClearTime;
	//クリア時の演出の時間
	constexpr int kDanceTime = 60 + kResultTime;
	//レベルアップの文字を出すY座標
	constexpr int kLevelUpPosY = 380;
	//レベルアップの文字が上がる限界Y座標
	constexpr int kLevelUpPosYMax = 330;
	//聖剣モードを始めるまでの時間
	constexpr int kSpecialModeStartTime = 30;
	//フォントの半分の大きさ
	constexpr int kFontHalfSize = 24;
	//READYを表示する時間
	constexpr int kReadyTime = 120;
	//ステージのサイズ
	constexpr int kStageGraphSize = 640;
	constexpr int kAllStageSize = kStageGraphSize * 7;
	//ボックスの回転する速さ
	constexpr float kBoxRotaSpeed = 0.15f;
	//ボックスの大きくなる速さ
	constexpr float kBoxZoomSpeed = 0.4f;
	//各ステージのボスの数
	constexpr int kBossVol[8] = { 1,1,2,2,3,1,1,14 };
	//黒いボックスの初期倍率
	constexpr float kBoxInitialRatio = 25.0f;
	//各ステージのチュートリアルの数
	constexpr int kTutorialVol[4] = { 2,0,3,1 };
	//各ステージのチュートリアルが何枚目から始まるか
	constexpr int kStartTutorialNum[4] = { 0,0,3,6 };
	//敵の種類
	constexpr int kEnemyKindVol = 6;
	//怒りゲージが貯まるようになるステージ数
	constexpr int kSpecialGaugeChargeStageNum = 3;
	//姫が殴られたときに貯まる怒りゲージの倍率
	constexpr float kSpecialGaugeChargeRate = 1.5f;
	//怒りゲージ発動時に減っていくゲージの量
	constexpr float kSpecialGaugeSubRate = 0.3f;
	//怒りゲージ発動時のパーティクルの情報
	constexpr int kSpecialModeParticleInfo[4] = { 1000,10,3,2 };
	//白いパーティクルの情報
	constexpr int kWhiteParticleInfo[4] = { 40,4,5,0 };
	//背景の横幅
	constexpr int kBgWidth = 965;
	//LEVELUPと表示する座標
	constexpr int kLevelupPosX = 530;
	//LEVELUPの色
	const int kLevelUpColor = GetColor(255, 255, 0);
	//チュートリアルを表示する座標
	constexpr int kTutorialPosX = 85;
	constexpr int kTutorialPosY = 290;
	//ポーズ画面のUI座標
	constexpr int kPauseUiPosX = 430;
	constexpr int kPauseUiPosY[2] = { 700,800 };
	//チュートリアルをどこまで表示するか
	constexpr int kTutorialStage = 4;
	//敵のポップする間隔
	constexpr int kEnemyPopFrame = 40;
	//ポーズした時の黒い背景の透明度
	constexpr int kPauseBlackBoxAlphe = 170;
	//怒りモード時に入った時の黒い背景の透明度
	constexpr int kAngryModeBlackBoxAlphe = 150;

}
SceneMain::SceneMain(SceneManager& sceneManager, DataManager& DataManager, int stageNum) :
	Scene(sceneManager, DataManager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0),
	m_isClearFlag(false),
	m_bossCount(kBossCount),
	m_killBossCount(0),
	m_clearTime(0),
	m_nextEnemyKind(0),
	m_nextEnemyPopTime(0),
	m_specialGauge(0),
	m_isSpecialMode(false),
	m_isPause(false),
	m_isStop(false),
	m_particleCount(0),
	m_isResult(false),
	m_pauseSelectNum(0),
	m_isExpLoop(false),
	m_isGoldLoop(false),
	m_isEnd(false),
	m_isClearString(false),
	m_startLoopTimeCount(0),
	m_attackSe(-1),
	m_weakAttackSe(-1),
	m_isBossFlag(false),
	m_danceMusic(-1),
	m_resultGold(-1),
	m_resultExp(-1),
	m_isWitchParticle(false),
	m_levelUpPosY(kLevelUpPosY),
	m_isUpLevel(false),
	m_isGameOver(false),
	m_isHalfExp(true),
	m_isHalfGold(true),
	m_lastSpace(true),
	m_selectScene(stageNum),
	m_pauseGraph(0),
	m_isShowReady(false),
	m_readyCount(0),
	m_isShowTutorial(true),
	m_isLastKey(false),
	m_isLastSe(true),
	m_boxAngle(0),
	m_boxRatio(kBoxInitialRatio),
	m_isMoveBox(true),
	m_startTutorialNum(0),
	m_isDeathTutorial(false),
	m_specialModeStartCount(0),
	m_pParticle(),
	m_expList(),
	m_isBackSelectScene(false),
	m_isGameOverString(false),
	m_isSelectKeyDown(false)
{
	//プレイヤーのグラフィックのロード
	m_playerHandle = m_dataManager.SearchGraph("playerGraph");
	//Playerのコンストラクタ
	m_pPlayer = new Player(this);
	//プレイヤーのメンバ変数にアクセス
	m_pPlayer->SetHandle(m_playerHandle);
	m_pPlayer->SetPlayerAngryGraph(m_dataManager.SearchGraph("angryMonkeyGraph"));
	m_pPlayer->SetAngryFireGraph(m_dataManager.SearchGraph("angryFireGraph"));
	m_pPlayer->SetTurnSe(m_dataManager.SearchSound("playerTurnSe"));
	m_pPlayer->SetDeathSe(m_dataManager.SearchSound("playerDeathSe"));
	m_pPlayer->SetStandUpSe(m_dataManager.SearchSound("playerStandUpSe"));
	//プリンセスのグラフィックのロード
	m_princessHandle = m_dataManager.SearchGraph("princessGraph");
	//Princessのコンストラクタ
	m_pPrincess = new Princess(this);
	//プリンセスのメンバ変数にアクセス
	m_pPrincess->SetHandle(m_princessHandle);
	m_pPrincess->SetMagicGraph(m_dataManager.SearchGraph("itemGraph"));
	m_pPrincess->SetArrowGraph(m_dataManager.SearchGraph("arrowGraph"));
	m_pPrincess->SetMagicArrowGraph(m_dataManager.SearchGraph("magicArrowGraph"));
	//敵のグラフィックのロード
	m_enemyHandle = m_dataManager.SearchGraph("enemyGraph");
	//背景のグラフィックのロード
	m_bgHandle = m_dataManager.SearchGraph("allFieldBgGraph");
	//アイテムのグラフィックのロード
	m_itemHandle = m_dataManager.SearchGraph("itemGraph");
	//ポーズを開いたときに出る画像のロード
	m_pauseGraph = m_dataManager.SearchGraph("PAUSEGraph");
	//ボタングラフィック
	m_buttonsGraph = m_dataManager.SearchGraph("buttonsGraph");
	//Uiを表示する背景
	m_uiBgGraph = m_dataManager.SearchGraph("mainUiBgGraph");
	//ぶつかったときの音のロード
	m_attackSe = m_dataManager.SearchSound("attackSe");
	//ダンスの音のロード
	m_danceMusic = m_dataManager.SearchSound("danceSe");
	//リザルトの効果音のロード
	m_resultGold = m_dataManager.SearchSound("resultGoldSe");
	m_resultExp = m_dataManager.SearchSound("resultExpSe");
	//マップの音楽のロード
	m_fieldBgm = m_dataManager.SearchSound("fieldBgm");
	//ボスが出てきた時の音楽のロード
	m_bossBgm = m_dataManager.SearchSound("bossBgm");
	//ダンスの前のSeのロード
	m_beforeDanceSe = m_dataManager.SearchSound("beforeDanceSe");
	//決定ボタンを押したときのSeのロード
	m_appSe = m_dataManager.SearchSound("approveSe");
	//カーソルを動かしたときの音のロード
	m_cursorSe = m_dataManager.SearchSound("cursorSe");
	//Bボタンを押したときの音
	m_cancelSe = m_dataManager.SearchSound("cancelSe");
	//コインを拾った時の音
	m_coinSe = m_dataManager.SearchSound("coinSe");
	//コインを拾った時の音
	m_expSe = m_dataManager.SearchSound("expSe");
	//風魔法を出したときの効果音
	m_windMagicSe = m_dataManager.SearchSound("windMagicSe");
	//炎魔法を出したときの効果音
	m_fireMagicSe = m_dataManager.SearchSound("fireMagicSe");
	//敵とプリンセスがぶつかった時の効果音
	m_hitPrincessSe = m_dataManager.SearchSound("hitPrincessSe");
	//聖剣モードを始めるときにならす効果音
	m_specialModeSe = m_dataManager.SearchSound("specialModeSe");
	//ポーションをとった時の効果音
	m_portionSe = m_dataManager.SearchSound("portionSe");
	//血を拾った時の効果音
	m_bloodSe = m_dataManager.SearchSound("bloodSe");
	//血を渡した時の効果音
	m_passBloodSe = m_dataManager.SearchSound("passBloodSe");
	//怒りモード時に表示する画像
	m_angryMarkGraph = m_dataManager.SearchGraph("angryMarkGraph");
	//ゲーム開始時に表示する画像
	m_readyGraph = m_dataManager.SearchGraph("READYGraph");
	//ゲームオーバー時の効果音
	m_princessDeathSe = m_dataManager.SearchSound("princessDeathSe");
	//黒いボックスの画像
	m_boxGraph = m_dataManager.SearchGraph("boxGraph");
	//チュートリアル画像
	m_tutorialGraph[0] = m_dataManager.SearchGraph("tutorialGraph1");
	m_tutorialGraph[1] = m_dataManager.SearchGraph("tutorialGraph2");
	m_tutorialGraph[2] = m_dataManager.SearchGraph("tutorialGraph3");
	m_tutorialGraph[3] = m_dataManager.SearchGraph("tutorialGraph4");
	m_tutorialGraph[4] = m_dataManager.SearchGraph("tutorialGraph5");
	m_tutorialGraph[5] = m_dataManager.SearchGraph("tutorialGraph6");
	m_tutorialGraph[6] = m_dataManager.SearchGraph("tutorialGraph7");

	//敵の最大数を設定
	m_pEnemy.resize(kMaxEnemy);
	//アイテムの最大数を設定
	m_pItem.resize(kMaxItem);
	//魔法の最大数を設定
	m_pMagic.resize(kMaxMagicValue);
	//宝箱の最大数を設定
	m_pTreasure.resize(kMaxTreasureBox);
	//パーティクルの最大数を設定
	m_pParticleArray.resize(kMaxParticle);
	//UIのコンストラクタ
	m_pUi = new UI(m_pPlayer, m_pPrincess, this);
	//魔法のグラフィックを設定
	m_pUi->SetMagicGraph(m_itemHandle);
	//魔法のUiの背景を設定する
	m_pUi->SetMagicBgGraph(m_dataManager.SearchGraph("magicUiBgGraph"));
	//怒りゲージのUiを設定する
	m_pUi->SetAngryGaugeGraph(m_dataManager.SearchGraph("angryGaugeUiGraph"));
	//怒りゲージがたまった時のUIを設定する
	m_pUi->SetAngryButtonGraph(m_dataManager.SearchGraph("angryButtonMarkGraph"));
	//怒りゲージが使えない時のUIを設定する
	m_pUi->SetStoneAngryGaugeGraph(m_dataManager.SearchGraph("stoneAngryGaugeGraph"));
	//選ばれたシーンによって表示するチュートリアルを設定する
	if (m_selectScene < kTutorialStage)
	{
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
	}
	//5ステージ目からはチュートリアルを表示しない
	else
	{
		m_tutorialNum = 0;
		m_nowShowTutorialNum = 0;
	}
	switch (m_selectScene)
	{
	case 0://1ステージ目のチュートリアル情報
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
		break;
	case 1:
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_nowShowTutorialNum = 0;
		break;
	case 2:
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
		break;
	case 3:
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
		break;
	default:
		m_tutorialNum = 0;
		m_nowShowTutorialNum = 0;
		break;
	}
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	{
		//表示するチュートリアルがなかったら
		if (m_tutorialNum == 0)
		{
			m_isShowTutorial = false;
			m_isShowReady = true;
		}
		//出てくる敵の情報を設定する
		m_bossCount = kBossVol[m_selectScene];
		SetEnemyInfo(m_selectScene);
		//ファイルを開く
		std::ifstream ifs("./data/expLevel.txt");
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
			m_expList[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	m_pPlayer->Init();
	m_pPrincess->Init();
}


void SceneMain::Update(Pad& pad)
{
	//ポーズや演出時など以外の場合動かす

	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//デバッグ用
#ifdef _DEBUG
	if (m_input.Buttons[XINPUT_BUTTON_LEFT_THUMB] || CheckHitKey(KEY_INPUT_0))
	{
		CountKillBoss();
	}
#endif 
	if (m_boxRatio > 0)
	{
		m_boxAngle -= kBoxRotaSpeed;
		m_boxRatio -= kBoxZoomSpeed;
	}
	else if (m_isMoveBox)
	{
		m_boxRatio = 0;
		m_isMoveBox = false;
	}
	//最初に死んだときにチュートリアルを表示する
	if (m_isDeathTutorial)
	{
		if (CheckHitKey(KEY_INPUT_RETURN) || m_input.Buttons[XINPUT_BUTTON_A])
		{
			m_isDeathTutorial = false;
			m_isStop = false;
			m_isLastKey = true;
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
		}
	}
	//シーン移動の演出が終わったら
	if (!m_isMoveBox && !m_isDeathTutorial)
	{
		//エンターキーを押したら次のチュートリアルに移行する
		if (CheckHitKey(KEY_INPUT_RETURN) && !m_isLastKey && m_isShowTutorial ||
			m_input.Buttons[XINPUT_BUTTON_A] && !m_isLastKey && m_isShowTutorial)
		{
			m_nowShowTutorialNum++;
			m_isLastKey = true;
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
			//すべてのチュートリアルを表示したら
			if (m_startTutorialNum + m_tutorialNum <= m_nowShowTutorialNum)
			{
				//チュートリアルを終了する
				m_isShowTutorial = false;
				//READYを表示する
				m_isShowReady = true;
			}
		}
		//エンターキーが連続で入力されないように
		else if (!CheckHitKey(KEY_INPUT_RETURN) && !m_input.Buttons[XINPUT_BUTTON_A])
		{
			m_isLastKey = false;
		}
	}
	//READYが表示されている間
	if (m_isShowReady)
	{
		m_readyCount++;
		if (m_readyCount > kReadyTime)
		{
			m_isShowReady = false;
		}
	}
	if (!m_isPause && !m_isShowReady && !m_isShowTutorial && m_boxRatio == 0)
	{
		//音楽再生
		if (m_isMusicFlag && !m_isBossFlag)
		{
			PlaySoundMem(m_fieldBgm, DX_PLAYTYPE_LOOP);
			m_isMusicFlag = false;
		}
		else if (m_isMusicFlag && m_isBossFlag)
		{
			StopSoundMem(m_fieldBgm);
			PlaySoundMem(m_bossBgm, DX_PLAYTYPE_LOOP);
			m_isMusicFlag = false;
		}
		//ポーズボタンが押されたら
		if (!m_pPrincess->IsDeath())
		{
			if (m_input.Buttons[XINPUT_BUTTON_START] || CheckHitKey(KEY_INPUT_P))
			{
				m_isPause = true;
			}
		}
		//シーン移動
		if (m_pPrincess->IsDeath())
		{
			//音楽の再生を止める
			StopSoundFile();
			if (!m_isResult)
			{
				PlaySoundMem(m_princessDeathSe, DX_PLAYTYPE_BACK);
			}
			for (auto& enemy : m_pEnemy)
			{
				if (enemy)
				{
					if (enemy->m_nowState != Game::State::kDelete)
						//エネミーの移動を止める
						enemy->m_nowState = Game::State::kStop;
				}
			}
			StopSoundMem(m_fieldBgm);
			StopSoundMem(m_bossBgm);
			m_pPlayer->m_nowState = Game::State::kStop;
			m_isGameOver = true;
			//リザルト画面に移行する
			m_isResult = true;
		}
		//リザルト状態のときは止まるようにする
		if (!m_isStop)
		{
			//エネミーのスタックがなくなるまで回す
			if (!m_popEnemyList.empty())
			{
				//ボスを倒したら敵を出てこないようにする
				if (m_killBossCount < m_bossCount && !m_pPrincess->IsDeath())
				{
					m_enemyPopTimeCount++;
				}
				//設定した時間になったら
				if (m_enemyPopTimeCount > m_nextEnemyPopTime * kEnemyPopFrame)
				{
					//カウントを初期化
					m_enemyPopTimeCount = 0;
					//エネミーを出現させる
					CreateEnemy(m_nextEnemyKind);
					//出てくる敵がボスだったら音楽を変える
					if (m_nextEnemyKind > kEnemyKindVol && !m_isBossFlag)
					{
						m_isBossFlag = true;
						m_isMusicFlag = true;
					}
					//次に出てくるエネミーの情報を入れる
					popEnemy temp = m_popEnemyList.top();
					m_nextEnemyPopTime = temp.popTime;
					m_nextEnemyKind = temp.enemyKind;
					m_popEnemyList.pop();
				}
			}
			m_pPlayer->Update();
			m_pPrincess->Update();
			m_pUi->Update();
			//エネミーのアップデート
			for (auto& enemy : m_pEnemy)
			{
				if (enemy)
				{
					//状態がkDeleteじゃない場合のみ動く
					if (enemy->m_nowState != Game::State::kDelete)
					{
						enemy->Update();
						//プレイヤーとエネミーがぶつかったとき
						if (m_pPlayer->m_nowState != Game::State::kDelete &&//プレイヤーが死んでいないときに
							IsCollision(m_pPlayer->GetColCircle(), enemy->GetColCircle()) &&//プレイヤーとエネミーがぶつかったら
							enemy->m_nowState != Game::State::kHitPlayer && enemy->m_nowState != Game::State::kStop)//エネミーがkDeleteじゃないときのみ
						{
							//エネミーのダメージ処理を行う
							enemy->HitPlayer(*m_pPlayer, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
							//プレイヤーのダメージ処理を行う
							m_pPlayer->HitEnemy(*enemy, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
							//スペシャルゲージがマックスじゃなかったらゲージを上昇させる
							if (!m_isSpecialMode && m_selectScene >= kSpecialGaugeChargeStageNum)
							{
								//敵の攻撃力に応じてゲージを上昇させる
								AddSpecialGauge(enemy->GetAtk());
							}
							//エネミーの状態を推移させる
							enemy->m_nowState = Game::State::kHitPlayer;
							PlaySoundMem(m_attackSe, DX_PLAYTYPE_BACK);
						}
						//魔女とエネミーがぶつかったとき
						if (IsCollision(m_pPrincess->GetColCircle(), enemy->GetColCircle()) &&
							m_pPrincess->m_nowState != Game::State::kDelete && enemy->m_nowState != Game::State::kStop)
						{
							//魔女のダメージ処理を行う,エネミーのノックバックを行う
							m_pPrincess->HitEnemy(*enemy);
							PlaySoundMem(m_hitPrincessSe, DX_PLAYTYPE_BACK);
							if (!m_isSpecialMode && m_selectScene > kSpecialGaugeChargeStageNum)
							{
								//姫が殴られたときは貯まるゲージ量を少し増やす
								AddSpecialGauge(enemy->GetAtk() * kSpecialGaugeChargeRate);
							}
						}
						//魔法の処理
						for (auto& magic : m_pMagic)
						{

							if (magic &&//magicがnullじゃない場合
								IsCollision(magic->GetCircleCol(), enemy->GetColCircle()) &&//MagicとEnemyがぶつかったら
								enemy->m_nowState != Game::State::kHitMagic)//状態がkHitMagicじゃなかったら
							{
								//エネミーの状態を変化させる
								enemy->m_nowState = Game::State::kHitMagic;
								//魔法のダメージ処理を行う
								enemy->HitMagic(magic);
							}
						}
					}
				}
			}
		}

		for (auto& magic : m_pMagic)
		{
			//魔法がnullじゃなく
			if (magic)
			{
				//状態がデリートになっていないとき
				if (magic->m_nowState != Game::State::kDelete)
				{
					//魔法の更新処理を行う
					magic->Update();
				}
				else//状態がデリートになったとき
				{
					//魔法のポインタを消す
					magic = nullptr;

				}
			}
		}

		//魔女とPlayerがぶつかった時
		if (IsCollision(m_pPlayer->GetColCircle(), m_pPrincess->GetColCircle()) &&
			!m_pPrincess->IsMagic())
		{
			m_pPlayer->GiveBlood(m_pPrincess);
		}
		for (auto& item : m_pItem)
		{
			//itemがnullじゃない場合
			if (item)
			{
				//アイテムが存在している場合
				if (item->m_nowState != Game::State::kDelete)
				{
					item->Update();
					//アイテムとプレイヤーが接触し、アイテムの状態がkNoneじゃなかったら
					if (IsCollision(m_pPlayer->GetColCircle(), item->GetColCircle()) &&
						item->m_nowState != Game::State::kNone)
					{
						m_pPlayer->PickUpItem(item);
					}
				}
			}
		}
		//宝箱
		for (auto& treasure : m_pTreasure)
		{
			//treasureがnullじゃない場合
			if (treasure)
			{
				//アイテムが存在している場合
				if (treasure->m_nowState != Game::State::kDelete)
				{
					treasure->Update();
					//プレイヤーとぶつかったら
					if (IsCollision(m_pPlayer->GetColCircle(), treasure->GetColCircle()))
					{
						PlaySoundMem(m_attackSe, DX_PLAYTYPE_BACK);
						m_pPlayer->HitTreasure(treasure);
						treasure->HitPlayer(m_pPlayer);
					}
					//魔法ととぶつかったら
					for (auto& magic : m_pMagic)
					{
						if (magic &&//magicがnullじゃなかったら
							IsCollision(magic->GetCircleCol(), treasure->GetColCircle()) &&
							treasure->m_nowState != Game::State::kHitMagic)
						{
							treasure->HitMagic();
						}
					}

				}
				else//アイテムが存在しない状態になったら
				{
					treasure = nullptr;
				}
			}
		}
		//パーティクル
		for (auto& particle : m_pParticleArray)
		{
			//itemがnullじゃない場合
			if (particle)
			{
				//アイテムが存在している場合
				if (particle->GetIsExist())
				{
					particle->Update();
				}
				else//アイテムが存在しない状態になったら
				{
					particle = nullptr;
				}
			}
		}
		//怒りモードが発動可能だったら
		if (m_specialGauge >= kMaxSpecialGauge && m_pPrincess->m_nowState != Game::State::kDelete)
		{
			if (m_lastSpace && !m_isStop)
			{
				if (m_input.Buttons[XINPUT_BUTTON_Y] || CheckHitKey(KEY_INPUT_SPACE))
				{
					m_isSpecialMode = true;
					m_isStop = true;
					m_specialModeStartCount = 0;
					m_pPlayer->SetSpecialMode();
					m_pPrincess->StartSpecialMode();
					m_lastSpace = false;
					PlaySoundMem(m_specialModeSe, DX_PLAYTYPE_BACK);
				}
			}
		}
		//怒りモードの演出を入れる
		if (m_isSpecialMode && m_isStop)
		{
			m_specialModeStartCount++;
			if (m_specialModeStartCount > kSpecialModeStartTime)
			{
				m_isStop = false;
				m_pPlayer->StartSpecialMode();
				m_lastSpace = true;
			}
		}
		//怒りモード発動中だったら
		if (m_isSpecialMode && !m_isStop)
		{
			//少しずつゲージを減らしていく
			m_specialGauge -= kSpecialGaugeSubRate;
			//カウントを進める
			m_particleCount++;
			//背景に表示するパーティクルを生成する
			if (m_particleCount > kParticleInterval)
			{
				m_particleCount = 0;
				m_pParticle = new Particle(m_pPrincess->GetPos(), static_cast<float>(kSpecialModeParticleInfo[0]), static_cast<float>(kSpecialModeParticleInfo[1]), kSpecialModeParticleInfo[2], kSpecialModeParticleInfo[3]);
				AddParticle(m_pParticle);
			}
			//ゲージが0になったら
			if (m_specialGauge < 0)
			{
				//聖剣モードを終わらせる
				m_isSpecialMode = false;
				m_specialGauge = 0;
			}
		}
	}
	//ポーズ中の処理
	else if (m_isPause)
	{
		//Bボタンを押したらメインシーンに戻る
		if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
		{
			PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
			m_isPause = false;
		}
		//Aボタンを押したら選択している項目に応じて処理を行う
		if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
		{
			switch (m_pauseSelectNum)
			{
			case 0:
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				m_isPause = false;
				break;
			case 1:
				m_isPause = false;
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				StopSoundMem(m_fieldBgm);
				StopSoundMem(m_bossBgm);
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectScene), true);
				return;
				break;
			}
		}
		if (!m_isSelectKeyDown)
		{
			//上キーが押されたら
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_pauseSelectNum--;
				if (m_pauseSelectNum < 0)
				{
					m_pauseSelectNum = kMaxPauseNum;
				}
				m_isSelectKeyDown = true;
			}
			//下キーが入力されたら
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_pauseSelectNum++;
				if (m_pauseSelectNum > kMaxPauseNum)
				{
					m_pauseSelectNum = 0;
				}
				m_isSelectKeyDown = true;
			}
		}
		//上キーと下キーが離されたら
		if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
		{
			m_isSelectKeyDown = false;
		}

	}
	///////////////////////////////////////////////////////////////////////////////
	//クリア判定
	if (m_killBossCount >= m_bossCount)
	{
		//時間をカウントし続ける
		m_clearTime++;
		StopSoundMem(m_fieldBgm);
		StopSoundMem(m_bossBgm);
	}
	//アイテムをとる時間をとる
	if (m_clearTime > kClearTime)
	{
		//聖剣モードを止める
		m_isSpecialMode = false;
		//プレイヤーとエネミーの動きを止める
		m_isStop = true;
	}
	//動きが止まって少ししたら
	if (m_clearTime > kResultTime && m_clearTime < kDanceTime)
	{
		if (!CheckSoundMem(m_beforeDanceSe))
		{
			PlaySoundMem(m_beforeDanceSe, DX_PLAYTYPE_BACK);
		}

		//プリンセスの位置に白いエフェクトを出す
		if (!m_isWitchParticle)
		{
			for (int i = 0; i < kParticleVol; i++)
			{
				m_pParticle = new Particle(m_pPrincess->GetPos(), //パーティクルの情報を入れる
					static_cast<float>(kWhiteParticleInfo[0]), static_cast<float>(kWhiteParticleInfo[1]), kWhiteParticleInfo[2], kWhiteParticleInfo[3]);
				AddParticle(m_pParticle);
			}
			m_isWitchParticle = true;
		}


		//マップ上にいる敵を消す
		for (const auto& enemy : m_pEnemy)
		{
			if (enemy && enemy->m_nowState != Game::State::kDelete)
			{
				enemy->m_nowState = Game::State::kDelete;
				Vec2 temp;
				//消えるときにエフェクトを出す
				temp = enemy->GetPos();
				//白いエフェクトを出す
				for (int i = 0; i < kParticleVol; i++)
				{
					m_pParticle = new Particle(m_pPrincess->GetPos(), //パーティクルの情報を入れる
						static_cast<float>(kWhiteParticleInfo[0]), static_cast<float>(kWhiteParticleInfo[1]), kWhiteParticleInfo[2], kWhiteParticleInfo[3]);;
					AddParticle(m_pParticle);
				}
			}
		}
		//プレイヤーを前に向ける
		m_pPlayer->TurnFront();
		m_pPrincess->TransStone();
		PlaySoundMem(m_danceMusic, DX_PLAYTYPE_BACK);

	}
	//敵が消えた後に少しだけ間を開けて
	if (m_clearTime > kDanceTime && !m_isResult)
	{
		//プレイヤーのクリア時の行動を入れる
		m_pPlayer->ClearDance();
	}
	if (m_isResult)
	{
		m_isClearFlag = true;
		int temp;
		if (m_isExpLoop)
		{
			m_startLoopTimeCount++;
			//ゲームオーバーだったら半分減らす
			if (m_isHalfExp && m_isGameOver)
			{
				m_pPlayer->SubExpGameOver();
				m_isHalfExp = false;
			}
			//経験値が0になるまでまわす
			if (m_pPlayer->GetExp() != 0 &&
				m_startLoopTimeCount > kStartLoopTime)//ループが始まるまで少し時間をとる
			{
				//減らす量を決める
				temp = GetDigits(m_pPlayer->GetExp());
				if (temp > m_expList[UserData::userMainLevel] - UserData::userExp)
				{
					temp = m_expList[UserData::userMainLevel] - UserData::userExp;
				}
				m_pPlayer->SubExp(temp);
				UserData::userExp += temp;
				if (UserData::userExp >= m_expList[UserData::userMainLevel])
				{
					UserData::userMainLevel++;
					m_isUpLevel = true;
				}
			}
			else if (m_pPlayer->GetExp() == 0)
			{
				if (!CheckSoundMem(m_resultExp) && !m_isGoldLoop)
				{
					PlaySoundMem(m_resultExp, DX_PLAYTYPE_BACK);
				}
				//経験値のループが終わったらゴールドのループに行く
				m_isExpLoop = false;
				m_isGoldLoop = true;
				m_pUi->ShowGold();
			}
		}
		if (m_isGoldLoop)
		{

			if (m_isHalfGold && m_isGameOver)
			{
				m_pPlayer->SubGoldGameOver();
				m_isHalfGold = false;
			}
			if (m_pPlayer->GetGold() != 0 &&
				m_startLoopTimeCount > kStartGoldLoopTime)
			{
				//減らす量を決める
				temp = GetDigits(m_pPlayer->GetGold());
				m_pPlayer->SubGold(temp);
				UserData::userGold += temp;
			}
			else if (m_pPlayer->GetGold() == 0)
			{
				if (!CheckSoundMem(m_resultGold) && !m_isEnd)
					PlaySoundMem(m_resultGold, DX_PLAYTYPE_BACK);
				m_isEnd = true;
				m_pUi->ShowLeaveButton();
			}
		}
		if (m_isEnd && m_input.Buttons[XINPUT_BUTTON_A] || m_isEnd && CheckHitKey(KEY_INPUT_RETURN))
		{
			if (m_isLastSe)
			{
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				m_isLastSe = false;
			}
			//まだクリアしてないステージだったらクリアしたことを保存する
			if (m_selectScene >= UserData::userClearStageNum && m_pPrincess->m_nowState != Game::State::kDelete)
			{
				UserData::userClearStageNum = m_selectScene + 1;
			}
			m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectScene), true);
			return;
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	//クリア時にプレイヤーがレベルアップしていたら
	if (m_isUpLevel)
	{
		if (m_levelUpPosY >= kLevelUpPosYMax)
		{
			m_levelUpPosY--;
		}

	}
}

void SceneMain::Draw()
{

	//プレイ画面の背景
	DrawRectExtendGraph(0, 0, kBgWidth, Game::kPlayScreenHeight, 0, kAllStageSize - (kStageGraphSize * m_selectScene), kStageGraphSize, kStageGraphSize, m_bgHandle, true);
	if (m_isSpecialMode)
	{
		//怒りモードに入ったら背景を暗くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAngryModeBlackBoxAlphe);
		DrawBox(0, 0, kBgWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	for (auto& magic : m_pMagic)
	{
		//magicがnullじゃない場合
		if (magic)
		{
			//アイテムが存在している場合
			if (magic->m_nowState != Game::State::kDelete)
			{
				magic->Draw();

			}
		}
	}
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState != Game::State::kDelete &&
				item->m_nowState != Game::State::kNone)
			{
				item->Draw();
			}
		}
	}
	for (auto& treasure : m_pTreasure)
	{
		//treasureがnullじゃない場合
		if (treasure)
		{
			//アイテムが存在している場合
			if (treasure->m_nowState != Game::State::kDelete)
			{
				treasure->Draw();
			}
		}
	}
	for (const auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			enemy->Draw();

		}
	}
	m_pPrincess->Draw();
	m_pPlayer->Draw();
	for (auto& particle : m_pParticleArray)
	{
		//particleがnullじゃない場合
		if (particle)
		{
			//アイテムが存在している場合
			if (particle->GetIsExist())
			{
				particle->Draw();
			}
		}
	}
	m_pUi->Draw();
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState == Game::State::kNone)
			{
				item->Draw();
			}
		}
	}
	//ClearDanceが終わったら
	if (m_isClearString)
	{
		m_pUi->SceneClearUI();
	}
	if (m_isGameOver)
	{
		m_pUi->GameOverUI();
	}
	//レベルアップしていたらレベルアップと表示する
	if (m_isUpLevel)
	{
		DrawString(kLevelupPosX, m_levelUpPosY, "LEVEL UP", kLevelUpColor);
	}
	//ポーズ画面を開いていたら
	if (m_isPause)
	{
		//黒いボックスを表示する
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kPauseBlackBoxAlphe);
		DrawBox(0, 0, kBgWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//白い文字を表示する
		int stringWidth = GetStringLength("つづける") * kFontHalfSize / 2;
		DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[0],
			"つづける", GetColor(255, 255, 255));
		stringWidth = GetStringLength("やめる") * kFontHalfSize / 2;
		DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[1],
			"やめる", GetColor(255, 255, 255));
		DrawGraph(0, 0, m_pauseGraph, true);

		//選ばれている文字を赤く表示する
		switch (m_pauseSelectNum)
		{
		case 0:
			stringWidth = GetStringLength("つづける") * kFontHalfSize / 2;
			DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[0],
				"つづける", GetColor(255, 0, 0));
			break;
		case 1:
			stringWidth = GetStringLength("やめる") * kFontHalfSize / 2;
			DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[1],
				"やめる", GetColor(255, 0, 0));
			break;

		}
	}
	//ステージ開始時にREADYを表示する
	if (m_isShowReady)
	{
		DrawGraph(0, 0, m_readyGraph, true);
	}
	//ステージの最初にチュートリアルが表示する
	if (m_isShowTutorial)
	{
		DrawGraph(kTutorialPosX, kTutorialPosY, m_tutorialGraph[m_nowShowTutorialNum], true);
	}
	//最初に死んだときのチュートリアルを表示する
	if (m_isDeathTutorial)
	{
		DrawGraph(kTutorialPosX, kTutorialPosY, m_tutorialGraph[2], true);
	}
	//回転する黒いボックスを表示する
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kPlayScreenHeight / 2,//座標
		m_boxRatio, m_boxAngle, m_boxGraph, true, 0, 0);
}

bool SceneMain::AddItem(std::shared_ptr<ItemBase> pItem)
{
	//アイテムの配列の長さだけfor文を回す
	for (int i = 0; i < m_pItem.size(); i++)
	{
		//使用中なら次のチェックへ
		if (m_pItem[i]) continue;
		//ここに来たということはm_pShot[i] == nullptr
		m_pItem[i] = pItem;
		//登録したら終了
		return true;
	}
	//ここに来た、ということはm_pShotにポインタを登録できなかった
	return false;
}

bool SceneMain::CreateEnemy(int enemyKind)
{
	//アイテムの配列の長さだけfor文を回す
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		//使用中なら次のチェックへ
		if (m_pEnemy[i]) continue;
		//ここに来たということはm_pShot[i] == nullptr
		m_pEnemy[i] = make_shared<Enemy>(this);
		m_pEnemy[i]->SetHandle(m_enemyHandle);
		m_pEnemy[i]->Init(enemyKind);
		m_pEnemy[i]->SetItemHandle(m_itemHandle);
		//登録したら終了
		return true;
	}
	//ここに来た、ということはm_pShotにポインタを登録できなかった
	return false;
}

void SceneMain::AddSpecialGauge(float gauge)
{
	float addGauge;
	addGauge = gauge - m_pPlayer->GetDef();
	//最低でも１は増えるようにする
	if (addGauge < 0)
	{
		addGauge = 1;
	}
	m_specialGauge += addGauge;
	//限界値を超えたら
	if (m_specialGauge > kMaxSpecialGauge)
	{
		//現在の値を限界値にする
		m_specialGauge = kMaxSpecialGauge;
	}
}

int SceneMain::GetDigits(int num)
{
	if (num >= 10000)
	{
		return 10000;
	}
	else if (num >= 1000)
	{
		return 1000;
	}
	else if (num >= 100)
	{
		return 100;
	}
	else if (num >= 10)
	{
		return 10;
	}
	else if (num >= 1)
	{
		return 1;
	}
	else if (num <= 0)
	{
		return 0;
	}
	else
	{
		return 0;
	}
}
int SceneMain::GetNextExp()
{
	return m_expList[UserData::userMainLevel] - UserData::userExp;
}

void SceneMain::SetEnemyInfo(int stageNum)
{
	//ファイルを開く
	std::ifstream ifs;
	if (stageNum == 0)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo1.txt");
	}
	else if (stageNum == 1)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo2.txt");
	}
	else if (stageNum == 2)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo3.txt");
	}
	else if (stageNum == 3)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo4.txt");
	}
	else if (stageNum == 4)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo5.txt");
	}
	else if (stageNum == 5)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo6.txt");
	}
	else if (stageNum == 6)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo7.txt");
	}
	else if (stageNum == 7)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo8.txt");
	}
	//帰ってきた値を返す配列
	vector<string> tempS;
	//配列を作成
	char str[kArraySize];
	//成功したら一行ずつ読み込む
	while (ifs.getline(str, kArraySize))
	{
		//分割
		tempS = MyString::split(str, ",");
		popEnemy tempEnemy;
		tempEnemy.enemyKind = std::stoi(tempS[0]);
		tempEnemy.popTime = std::stoi(tempS[1]);
		m_popEnemyList.push(tempEnemy);
	}
	//最初に出てくるエネミーの情報を入れる
	popEnemy firstEnemyInfo = m_popEnemyList.top();
	m_nextEnemyPopTime = firstEnemyInfo.popTime;
	m_nextEnemyKind = firstEnemyInfo.enemyKind;
	//スタックの中の読み取った情報を消す
	m_popEnemyList.pop();
	//ファイルを閉じる
	ifs.close();

}
bool SceneMain::AddMagic(MagicBase* pMagic)
{
	//魔法の配列の長さ
	for (int i = 0; i < m_pMagic.size(); i++)
	{
		//使用中なら次のチェックへ
		if (m_pMagic[i]) continue;
		//ここに来たということはm_pShot[i] == nullptr
		m_pMagic[i] = pMagic;
		//登録したら終了
		return true;
	}
	//ここに来た、ということはm_pShotにポインタを登録できなかった
	return false;
}

bool SceneMain::AddTreasure(TreasureBox* pTreasure)
{
	//宝箱の配列の長さ
	for (int i = 0; i < m_pTreasure.size(); i++)
	{
		//使用中なら次のチェックへ
		if (m_pTreasure[i]) continue;
		//ここに来たということはm_pShot[i] == nullptr
		m_pTreasure[i] = pTreasure;
		//登録したら終了
		return true;
	}
	//ここに来た、ということはm_pShotにポインタを登録できなかった
	return false;
}

bool SceneMain::AddParticle(Particle* pParticle)
{
	//パーティクルの配列の長さ
	for (int i = 0; i < m_pParticleArray.size(); i++)
	{
		//使用中なら次のチェックへ
		if (m_pParticleArray[i]) continue;
		//ここに来たということはm_pParticle[i] == nullptr
		m_pParticleArray[i] = pParticle;
		//登録したら終了
		return true;
	}
	//ここに来た、ということはm_pShotにポインタを登録できなかった
	return false;
}

bool SceneMain::IsCollision(CircleCol col1, CircleCol col2)
{
	if (col1.IsCollsion(col2))
	{
		return true;
	}
	return false;
}