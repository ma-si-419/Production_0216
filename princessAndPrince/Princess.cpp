#include "Princess.h"
#include "CircleCol.h"
#include "DxLib.h"
#include "Game.h"
#include "Enemy.h"
#include "SceneMain.h"
#include "MagicBase.h"
#include "UserData.h"
namespace
{
	// 移動速度
	constexpr float kSpeed = 3.0f;

	// キャラクターのサイズ
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	// 半分のサイズ
	constexpr float kGraphHalfWidth = kGraphWidth / 2.0f;
	constexpr float kGraphHalfHeight = kGraphHeight / 2.0f;

	// キャラクターのアニメーション
	constexpr int kUseFrame[] = { 1,0 };
	// アニメーション1コマのフレーム数
	constexpr int kAnimFrameNum = 60;
	// アニメーションの１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//ノックバックの大きさ
	constexpr int kKnockBackScale = 6;
	//基本的な魔法の大きさ
	constexpr float kMagicScale = 20;
	//Hpバーの横の長さ
	constexpr float kMaxBarWidth = 60;
	//Hpバーの縦の長さ
	constexpr float kBarHeight = 5;
	//Hpバーのポジション
	constexpr int kHpBarPosY = 40;
	//持てる血の量の上限
	constexpr float kMaxBlood = 10;
	//バーの後ろに表示するBoxの余白の大きさ
	constexpr int kBoxSpace = 3;
	//魔女が向いている方向を示す線の長さ
	constexpr float kBarLen = 200;
	//真下を向いている場合の時の数値
	constexpr float kDownAngle = 4.71f;
	//魔法を打つ間隔
	constexpr int kMagicInterval = 40;
	//同時に出す風魔法の数
	constexpr int kWindVol = 2;
	//キャラの拡大率
	constexpr float kCharcterScale = 6.0f;
	//ベースの攻撃力
	constexpr float kBaseAtk = 1.0f;
	//揺れる大きさ
	constexpr int kShakeWidth = 2;
	//揺れる時間
	constexpr int kShakeTime = 15;
	//揺れるスピード
	constexpr float kshakeSpeed = 2.0f;
}
Princess::Princess(SceneMain* pMain) :
	m_hpBarWidth(0),
	m_bloodBarWidth(0),
	m_maxBlood(kMaxBlood),
	m_nowBlood(0),
	m_nowHp(m_hp),
	m_isLastKeyFlag(false),
	m_isFire(true),
	m_angle(kDownAngle),
	m_result(0, 0),
	m_MagicCount(0),
	m_isMagic(false),
	m_pMain(pMain),
	m_shakeTimeCount(kShakeTime),
	m_shakeSpeed(kshakeSpeed)
{
	m_pos.x = Game::kPlayScreenWidth / 2;
	m_pos.y = Game::kPlayScreenHeight / 2;
	m_basePos = m_pos;
	m_dir = Game::kDirDown;
	m_animFrame = kAnimFrameNum;
	m_radius = Game::kRadius;
}

Princess::~Princess()
{
}

void Princess::Init()
{
	m_hp = 50;
	m_nowHp = m_hp;
	m_atk = kBaseAtk;
	m_def = 0;
	m_scale = kMagicScale;
}

void Princess::Update()
{
	//当たり判定の中心座標を設定
	m_circleCol.SetCenter(m_pos, m_radius);
	//HPバーの処理//
	//座標を参考にHpバーの位置を設定
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	//	あとでマジックナンバーどうにかする
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//血の量が上限値を超えていたら
	if (m_nowBlood >= kMaxBlood)
	{
		m_isMagic = true;
		m_nowBlood = kMaxBlood;
	}
	else if (m_nowBlood <= 0)
	{
		m_isMagic = false;
		m_nowBlood = 0;
	}
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//魔法を打っている最中
	if (m_isMagic || m_pMain->GetSpecialMode())
	{
		m_MagicCount++;
		//ボタンの押されている状況をとる
		//RBボタンが押されていたら
		if (m_input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER])
		{
			//回転させる処理を入れる
			m_angle -= 0.03f;

		}
		//LBボタンが押されていたら
		if (m_input.Buttons[XINPUT_BUTTON_LEFT_SHOULDER])
		{
			//回転させる処理を入れる
			m_angle += 0.03f;
		}
		//炎魔法を選択している場合
		if (m_isFire)
		{
			//カウントが定数を超えたら
			if (m_MagicCount > kMagicInterval)
			{
				//カウントをゼロにして
				m_MagicCount = 0;
				//魔法を撃つ
				m_pMagic = new MagicBase(this, m_scale);
				m_pMagic->Init(0);
				m_pMain->AddMagic(m_pMagic);

			}
		}
		//風魔法を選択している場合
		else
		{
			//風魔法のインターバルは長くするために２倍する
			if (m_MagicCount > kMagicInterval * 2)
			{
				//カウントをゼロにして
				m_MagicCount = 0;
				//風魔法は複数個同時に出るので
				for (int i = 0; kWindVol > i; i++)
				{
					m_pMagic = new MagicBase(this, m_scale);
					m_pMagic->Init(i);
					m_pMain->AddMagic(m_pMagic);
				}
			}
		}

		//持っている血の量を減らしていく
		m_nowBlood -= 0.02f;

	}
	//Aボタンが押されたら
	if (m_input.Buttons[XINPUT_BUTTON_A] && !m_isLastKeyFlag || CheckHitKey(KEY_INPUT_Z))
	{
		//連続で切り替わらないように
		m_isLastKeyFlag = true;
		//魔法の種類を変更する
		m_isFire = !m_isFire;
		//魔法を撃つ間隔をリセットする
		m_MagicCount = 0;
	}
	else if (!m_input.Buttons[XINPUT_BUTTON_A])
	{
		//連続で切り替わらないように
		m_isLastKeyFlag = false;
	}
	//血の量バーの処理
	//上のHpバーよりも少し低い座標に表示させる
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//血の量の割合を計算してバーの長さを算出する
	m_bloodBarWidth = kMaxBarWidth * GetBloodRate();
	//今向いている方向を計算する
	m_result.x = cosf(m_angle);
	m_result.y = sinf(m_angle);
	m_result *= kBarLen;
	m_magicVec.x = m_pos.x + m_result.x;
	m_magicVec.y = m_pos.y - m_result.y;
	//聖剣モードの処理
	if (m_pMain->GetSpecialMode())
	{
		m_atk = kBaseAtk * 1.5f;
		m_scale = kMagicScale * 2.0f;
	}
	else
	{
		m_atk = kBaseAtk;
		m_scale = kMagicScale;
	}
	//足し続ける
	m_shakeTimeCount++;
	if (m_pos.x < m_basePos.x - kShakeWidth)
	{
		m_shakeSpeed *= -1;
	}
	else if (m_pos.x > m_basePos.x + kShakeWidth)
	{
		m_shakeSpeed *= -1;
	}
	if (m_shakeTimeCount < kShakeTime)
	{
		m_pos.x += m_shakeSpeed;
	}
	else
	{
		m_pos = m_basePos;
	}
	//ポーズから戻ったときに魔法が切り替わらないように
	if (m_pMain->GetPause())
	{
		m_isLastKeyFlag = true;
	}
}

void Princess::Draw() const
{
	int animEle = m_animFrame / kAnimFrameNum;

	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;

	//魔法を打つ方向に線を表示する
	if (m_isMagic)
	{
		DrawLine(m_pos.x, m_pos.y,//始点
			m_pos.x + m_result.x, m_pos.y - m_result.y,//計算結果を始点に足して終点にする
			GetColor(0, 0, 0));
	}
	//画像のどこを切り取るかを指定して、切り取った画像を表示する
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,//切り取る位置
		kGraphWidth, kGraphHeight,//切り取る大きさ
		kCharcterScale,//拡大率
		0.0,//回転率
		m_handle, true, false);
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


#ifdef _DEBUG
	m_circleCol.Draw(m_radius, 0x0000ff, false);
#endif
}

void Princess::HitEnemy(Enemy& enemy)
{
	m_shakeTimeCount = 0;
	Vec2 knockBack;
	knockBack = m_pos - enemy.GetPos();
	knockBack.Normalize();
	knockBack *= kKnockBackScale;
	m_nowHp -= enemy.GetAtk() - m_def;
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
	}
	enemy.HitPrincess(knockBack);
}

bool Princess::IsDeath()
{
	if (m_nowHp <= 0)
	{
		return true;
	}
	return false;
}
