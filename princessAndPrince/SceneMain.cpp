#include <DxLib.h>
#include <cassert>
#include "Pad.h"
#include "CircleCol.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"
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
	constexpr int kClearTime = 360;
	//倒すボスの数(仮実装)
	constexpr int kBossCount = 1;
	//聖剣モードのゲージの最大量
	constexpr float kMaxSpecialGauge = 100.0f;
	//パーティクルが出てくる間隔
	constexpr int kParticleInterval = 5;
	//ポーズ画面の項目数
	constexpr int kMaxPauseNum = 1;
}
SceneMain::SceneMain(SceneManager& manager, int stageNum) :
	Scene(manager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0),
	m_isClearFlag(false),
	m_bossCount(kBossCount),
	m_killBossCount(0),
	m_clearTime(0),
	m_nextEnemyKind(0),
	m_nextEnemyPopTime(0),
	m_specialGauge(0.1f),
	m_isSpecialMode(false),
	m_isPause(false),
	m_isStop(false),
	m_particleCount(0),
	m_isEnd(false),
	m_pauseSelectNum(0)
{
	//プレイヤーのグラフィックのロード
	m_playerHandle = LoadGraph("data/image/Monkey.png");
	//Playerのコンストラクタ
	m_pPlayer = new Player(this);
	//プレイヤーのメンバ変数にアクセス
	m_pPlayer->SetHandle(m_playerHandle);
	//プリンセスのグラフィックのロード
	m_princessHandle = LoadGraph("data/image/Monkey.png");
	//Princessのコンストラクタ
	m_pPrincess = new Princess(this);
	//プリンセスのメンバ変数にアクセス
	m_pPrincess->SetHandle(m_princessHandle);
	//敵のグラフィックのロード
	m_enemyHandle = LoadGraph("data/image/Enemy.png");
	//背景のグラフィックのロード
	m_bgHandle = LoadGraph("data/image/_bg.png");
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
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	{
		//ファイルを開く
		std::ifstream ifs("./data/EnemyInfo.txt");
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
			tempEnemy.popTime = std::stof(tempS[1]);
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
	m_pPlayer->Init();
	m_pPrincess->Init();
}


void SceneMain::Update(Pad& pad)
{
	//音楽再生
//	if (m_flag)
//	{
//		PlaySoundFile("data/sound/mainBgm.mp3", DX_PLAYTYPE_LOOP);
//		m_flag = false;
//	}
//ポーズや演出時など以外の場合動かす

	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);

	if (!m_isPause && !m_isStop)
	{
		//ポーズボタンが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_START])
		{
			m_isPause = true;
		}
		//シーン移動
		if (m_pPrincess->IsDeath())
		{
			//音楽の再生を止める
			StopSoundFile();
			//タイトルシーンに移行する
			m_manager.ChangeScene(std::make_shared<SceneTitle>(m_manager));

			return;
		}
		//Enemyの数だけ回す後で仕様変更
		//エネミーのスタックがなくなるまで回す
		if (!m_popEnemyList.empty())
		{
			m_enemyPopTimeCount++;
			if (m_enemyPopTimeCount > m_nextEnemyPopTime * 20)
			{
				//カウントを初期化
				m_enemyPopTimeCount = 0;
				//エネミーを出現させる
				CreateEnemy(m_nextEnemyKind);
				//次に出てくるエネミーの情報を入れる
				popEnemy temp = m_popEnemyList.top();
				m_enemyPopTimeCount = temp.popTime;
				m_nextEnemyKind = temp.enemyKind;
				m_popEnemyList.pop();
			}
		}
		m_pPlayer->Update();
		m_pPrincess->Update();
		bool isLeaveFlag = true;
		//エネミーのアップデート
		for (auto& enemy : m_pEnemy)
		{
			if (enemy)
			{
				//状態がkDeleteじゃない場合のみ動く
				if (enemy->m_nowState != Game::kDelete)
				{
					enemy->Update();
					//プレイヤーとエネミーがぶつかったとき
					if (m_pPlayer->m_nowState != Game::kDelete &&//プレイヤーが死んでいないときに
						IsCollision(m_pPlayer->GetColCircle(), enemy->GetColCircle()) &&//プレイヤーとエネミーがぶつかったら
						enemy->m_nowState != Game::kHitPlayer)//エネミーがkDeleteじゃないときのみ
					{
						//エネミーのダメージ処理を行う
						enemy->HitPlayer(*m_pPlayer, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
						//プレイヤーのダメージ処理を行う
						m_pPlayer->HitEnemy(*enemy, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
						//スペシャルゲージがマックスじゃなかったらゲージを上昇させる
						if (!m_isSpecialMode)
						{
							//敵の攻撃力に応じてゲージを上昇させる
							AddSpecialGauge(enemy->GetAtk());
						}
						//エネミーの状態を推移させる
						enemy->m_nowState = Game::kHitPlayer;
					}
					//魔女とエネミーがぶつかったとき
					if (IsCollision(m_pPrincess->GetColCircle(), enemy->GetColCircle()))
					{
						//魔女のダメージ処理を行う,エネミーのノックバックを行う
						m_pPrincess->HitEnemy(*enemy);
						if (!m_isSpecialMode)
						{
							AddSpecialGauge(enemy->GetAtk());
						}
					}
					for (auto& magic : m_pMagic)
					{

						if (magic &&//magicがnullじゃない場合
							IsCollision(magic->GetCircleCol(), enemy->GetColCircle()) &&//MagicとEnemyがぶつかったら
							enemy->m_nowState != Game::kHitMagic)//状態がkHitMagicじゃなかったら
						{
							//エネミーの状態を変化させる
							enemy->m_nowState = Game::kHitMagic;
							//魔法のダメージ処理を行う
							enemy->HitMagic(magic);
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
				if (magic->m_nowState != Game::kDelete)
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
				if (item->m_nowState == Game::kNormal)
				{
					item->Update();
					if (IsCollision(m_pPlayer->GetColCircle(), item->GetColCircle()))
					{
						m_pPlayer->PickUpItem(item);
						item->m_nowState = Game::kDelete;
					}
				}
			}
		}
		for (auto& treasure : m_pTreasure)
		{
			//itemがnullじゃない場合
			if (treasure)
			{
				//アイテムが存在している場合
				if (treasure->m_nowState != Game::kDelete)
				{
					treasure->Update();
					//プレイヤーとぶつかったら
					if (IsCollision(m_pPlayer->GetColCircle(), treasure->GetColCircle()))
					{
						m_pPlayer->HitTreasure(treasure);
						treasure->HitPlayer(m_pPlayer);
					}
					//魔法ととぶつかったら
					for (auto& magic : m_pMagic)
					{
						if (magic &&//magicがnullじゃなかったら
							IsCollision(magic->GetCircleCol(), treasure->GetColCircle()))
						{
							treasure->HitMagic();
							treasure->m_nowState = Game::kHitMagic;
						}
					}

				}
				else//アイテムが存在しない状態になったら
				{
					treasure = nullptr;
				}
			}
		}
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
		if (m_specialGauge >= kMaxSpecialGauge)
		{
			if (m_input.Buttons[XINPUT_BUTTON_Y])
			{
				m_isSpecialMode = true;
			}
		}
		//聖剣モード発動中だったら
		if (m_isSpecialMode)
		{
			//少しずつゲージを減らしていく
			m_specialGauge -= 0.2f;
			//カウントを進める
			m_particleCount++;
			//背景に表示するパーティクルを生成する
			if (m_particleCount > kParticleInterval)
			{
				m_particleCount = 0;
				m_pParticle = new Particle(m_pPrincess->GetPos(), 2000, 10, 2, 2);
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
		if (m_input.Buttons[XINPUT_BUTTON_B])
		{
			m_isPause = false;
		}
		//Aボタンを押したら選択している項目に応じて処理を行う
		if (m_input.Buttons[XINPUT_BUTTON_A])
		{
			switch (m_pauseSelectNum)
			{
			case 0:
				m_isPause = false;
				break;
			case 1:
				m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
				break;

			}
		}
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
		//上キーと下キーが離されたら
		else if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
		{
			m_isSelectKeyDown = false;
		}

	}
	///////////////////////////////////////////////////////////////////////////////
	//クリア判定
	if (m_killBossCount >= m_bossCount)
	{
		m_clearTime++;

		if (m_clearTime > kClearTime)
		{
			m_isClearFlag = true;
			m_isStop = true;
			m_clearTime = 0;
			if (m_isEnd)
			{
				for (int i = 0; m_pPlayer->GetExp() != 0; i++)
				{
					m_pPlayer->SubExp();
					UserData::userExp++;
				}
				for (int i = 0; m_pPlayer->GetGold() != 0; i++)
				{
					m_pPlayer->SubGold();
					UserData::userGold++;
				}
				m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////
}

void SceneMain::Draw()
{

	//プレイ画面の背景
	DrawGraph(0, 0, m_bgHandle, true);
	if (m_isSpecialMode)
	{
		//聖剣モードに入ったら背景を暗くする
		DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
	}

	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState == Game::kNormal)
			{
				item->Draw();
			}
		}
	}
	for (auto& magic : m_pMagic)
	{
		//magicがnullじゃない場合
		if (magic)
		{
			//アイテムが存在している場合
			if (magic->m_nowState != Game::kDelete)
			{
				magic->Draw();

			}
		}
	}

	for (auto& treasure : m_pTreasure)
	{
		//treasureがnullじゃない場合
		if (treasure)
		{
			//アイテムが存在している場合
			if (treasure->m_nowState == Game::kNormal)
			{
				treasure->Draw();
			}
		}
	}
	m_pPlayer->Draw();
	for (const auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			enemy->Draw();

		}
	}
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
	m_pPrincess->Draw();
	m_pUi->Draw();
	//クリアしたら
	if (m_isClearFlag)
	{
		m_pUi->SceneClearUI();
	}
	//ポーズ画面を開いていたら
	if (m_isPause)
	{
		int stringWidth = GetDrawStringWidth("つづける", 0);
		DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 700,
			"つづける", GetColor(0, 0, 0));
		stringWidth = GetDrawStringWidth("やめる", 0);
		DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 800,
			"やめる", GetColor(0, 0, 0));
		switch (m_pauseSelectNum)
		{
		case 0:
			stringWidth = GetDrawStringWidth("つづける", 0);
			DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 700,
				"つづける", GetColor(255, 0, 0));
			break;
		case 1:
			stringWidth = GetDrawStringWidth("やめる", 0);
			DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 800,
				"やめる", GetColor(255, 0, 0));
			break;

		}
	}
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

