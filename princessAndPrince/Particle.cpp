#include "Particle.h"
#include "DxLib.h"

Particle::Particle(Vec2 pos, float lange,float speed,int scale, int color) :
	m_isExist(true),
	m_pos(pos),
	m_initialPos(pos),
	m_dirVec(static_cast<float>(GetRand(1000)) - 500, static_cast<float>(GetRand(1000)) - 500),
	m_scale(GetRand(scale) + 2),
	m_appearLange(GetRand(lange) + lange / 5),
	m_spd(speed),
	m_color(0)
{
	//数字に応じて色を変える
	switch (color)
	{
	case 0:
		m_color = GetColor(GetRand(30) + 225,GetRand(30) + 225,GetRand(30)+225);
		break;
	case 1:
		m_color = GetColor(GetRand(55) + 200, GetRand(100), 0);
		break;
	case 2:
		m_color = GetColor(GetRand(155) + 100, GetRand(155) + 100, GetRand(155) + 100);
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
		DrawCircle((int)m_pos.x, (int)m_pos.y, m_scale,m_color, true);
}
