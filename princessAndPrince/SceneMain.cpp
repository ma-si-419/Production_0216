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
#include "Enemy.h"
#include "UI.h"
#include "UserData.h"
namespace
{
	//アイテムの最大数
	constexpr int kMaxItem = 1024;
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
	constexpr int kClearTime = 180;
	//倒すボスの数(仮実装)
	constexpr int kBossCount = 1;
}
SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0),
	m_clearFlag(false),
	m_bossCount(kBossCount),
	m_killBossCount(0),
	m_clearTime(0),
	m_nextEnemyKind(0),
	m_nextEnemyPopTime(0)
{
	//プレイヤーのグラフィックのロード
	m_playerHandle = LoadGraph("data/image/Monkey.png");
	//Playerのコンストラクタ
	m_pPlayer = new Player;
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
	//UIのコンストラクタ
	m_pUi = new UI(m_pPlayer);
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
	//音楽再生
//	if (m_flag)
//	{
//		PlaySoundFile("data/sound/mainBgm.mp3", DX_PLAYTYPE_LOOP);
//		m_flag = false;
//	}
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
					enemy->m_nowState != Game::kHit)//エネミーがkDeleteじゃないときのみ
				{
					//エネミーのダメージ処理を行う
					enemy->HitPlayer(*m_pPlayer,IsCollision(m_pPlayer->GetColCircle(),enemy->GetWeakCircle()));
					//プレイヤーのダメージ処理を行う
					m_pPlayer->HitEnemy(*enemy,IsCollision(m_pPlayer->GetColCircle(),enemy->GetWeakCircle()));
					//エネミーの状態を推移させる
					enemy->m_nowState == Game::kHit;
				}
				//魔女とエネミーがぶつかったとき
				if (IsCollision(m_pPrincess->GetColCircle(), enemy->GetColCircle()))
				{
					//魔女のダメージ処理を行う,エネミーのノックバックを行う
					m_pPrincess->HitEnemy(*enemy);
				}
				//エネミーとプレイヤーが離れたら(ぶつかったときの処理が複数回行われないために)
				if (IsCollision(m_pPlayer->GetColCircle(), enemy->GetColCircle()) &&
					enemy->m_nowState == Game::kHit)
				{
					//エネミーの状態をkNormalに変化させる
					enemy->m_nowState = Game::kNormal;
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
					treasure->HitPlayer();
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
	//クリア判定
	if (m_killBossCount >= m_bossCount)
	{
		m_clearTime++;

		m_clearFlag = true;
		if (m_clearTime > kClearTime)
		{
			UserData::userGold += m_pPlayer->GetGold();
			UserData::userExp += m_pPlayer->GetExp();
			m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
		}
	}
}

void SceneMain::Draw()
{
	
	//プレイ画面の背景
	DrawGraph(0, 0, m_bgHandle, true);
	
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
		//itemがnullじゃない場合
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
	DrawBox(960, 0, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(100, 100, 100), true);
	m_pPrincess->Draw();
	m_pUi->Draw();
	//クリアしたら
	if (m_clearFlag)
	{
		int stringWidth = GetDrawStringWidth("ゲームクリア", -1);
		DrawString((Game::kPlayScreenWIdth - stringWidth) / 2, 200, "ゲームクリア", GetColor(0, 0, 0));
		DrawString(300, 500, "獲得経験値", GetColor(0, 0, 0));
		DrawString(300, 600, "獲得ゴールド", GetColor(0, 0, 0));
		DrawFormatString(600, 500, GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());
		DrawFormatString(600, 600, GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());

	}
	//UIの表示（仮）
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
	//魔法の配列の長さ
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

bool SceneMain::IsCollision(CircleCol col1, CircleCol col2)
{
	if (col1.IsCollsion(col2))
	{
		return true;
	}
	return false;
}

