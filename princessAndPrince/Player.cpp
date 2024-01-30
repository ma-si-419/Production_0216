#include <DxLib.h>
#include "Player.h"
#include "Game.h"
#include "Pad.h"
#include "SceneMain.h"
#include "CircleCol.h"
#include "TreasureBox.h"
#include "Enemy.h"
#include "Princess.h"
#include "ItemBase.h"
#include "UserData.h"

namespace
{
	// 移動速度
	constexpr float kSpeed = 3.0f;
	//画像の拡大率
	constexpr double kGraphScale = 3.0;
	// キャラクターのサイズ
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	// キャラクターの半分のサイズ
	constexpr float kGraphHalfWidth = kGraphWidth / 2.0f;
	constexpr float kGraphHalfHeight = kGraphHeight / 2.0f;
	// キャラクターのアニメーション
	constexpr int kUseFrame[] = { 1, 2, 3, 2 };
	// アニメーション1コマのフレーム数
	constexpr int kAnimFrameNum = 12;
	// アニメーションの１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//ノックバックの大きさ
	constexpr int kKnockBackScale = 3;
	//Hpバーの横の長さ
	constexpr float kMaxBarWidth = 60;
	//Hpバーの縦の長さ
	constexpr float kBarHeight = 5;
	//Hpバーのポジション
	constexpr int kHpBarPosY = 50;
	//復活した後の硬直時間
	constexpr int kRevivalTime = 30;
	//持てる血の量の最大値
	constexpr float kMaxBlood = 10;
	//バーの後ろに表示するBoxの余白の大きさ
	constexpr int kBoxSpace = 3;
	//キャラの拡大率
	constexpr float kCharcterScale = 6.0f;
	//クリア時のダンスの時間の長さ
	constexpr int kDanceTIme = 5;
	//クリア時の回る回数
	constexpr int kTurnCount = 16;
}

Player::Player(SceneMain* pMain) :
	m_isMove(false),
	m_hpBarWidth(0),
	m_bloodBarWidth(0),
	m_lastPad(0),
	m_maxBlood(kMaxBlood),
	m_nowBlood(0),
	m_nowHp(m_hp),
	m_gold(0),
	m_exp(0),
	m_moveVec(),
	m_isDeathFlag(false),
	m_pMain(pMain),
	m_turnCount(0),
	m_danceCount(0)
{
	//初期座標を魔女の隣に設定
	m_pos.x = Game::kPlayScreenWidth / 2 + 100;
	m_pos.y = Game::kPlayScreenHeight / 2;
	//最初の向きを下向きに設定
	m_dir = Game::kDirDown;
	//アニメーションの最初の画像を設定
	m_animFrame = kAnimFrameNum;
	//円の半径を設定
	m_radius = Game::kRadius;
	//状態を初期化
	m_nowState = Game::kNormal;

}

Player::~Player()
{

}

void Player::Init()
{
	m_atk = 2.0f + (UserData::userAtkLevel * 0.5f) + (UserData::userMainLevel * 0.1f);
	m_hp = 30 + (UserData::userMainLevel * 2.0f);
	m_spd = 1.5f + (UserData::userSpdLevel * 0.1f) + (UserData::userMainLevel * 0.02f);
	m_def = 1.0f + (UserData::userDefLevel * 0.3f) + (UserData::userMainLevel * 0.05f);
	m_nowHp = m_hp;
	//座標を参考にHpバーの位置を設定
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	//Hpの割合を計算してバーの長さを算出する
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//血の量バーの処理
	//上のHpバーよりも少し低い座標に表示させる
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//コインの効果音を設定する
	m_coinSe = m_pMain->GetCoinSe();
	m_expSe = m_pMain->GetExpSe();
	m_portionSe = m_pMain->GetPortionSe();
	m_bloodSe = m_pMain->GetBloodSe();
}

void Player::Update()
{
	// パッドの十字キーを使用してプレイヤーを移動させる
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//倒れているときは徐々に回復するようにする
	if (m_nowState == Game::kDelete)
	{
		//少しずつ回復するようにする
		m_nowHp += GetHealRate();
		//レバガチャすると早く回復するようにする
		//おしっぱではだめなので、if文を追加する
		//倒れている状態でないとレバガチャできないようにする(死亡演出を入れるために)
		if (m_isDeathFlag)
		{

			if (pad & PAD_INPUT_UP || CheckHitKey(KEY_INPUT_W))
			{
				if (m_lastPad != 0)
				{
					//今入力したkeyを保存する
					m_lastPad = 0;
					//回復する
					m_nowHp += GetHealRate() * 3;
					//ランダムにプレイヤーを動かす
					m_pos.x = m_deathPos.x + GetRand(2) - 1;
					m_pos.y = m_deathPos.y + GetRand(2) - 1;
				}

			}
			else if (pad & PAD_INPUT_DOWN || CheckHitKey(KEY_INPUT_S))
			{
				if (m_lastPad != 1)
				{
					//今入力したkeyを保存する
					m_lastPad = 1;
					//回復する
					m_nowHp += GetHealRate() * 3;
					//ランダムにプレイヤーを動かす
					m_pos.x = GetRand(10) + m_deathPos.x;
					m_pos.y = GetRand(10) + m_deathPos.y;
				}
			}
			else if (pad & PAD_INPUT_LEFT || CheckHitKey(KEY_INPUT_A))
			{
				if (m_lastPad != 2)
				{
					//今入力したkeyを保存する
					m_lastPad = 2;
					//回復する
					m_nowHp += GetHealRate() * 3;
					//ランダムにプレイヤーを動かす
					m_pos.x = GetRand(10) + m_deathPos.x;
					m_pos.y = GetRand(10) + m_deathPos.y;
				}
			}
			else if (pad & PAD_INPUT_RIGHT || CheckHitKey(KEY_INPUT_D))
			{
				if (m_lastPad != 3)
				{
					//今入力したkeyを保存する
					m_lastPad = 3;
					//回復する
					m_nowHp += GetHealRate() * 3;
					//ランダムにプレイヤーを動かす
					m_pos.x = GetRand(10) + m_deathPos.x;
					m_pos.y = GetRand(10) + m_deathPos.y;
				}
			}
		}
		if (m_nowHp >= m_hp)
		{
			/////////////////////////////////////////////////////////////////////////
			//現在の体力が最大値よりも大きくならないように
			m_nowHp = m_hp;
			//アニメーションを動かすかどうか
			bool isAnimEnd = false;
			if (m_animFrame / kAnimFrameNum == 2)
			{
				if (m_pMain->GetSpecialMode())
				{
					m_revivalCount += kRevivalTime;
				}
				m_revivalCount++;
				isAnimEnd = true;
			}
			// 復活アニメーションを動かす
			if (!isAnimEnd)
			{
				m_animFrame++;
				if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 1;
			}

			//復活の硬直時間を超えたら
			if (m_revivalCount > kRevivalTime)
			{
				//状態をkNormalに変化させる
				m_nowState = Game::kNormal;
				//向いている向きを下向きにする
				m_dir = Game::kDirDown;
				//死んだフラッグをfalseにする
				m_isDeathFlag = false;
				m_revivalCount = 0;
			}
			/////////////////////////////////////////////////////////////////////////
		}
	}
	//体力が0の時以外動くようにする
	if (m_nowHp > 0 && m_nowState != Game::kDelete)
	{


		m_isMove = false;

		m_moveVec.x = 0.0f;
		m_moveVec.y = 0.0f;
		if (m_nowState != Game::kStop)
		{

			//ユーザのKey入力を取得
			if (pad & PAD_INPUT_UP || CheckHitKey(KEY_INPUT_W))
			{
				m_moveVec.y--;
				m_dirY = -1;
				m_isMove = true;
			}
			else if (pad & PAD_INPUT_DOWN || CheckHitKey(KEY_INPUT_S))
			{
				m_moveVec.y++;
				m_dirY = 1;
				m_isMove = true;
			}
			else
			{
				m_dirY = 0;
			}
			if (pad & PAD_INPUT_LEFT || CheckHitKey(KEY_INPUT_A))
			{
				m_moveVec.x--;
				m_dirX = -1;
				m_isMove = true;
			}
			else if (pad & PAD_INPUT_RIGHT || CheckHitKey(KEY_INPUT_D))
			{
				m_moveVec.x++;
				m_dirX = 1;
				m_isMove = true;
			}
			else
			{
				m_dirX = 0;
			}
		}

		//取得したKey入力から、Playerキャラの方向を作成してそれを返す。
		GetDir(m_dirX, m_dirY);

		// 正規化
		m_moveVec.Normalize();
		// 長さの変更
		m_moveVec *= m_spd;
		//移動量にノックバックを足す
		m_moveVec -= m_knockBack;
		// 座標にベクトルを足す
		if (m_nowState != Game::kHitEnemy)
		{
			m_pos += m_moveVec;
		}
		else if (m_nowState == Game::kHitEnemy)
		{
			m_pos -= m_knockBack;
		}
		//ノックバック処理
		if (m_knockBack.x != 0 || m_knockBack.y != 0)
		{
			m_knockBackTime++;
			//一定時間ノックバックしたら
			if (m_knockBackTime > 5)
			{
				m_nowState = Game::kNormal;
				m_knockBack *= 0;
				m_knockBackTime = 0;
			}
		}
		// 当たり判定の更新
		m_circleCol.SetCenter(m_pos, m_radius);

		// 斜め移動の場合も同じ速さで移動するようにする
		if (m_isMove)
		{
			// 歩きアニメーション
			m_animFrame++;
			if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 1;
		}
		else
		{
			m_animFrame = kAnimFrameNum;
		}
	}
	else
	{
		DeathMove();
	}
	// 縦軸の移動制限
	if (m_pos.y < kGraphHalfHeight * kCharcterScale)
		m_pos.y = kGraphHalfHeight * kCharcterScale;
	else if (Game::kPlayScreenHeight - (kGraphHalfHeight * kCharcterScale) < m_pos.y)
		m_pos.y = Game::kPlayScreenHeight - (kGraphHalfHeight * kCharcterScale);
	// 横軸の移動制限
	if (m_pos.x < kGraphHalfWidth * kCharcterScale)
		m_pos.x = kGraphHalfWidth * kCharcterScale;
	else if (Game::kPlayScreenWidth - (kGraphHalfWidth * kCharcterScale) < m_pos.x)
		m_pos.x = Game::kPlayScreenWidth - (kGraphHalfWidth * kCharcterScale);
	//HPバーの処理//
	//座標を参考にHpバーの位置を設定
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	//Hpの割合を計算してバーの長さを算出する
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//血の量バーの処理
	//上のHpバーよりも少し低い座標に表示させる
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//血の量の割合を計算してバーの長さを算出する
	m_bloodBarWidth = kMaxBarWidth * GetBloodRate();
	if (m_nowBlood <= 0)
	{
		m_nowBlood = 0;
	}
	//聖剣モード状態のステータス変化
	if (m_pMain->GetSpecialMode())
	{
		m_nowHp = m_hp;
		m_atk = (2.0f + (UserData::userAtkLevel * 0.5f)) * 1.5f;
		m_spd = 4.0f;
	}
	else
	{
		m_atk = 2.0f + (UserData::userAtkLevel * 0.5f);
		m_spd = 1.5f + (UserData::userSpdLevel * 0.1f);
	}
}

void Player::Draw() const
{
	int animEle = m_animFrame / kAnimFrameNum;

	//画像のどこを切り取るか計算
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;
	//描画処理
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		kCharcterScale,
		0.0,
		m_handle, true, false);
	////////////////////
	/* HPバー表示演出 */
	////////////////////
	{
		//Hp等を見やすくするために後ろにBox作成
		DrawBox((int)m_hpBarPos.x - kBoxSpace, (int)m_hpBarPos.y - kBoxSpace,//始点
			(int)m_bloodBarPos.x + kMaxBarWidth + kBoxSpace,//始点の位置にバーの長さを足す
			(int)m_bloodBarPos.y + (int)kBarHeight + kBoxSpace,//終点
			GetColor(75, 75, 75), true);
		//Hpが見やすいようにHpの後ろに黒いBoxを出す
		DrawBox((int)m_hpBarPos.x, (int)m_hpBarPos.y,//始点
			(int)m_hpBarPos.x + (int)kMaxBarWidth,//始点の位置にバーの長さを足す
			(int)m_hpBarPos.y + (int)kBarHeight,//終点
			GetColor(0, 0, 0), true);
		//HPバーの表示
		DrawBox((int)m_hpBarPos.x, (int)m_hpBarPos.y,//始点
			(int)m_hpBarPos.x + (int)m_hpBarWidth,//始点の位置にバーの長さを足す
			(int)m_hpBarPos.y + (int)kBarHeight,//終点
			GetColor(255, 255, 100), true);
		//血の量が見やすいように血の量の後ろに白いBoxを出す
		DrawBox((int)m_bloodBarPos.x, (int)m_bloodBarPos.y,//始点
			(int)m_bloodBarPos.x + (int)kMaxBarWidth,//始点の位置にバーの長さを足す
			(int)m_bloodBarPos.y + (int)kBarHeight,//終点
			GetColor(0, 0, 0), true);
		//血の量バーの表示
		DrawBox((int)m_bloodBarPos.x, (int)m_bloodBarPos.y,//始点
			(int)m_bloodBarPos.x + (int)m_bloodBarWidth,//始点の位置にバーの長さを足す
			(int)m_bloodBarPos.y + (int)kBarHeight,//終点
			GetColor(255, 0, 0), true);
	}

#ifdef _DEBUG
	m_circleCol.Draw(m_radius, 0x0000ff, false);
#endif
}

void Player::HitEnemy(Enemy enemy, bool weak)
{
	m_nowState = Game::kHitEnemy;
	m_knockBack = enemy.GetPos() - m_pos;
	m_knockBack.Normalize();
	//エネミーの移動速度に応じてノックバックする
	//聖剣モードに入っているときはダメージ処理を行わない
		//弱点に当たっていたら
	float damage = 0;
	if (weak)
	{
		//受けるダメージを半分にする
		damage = enemy.GetAtk() - m_def / 2;
		//最低でも0.5ダメージは食らうようにする
		if (damage < 1)
		{
			damage = 0.5f;
		}
		//エネミーの移動速度に応じてノックバックする
		m_knockBack *= enemy.GetKnockBackPow() * kKnockBackScale;
		//弱点に当たっていたらノックバックの大きさを減らす
		m_knockBack *= 0.6f;

	}
	else//弱点に当たっていなかったらそのまま
	{
		damage = enemy.GetAtk() - m_def;
		//最低でも0.5ダメージは食らうようにする
		if (damage < 1)
		{
			damage = 0.5f;
		}
		//エネミーの移動速度に応じてノックバックする
		m_knockBack *= enemy.GetKnockBackPow() * kKnockBackScale;
	}
	if (!m_pMain->GetSpecialMode())
	{
		m_nowHp -= damage;
	}

	//体力がマイナスにならないように
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
		//状態をkDeleteに変化させる
		m_nowState = Game::kDelete;
	}
}
void Player::HitTreasure(TreasureBox* treasureBox)
{
	m_knockBack = m_moveVec;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale * treasureBox->GetKnockBackPow() * (GetRand(2) + 1);
	m_nowState = Game::kHitEnemy;
}
void Player::PickUpItem(std::shared_ptr<ItemBase> item)
{
	switch (item->GetKind())
	{
	case Game::kEmpty:
		printfDx("バグ");
		break;
	case Game::kExp:
		PlaySoundMem(m_expSe, DX_PLAYTYPE_BACK);
		//持っている経験値量を増やす
		item->m_nowState = Game::kNone;
		item->MoveItem(this);
		break;
	case Game::kGold:
		PlaySoundMem(m_coinSe, DX_PLAYTYPE_BACK);
		//持っているお金を増やす
		item->m_nowState = Game::kNone;
		item->MoveItem(this);
		break;
	case Game::kBlood:
		//プレイヤーの持つ血の量を増やす
		//持っているの血の量が上限値よりも大きくなかったら
		if (m_nowBlood < m_maxBlood)
		{
			m_nowBlood++;
		}
		PlaySoundMem(m_bloodSe, DX_PLAYTYPE_BACK);
		item->m_nowState = Game::kDelete;
		break;
	case Game::kPortion:
		PlaySoundMem(m_portionSe, DX_PLAYTYPE_BACK);
		//プレイヤーの体力を全回復させる
		m_nowHp = m_hp;
		item->m_nowState = Game::kDelete;
	}
}

void Player::GiveBlood(Princess* princess)
{
	float giveBlood = 0;
	giveBlood = m_nowBlood;
	m_nowBlood -= (kMaxBlood - princess->GetBlood());
	princess->ReceiveBlood(giveBlood);
	if (m_nowBlood < 0)
	{
		m_nowBlood = 0;
	}

}

void Player::DeathMove()
{
	//倒れた時に動くベクトル
	Vec2 deathMoveVec;
	//ノックバックの移動をゆっくり行う
	deathMoveVec = m_knockBack * 0.1f;
	// 座標にベクトルを足す
	m_pos -= deathMoveVec;
	//ノックバック処理
	if (m_knockBack.x != 0 || m_knockBack.y != 0)
	{
		if (m_pMain->GetSpecialMode())
		{
			m_knockBackTime += 100;
		}
		m_knockBackTime++;
		//死亡演出
		if (m_knockBackTime < 20)
		{
			m_dir = Game::kDirDown;
			m_animFrame = 0;
		}
		else if (m_knockBackTime < 40)
		{
			m_dir = Game::kDirLeft;
		}
		else if (m_knockBackTime < 60)
		{
			m_dir = Game::kDirUp;
		}
		else if (m_knockBackTime < 80)
		{
			m_dir = Game::kDirRight;
		}
		else
		{
			if (!m_pMain->GetSpecialMode())
			{
				m_dir = Game::kDirDeath;
			}
		}
		//一定時間ノックバックしたら
		if (m_knockBackTime > 100)
		{
			m_knockBack *= 0;
			m_knockBackTime = 0;
			//倒れたフラッグを立てる
			m_isDeathFlag = true;
			//倒れた座標を保存する
			m_deathPos = m_pos;
		}
	}

}

void Player::ClearDance()
{
	m_animFrame = kAnimFrameNum;
	m_danceCount++;
	if (m_danceCount > kDanceTIme)
	{
		m_danceCount = 0;
		switch (m_dir)
		{
		case Game::kDirDown:
			m_dir = Game::kDirLeft;
			break;
		case Game::kDirLeftDown:
			m_dir = Game::kDirLeft;
			break;
		case Game::kDirLeft:
			m_dir = Game::kDirUp;
			break;
		case Game::kDirLeftUp:
			m_dir = Game::kDirUp;
			break;
		case Game::kDirUp:
			m_dir = Game::kDirRight;
			break;
		case Game::kDirRightUp:
			m_dir = Game::kDirRight;
			break;
		case Game::kDirRight:
			m_dir = Game::kDirDown;
			break;
		case Game::kDirRightDown:
			m_dir = Game::kDirDown;
			break;
		case Game::kDirDeath:
			break;
		default:
			break;
		}
		m_turnCount++;
	}
	if (m_turnCount > kTurnCount)
	{
		m_dir = Game::kDirDeath;
		m_animFrame = kAnimFrameNum * 2;
		m_pMain->ShowResult();
	}
}


