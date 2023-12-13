#include <DxLib.h>
#include <cassert>
#include "Pad.h"
#include "CircleCol.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "MagicBase.h"
#include "Blood.h"
#include "TreasureBox.h"
#include "Player.h"
#include "Princess.h"
#include "Enemy.h"
#include "Bg.h"
#include "UI.h"
namespace
{
	//アイテムの最大数
	constexpr int kMaxItem = 256;
	//同時に存在する魔法の最大数
	constexpr int kMaxMagicValue = 32;
	//持てる血の量の最大数
	constexpr float kMaxBlood = 10;
	//エネミーが生まれるインターバル(仮処理)
	constexpr int kEnemyPopInterval = 50;
	//宝箱の同時に存在する最大数
	constexpr int kMaxTreasureBox = 16;
}
SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0)
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
	m_pEnemy.resize(30);
	//アイテムの最大数を設定
	m_pItem.resize(kMaxItem);
	//魔法の最大数を設定
	m_pMagic.resize(kMaxMagicValue);
	//宝箱の最大数を設定
	m_pTreasure.resize(kMaxTreasureBox);
	//UIのコンストラクタ
	m_pUi = new UI(m_pPlayer);
	for (auto& enemy : m_pEnemy)
	{
		////Enemyのコンストラクタ
		//enemy = std::make_shared<Enemy>(this, m_pPlayer);
		//Enemyのメンバ変数にアクセス
		/*enemy->SetHandle(m_enemyHandle);*/
	}
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pPrincess->Init();
}

void SceneMain::End()
{
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
	m_enemyPopTimeCount++;
	if (m_enemyPopTimeCount > kEnemyPopInterval)
	{
		//カウントを初期化
		m_enemyPopTimeCount = 0;
		//エネミーを出現させる
		createEnemy();


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
					enemy->m_nowState != Game::kDelete)//エネミーがkDeleteじゃないときのみ
				{
					//エネミーのダメージ処理を行う
					enemy->HitPlayer(*m_pPlayer);
					//プレイヤーのダメージ処理を行う
					m_pPlayer->HitEnemy(*enemy);
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
					m_pPlayer->PickUpItem(item->GetKind());
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
			if (treasure->m_nowState == Game::kNormal)
			{
				treasure->Update();
				//当たり判定処理を作成する
				if (IsCollision(m_pPlayer->GetColCircle(), treasure->GetColCircle()))
				{
					m_pPlayer->HitTreasure(treasure);
				}
				//魔法とプレイヤー

			}
			else//アイテムが存在しない状態になったら
			{
				treasure = nullptr;
			}
		}
	}
}

void SceneMain::Draw()
{
	DrawGraph(0, 0, m_bgHandle, false);
	for (const auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			enemy->Draw();

		}
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
	m_pPrincess->Draw();
	m_pPlayer->Draw();
	m_pUi->Draw();
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

bool SceneMain::createEnemy()
{
	//アイテムの配列の長さだけfor文を回す
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		//使用中なら次のチェックへ
		if (m_pEnemy[i]) continue;
		//ここに来たということはm_pShot[i] == nullptr
		m_pEnemy[i] = make_shared<Enemy>(this);
		m_pEnemy[i]->SetHandle(m_enemyHandle);
		m_pEnemy[i]->Init(GetRand(4));
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

