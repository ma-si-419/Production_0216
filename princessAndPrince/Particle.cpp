#include "Particle.h"
#include "DxLib.h"
namespace
{
	//�p�[�e�B�N���̕���
	constexpr int kParticleVec = 1000;
	//�p�[�e�B�N���̍Œ���̑傫��
	constexpr int kMinimumScale = 2;
	//�p�[�e�B�N���̍Œ���̒���(���͂��ꂽ�������Q�Ƃ���)
	constexpr int kShortestLange = 5;
	//���ɕ\������T�[�N���̑傫��(�p�[�e�B�N���̃X�P�[�����Q�Ƃ���)
	constexpr int kBackCircleScale = 2;
	//�����p�[�e�B�N���̊�{�I�ȐF
	const int kWhiteBaseColor = GetColor(225, 225, 225);
	//�Ԃ��p�[�e�B�N���̊�{�I�ȐF
	const int kRedBaseColor = GetColor(200, 0, 0);
	//���F�̃p�[�e�B�N���̊�{�I�ȐF
	const int kRainbowBaseColor = GetColor(100, 100, 100);
	//�����p�[�e�B�N���̐F�̕�
	constexpr int kWhiteRandomColor[3] = { 30,30,30 };
	//�Ԃ��p�[�e�B�N���̐F�̕�
	constexpr int kRedRandomColor[3] = { 55,100,0 };
	//���F�̃p�[�e�B�N���̐F�̕�
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
	//�����ɉ����ĐF��ς���
	switch (color)
	{
	case 0:
		//��
		m_color = kWhiteBaseColor;
		//�����F�ɕ�����������
		randomColorRange = GetColor(GetRand(kWhiteRandomColor[0]), GetRand(kWhiteRandomColor[1]), GetRand(kWhiteRandomColor[2]));
		//�F����������
		m_color += randomColorRange;
		m_isWhite = true;
		break;
	case 1:
		//��
		m_color = kRedBaseColor;
		//�����F�ɕ�����������
		randomColorRange = GetColor(GetRand(kRedRandomColor[0]), GetRand(kRedRandomColor[1]), GetRand(kRedRandomColor[2]));
		//�F����������
		m_color += randomColorRange;
		break;
	case 2:
		//��
		m_color = kRainbowBaseColor;
		//�����F�ɕ�����������
		randomColorRange = GetColor(GetRand(kRainbowRandomColor[0]), GetRand(kRainbowRandomColor[1]), GetRand(kRainbowRandomColor[2]));
		//�F����������
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
		//�ړ�������ۑ�����
		float moveDistance;
		//�v�Z�p�̃x�N�g��
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
	//���ɍ����T�[�N�����o���g����\������(�����X�e�[�W�Ō����ɂ�����������)
	DrawCircle((int)m_pos.x, (int)m_pos.y, m_backCircleScale, GetColor(0, 0, 0), true, 1);
	//�p�[�e�B�N���̕\��
	DrawCircle((int)m_pos.x, (int)m_pos.y, m_scale, m_color, true, 1);
}
