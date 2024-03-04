#include "Princess.h"
#include "CircleCol.h"
#include "DxLib.h"
#include "Game.h"
#include "Enemy.h"
#include "SceneMain.h"
#include "MagicBase.h"
#include "UserData.h"
#include "Particle.h"
namespace
{
	// 移動速度
	constexpr float kSpeed = 3.0f;

	// キャラクターのサイズ
	constexpr int kGraphWidth = 24;
	constexpr int kGraphHeight = 24;
	// 半分のサイズ
	constexpr float kGraphHalfWidth = kGraphWidth / 2.0f;
	constexpr float kGraphHalfHeight = kGraphHeight / 2.0f;

	// キャラクターのアニメーション
	constexpr int kUseFrame[] = { 1,0 };
	// アニメーション1コマのフレーム数
	constexpr int kAnimFrameNum = 30;
	// アニメーションの１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//ノックバックの大きさ
	constexpr int kKnockBackScale = 28;
	//基本的な魔法の大きさ
	constexpr float kMagicScale = 15;
	//Hpバーの横の長さ
	constexpr float kMaxBarWidth = 60;
	//Hpバーの縦の長さ
	constexpr float kBarHeight = 5;
	//Hpバーのポジション
	constexpr int kHpBarPosY = 60;
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
	constexpr float kCharcterScale = 5.0f;
	//ベースの攻撃力
	constexpr float kBaseAtk = 0.5f;
	//揺れる大きさ
	constexpr int kShakeWidth = 2;
	//揺れる時間
	constexpr int kShakeTime = 15;
	//揺れるスピード
	constexpr float kShakeSpeed = 2.0f;
	//パーティクルの数
	constexpr int kParticleVol = 30;
	//Hpと血の量を表示するボックスの色
	const int kBoxColor = GetColor(75, 75, 75);
	//Hpバーの色
	const int kHpBarColor = GetColor(255, 255, 100);
	//血の量バーの色
	const int kBloodBarColor = GetColor(255, 0, 0);
	//怒りモード発動中の攻撃力
	constexpr float kAngryModeMagicAtk = kBaseAtk * 1.5f;
	//怒りモードが発動中の大きさ
	constexpr float kAngryModeMagicScale = kMagicScale * 2.0f;
	//敵とぶつかった時に出すパーティクルの情報
	constexpr int kWhiteParticleInfo[4] = { 40,4,5,0 };
	//死んだときのアニメフレーム
	constexpr int kDeathAnimFrame = 24;
	//減っていく血の量
	constexpr float kSubBloodVol = 0.02f;
	//回転するスピード
	constexpr float kRotaSpeed = 0.03f;
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
	m_shakeSpeed(kShakeSpeed),
	m_drawState(Game::WitchState::kStone),
	m_arrowGraph(0),
	m_fireMagicSe(0),
	m_hitFlag(false),
	m_magicArrowGraph(0),
	m_magicHandle(0),
	m_pMagic(),
	m_pParticle(),
	m_passBloodSe(0),
	m_scale(0),
	m_windMagicSe(0)

{
	m_pos.x = Game::kPlayScreenWidth / 2;
	m_pos.y = Game::kPlayScreenHeight / 2;
	m_basePos = m_pos;
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
	//座標を参考にHpバーの位置を設定
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	//Hpの割合を計算してバーの長さを算出する
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//血の量バーの処理
	//上のHpバーよりも少し低い座標に表示させる
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//風魔法の効果音を入れる
	m_windMagicSe = m_pMain->GetWindMagicSe();
	//炎魔法の効果音を入れる
	m_fireMagicSe = m_pMain->GetFireMagicSe();
	m_passBloodSe = m_pMain->GetPassBloodSe();
}

void Princess::Update()
{
	//当たり判定の中心座標を設定
	m_circleCol.SetCenter(m_pos, static_cast<float>(m_radius));
	//HPバーの処理//
	//座標を参考にHpバーの位置を設定
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	//	あとでマジックナンバーどうにかする
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	if (m_nowState != Game::State::kDelete)
	{

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
		if (m_isMagic || m_pMain->GetAngryMode())
		{
			//石の状態から直す
			m_drawState = Game::WitchState::kMagic;
			//アニメーションを動かす
			m_animFrame++;
			//スペシャルモード中だったら動きを早くする
			if (m_pMain->GetAngryMode())m_animFrame++;
			if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 1;
			//魔法を撃つ間隔を計る
			m_MagicCount++;
			//ボタンの押されている状況をとる
			//RBボタンが押されていたら
			if (m_input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] || CheckHitKey(KEY_INPUT_L))
			{
				//回転させる処理を入れる
				m_angle -= kRotaSpeed;

			}
			//LBボタンが押されていたら
			if (m_input.Buttons[XINPUT_BUTTON_LEFT_SHOULDER] || CheckHitKey(KEY_INPUT_J))
			{
				//回転させる処理を入れる
				m_angle += kRotaSpeed;
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
					m_pMagic->SetHandle(m_magicHandle);
					m_pMain->AddMagic(m_pMagic);
					PlaySoundMem(m_fireMagicSe, DX_PLAYTYPE_BACK);

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
						m_pMagic->SetHandle(m_magicHandle);
						m_pMain->AddMagic(m_pMagic);
						PlaySoundMem(m_windMagicSe, DX_PLAYTYPE_BACK);
					}
				}
			}

			//持っている血の量を減らしていく
			m_nowBlood -= kSubBloodVol;

		}
		else
		{
			m_drawState = Game::WitchState::kStone;
		}
		//Aボタンが押されたら
		if (m_input.Buttons[XINPUT_BUTTON_A] && !m_isLastKeyFlag || CheckHitKey(KEY_INPUT_Z) && !m_isLastKeyFlag)
		{
			//連続で切り替わらないように
			m_isLastKeyFlag = true;
			//魔法の種類を変更する
			m_isFire = !m_isFire;
			//魔法を撃つ間隔をリセットする
			m_MagicCount = 0;
#ifdef _DEBUG
			m_nowHp -= 10;
#endif
		}
		else if (!m_input.Buttons[XINPUT_BUTTON_A] && !CheckHitKey(KEY_INPUT_Z))
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
		//怒りモードの処理
		if (m_pMain->GetAngryMode())
		{
			m_atk = kAngryModeMagicAtk;
			m_scale = kAngryModeMagicScale;
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
}

void Princess::Draw() const
{
	int animEle = m_animFrame / kAnimFrameNum;

	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * static_cast<int>(m_drawState);
	//魔法を打つ方向に線を表示する
	if (m_isMagic || m_pMain->GetAngryMode())
	{
		//魔法を撃つ方向を示す矢印を表示する
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, -m_angle, m_magicArrowGraph, true, 0, 0);
	}
	//画像のどこを切り取るかを指定して、切り取った画像を表示する
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,//切り取る位置
		kGraphWidth, kGraphHeight,//切り取る大きさ
		kCharcterScale,//拡大率
		0.0,//回転率
		m_handle, true, false);
	//Hp等を見やすくするために後ろにBox作成
	DrawBox(static_cast<int>(m_hpBarPos.x) - kBoxSpace, static_cast<int>(m_hpBarPos.y) - kBoxSpace,//始点
		static_cast<int>(m_bloodBarPos.x + kMaxBarWidth) + kBoxSpace,//始点の位置にバーの長さを足す
		static_cast<int>(m_bloodBarPos.y + kBarHeight) + kBoxSpace,//終点
		kBoxColor, true);
	//Hpが見やすいようにHpの後ろに黒いBoxを出す
	DrawBox(static_cast<int>(m_hpBarPos.x), static_cast<int>(m_hpBarPos.y),//始点
		static_cast<int>(m_hpBarPos.x + kMaxBarWidth),//始点の位置にバーの長さを足す
		static_cast<int>(m_hpBarPos.y + kBarHeight),//終点
		GetColor(0, 0, 0), true);
	//HPバーの表示
	DrawBox(static_cast<int>(m_hpBarPos.x), static_cast<int>(m_hpBarPos.y),//始点
		static_cast<int>(m_hpBarPos.x) + static_cast<int>(m_hpBarWidth),//始点の位置にバーの長さを足す
		static_cast<int>(m_hpBarPos.y + kBarHeight),//終点
		kHpBarColor, true);
	//血の量が見やすいように血の量の後ろに白いBoxを出す
	DrawBox(static_cast<int>(m_bloodBarPos.x), static_cast<int>(m_bloodBarPos.y),//始点
		static_cast<int>(m_bloodBarPos.x + kMaxBarWidth),//始点の位置にバーの長さを足す
		static_cast<int>(m_bloodBarPos.y + kBarHeight),//終点
		GetColor(0, 0, 0), true);
	//血の量バーの表示
	DrawBox(static_cast<int>(m_bloodBarPos.x), static_cast<int>(m_bloodBarPos.y),//始点
		static_cast<int>(m_bloodBarPos.x + m_bloodBarWidth),//始点の位置にバーの長さを足す
		static_cast<int>(m_bloodBarPos.y + kBarHeight),//終点
		kBloodBarColor, true);


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
	knockBack *= kKnockBackScale * enemy.GetSpd();
	//衝突点の座標
	m_hitPos = (enemy.GetPos() + m_pos) / 2;
	//白いエフェクトを出す
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_hitPos, //パーティクルの情報を入れる
			static_cast<float>(kWhiteParticleInfo[0]),static_cast<float>(kWhiteParticleInfo[1]), kWhiteParticleInfo[2], kWhiteParticleInfo[3]);
		m_pMain->AddParticle(m_pParticle);
	}
	m_nowHp -= enemy.GetAtk() - m_def;
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
	}
	enemy.HitPrincess(knockBack);
}

void Princess::ReceiveBlood(float blood)
{
	m_nowBlood += blood;
	if (blood > 0)
	{

		PlaySoundMem(m_passBloodSe, DX_PLAYTYPE_BACK);
	}
}

bool Princess::IsDeath()
{
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
		m_nowState = Game::State::kDelete;
		m_drawState = Game::WitchState::kBreak;
		m_animFrame = kDeathAnimFrame;
		return true;
	}
	return false;
}
