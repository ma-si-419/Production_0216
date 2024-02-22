#include "Particle.h"
#include "DxLib.h"
namespace
{
	//パーティクルの方向
	constexpr int kParticleVec = 1000;
	//パーティクルの最低限の大きさ
	constexpr int kMinimumScale = 2;
	//パーティクルの最低限の長さ(入力された長さを参照する)
	constexpr int kShortestLange = 5;
	//後ろに表示するサークルの大きさ(パーティクルのスケールを参照する)
	constexpr int kBackCircleScale = 2;
	//白いパーティクルの基本的な色
	const int kWhiteBaseColor = GetColor(225, 225, 225);
	//赤いパーティクルの基本的な色
	const int kRedBaseColor = GetColor(200, 0, 0);
	//虹色のパーティクルの基本的な色
	const int kRainbowBaseColor = GetColor(100, 100, 100);
	//白いパーティクルの色の幅
	constexpr int kWhiteRandomColor[3] = { 30,30,30 };
	//赤いパーティクルの色の幅
	constexpr int kRedRandomColor[3] = { 55,100,0 };
	//虹色のパーティクルの色の幅
	constexpr int kRainbowRandomColor[3] = { 155, 155, 155 };
}
Particle::Particle(Vec2 pos, float lange, float speed, int scale, int color) :
	m_isExist(true),
	m_pos(pos),
	m_initialPos(pos),
	m_dirVec(static_cast<float>(GetRand(kParticleVec)) - kParticleVec / 2, static_cast<float>(GetRand(kParticleVec)) - kParticleVec / 2),
	m_scale(GetRand(scale) + kMinimumScale),
	m_appearLange(static_cast<float>(GetRand(static_cast<int>(lange)) + (lange / kShortestLange))),
	m_spd(speed),
	m_color(0),
	m_isWhite(false),
	m_backCircleScale(m_scale + kBackCircleScale)
{
	int randomColorRange;
	//数字に応じて色を変える
	switch (color)
	{
	case 0:
		//白
		m_color = kWhiteBaseColor;
		//少し色に幅を持たせる
		randomColorRange = GetColor(GetRand(kWhiteRandomColor[0]), GetRand(kWhiteRandomColor[1]), GetRand(kWhiteRandomColor[2]));
		//色を合成する
		m_color += randomColorRange;
		m_isWhite = true;
		break;
	case 1:
		//赤
		m_color = kRedBaseColor;
		//少し色に幅を持たせる
		randomColorRange = GetColor(GetRand(kRedRandomColor[0]), GetRand(kRedRandomColor[1]), GetRand(kRedRandomColor[2]));
		//色を合成する
		m_color += randomColorRange;
		break;
	case 2:
		//虹
		m_color = kRainbowBaseColor;
		//少し色に幅を持たせる
		randomColorRange = GetColor(GetRand(kRainbowRandomColor[0]), GetRand(kRainbowRandomColor[1]), GetRand(kRainbowRandomColor[2]));
		//色を合成する
		m_color += randomColorRange;
		break;
	default:
		break;
	}
}

Particle::~Particle()
{
}

void Particle::Init()
{
}

void Particle::Update()
{
	m_dirVec.Normalize();
	Vec2 moveVec = m_dirVec * m_spd;
	if (m_isExist)
	{
		m_pos += moveVec;
		//移動距離を保存する
		float moveDistance;
		//計算用のベクトル
		Vec2 ansVector = m_initialPos - m_pos;
		moveDistance = ansVector.Length();
		if (moveDistance > m_appearLange)
		{
			m_isExist = false;
		}
	}
}

void Particle::Draw()
{
	//後ろに黒いサークルを出し枠線を表示する(白いステージで見えにくかったから)
	DrawCircle((int)m_pos.x, (int)m_pos.y, m_backCircleScale, GetColor(0, 0, 0), true, 1);
	//パーティクルの表示
	DrawCircle((int)m_pos.x, (int)m_pos.y, m_scale, m_color, true, 1);
}
