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
	constexpr int kKnockBackScale = 6;
	//Hpバーの横の長さ
	constexpr float kMaxBarWidth = 60;
	//Hpバーの縦の長さ
	constexpr float kBarHeight = 5;
	//Hpバーのポジション
	constexpr int kHpBarPosY = 40;
	//持てる血の量の最大値
	constexpr float kMaxBlood = 10;
	//バーの後ろに表示するBoxの余白の大きさ
	constexpr int kBoxSpace = 3;
	//キャラの拡大率
	constexpr float kCharcterScale = 6.0f;
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
	m_isDeathFlag(false),
	m_pMain(pMain)
{
	//初期座標を魔女の隣に設定
	m_pos.x = Game::kPlayScreenWidth / 2 + 70;
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
	m_atk = 2.0f + (UserData::userAtkLevel * 0.5f);
	m_hp = 30;
	m_spd = 1.5f + (UserData::userSpdLevel * 0.1f);
	m_def = 1.0f + (UserData::userDefLevel * 0.3f);
	m_nowHp = m_hp;
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
		if (pad & PAD_INPUT_UP)
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
		else if (pad & PAD_INPUT_DOWN)
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
		else if (pad & PAD_INPUT_LEFT)
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
		else if (pad & PAD_INPUT_RIGHT)
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
		if (m_nowHp > m_hp)
		{
			//現在の体力が最大値よりも大きくならないように
			m_nowHp = m_hp;
			//状態をkNormalに変化させる
			m_nowState = Game::kNormal;
		}
	}
	//体力が0の時は動けないようにする
	if (m_nowHp > 0 && m_nowState != Game::kDelete)
	{


		m_isMove = false;
		// 移動量を持つようにする
		Vec2 move{ 0.0f, 0.0f };	// 引数ありコンストラクタは{}　()でもできるが{}のほうが良い

		//ユーザのKey入力を取得
		if (pad & PAD_INPUT_UP)
		{
			move.y--;
			m_dirY = -1;
			m_isMove = true;
		}
		else if (pad & PAD_INPUT_DOWN)
		{
			move.y++;
			m_dirY = 1;
			m_isMove = true;
		}
		else
		{
			m_dirY = 0;
		}
		if (pad & PAD_INPUT_LEFT)
		{
			move.x--;
			m_dirX = -1;
			m_isMove = true;
		}
		else if (pad & PAD_INPUT_RIGHT)
		{
			move.x++;
			m_dirX = 1;
			m_isMove = true;
		}
		else
		{
			m_dirX = 0;
		}

		//取得したKey入力から、Playerキャラの方向を作成してそれを返す。
		GetDir(m_dirX, m_dirY);

		// 正規化
		move.Normalize();
		// 長さの変更
		move *= m_spd;
		// 座標にベクトルを足す
		m_pos += move;
		//ノックバック処理
		if (m_knockBack.x != 0 || m_knockBack.y != 0)
		{
			m_knockBackTime++;
			if (m_knockBackTime > 5)
			{
				m_knockBack *= 0;
				m_knockBackTime = 0;
			}
		}
		//現在のポジションにノックバックの力を加える
		m_pos -= m_knockBack;
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
	m_knockBack = enemy.GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale;
	//聖剣モードに入っているときはダメージ処理を行わない
	if (!m_pMain->GetSpecialMode())
	{
	//弱点に当たっていたら
		if (weak)
		{
			float damage = 0;
			//受けるダメージを半分にする
			damage = enemy.GetAtk() - m_def / 2;
			//最低でも0.5ダメージは食らうようにする
			if (damage < 1)
			{
				damage = 0.5f;
			}
			m_nowHp -= damage;

		}
		else//弱点に当たっていなかったらそのまま
		{
			float damage = 0;
			damage = enemy.GetAtk() - m_def;
			//最低でも0.5ダメージは食らうようにする
			if (damage < 1)
			{
				damage = 0.5f;
			}
			m_nowHp -= damage;
		}
	}
	//体力がマイナスにならないように
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
		//状態をkDeleteに変化させる
		m_nowState = Game::kDelete;
		//倒れた座標を保存する
		m_deathPos = m_pos;
	}
}
void Player::HitTreasure(TreasureBox* treasureBox)
{
	m_knockBack = treasureBox->GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale * (GetRand(3) + 3);
}
void Player::PickUpItem(std::shared_ptr<ItemBase> item)
{
	switch (item->GetKind())
	{
	case Game::kEmpty:
		printfDx("バグ");
		break;
	case Game::kExp:
		//持っている経験値量を増やす
		m_exp += item->GetExp();
		break;
	case Game::kGold:
		//持っているお金を増やす
		m_gold += item->GetPrice();
		break;
	case Game::kBlood:
		//プレイヤーの持つ血の量を増やす
		//持っているの血の量が上限値よりも大きくなかったら
		if (m_nowBlood < m_maxBlood)
		{
			m_nowBlood++;
		}
		break;
	case Game::kPortion:
		//プレイヤーの体力を全回復させる
		m_nowHp = m_hp;
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


