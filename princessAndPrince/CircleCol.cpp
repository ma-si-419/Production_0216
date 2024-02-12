#include "CircleCol.h"
#include <DxLib.h>
CircleCol::CircleCol()
{

}

CircleCol::~CircleCol()
{
}

void CircleCol::Draw(int radius, unsigned int color, bool isFill) const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),radius,
		color, isFill);
}

void CircleCol::SetCenter(Vec2 pos, float radius)
{
	m_pos = pos;
	m_radius = static_cast<int>(radius);
}

Vec2 CircleCol::GetCenter() const
{
	return Vec2();
}

bool CircleCol::IsCollsion(const CircleCol& target)
{
	//���S���W���m�̋����𑪂�A���̋�������̔��a�����킹���l����
	//�������Ȃ�����true��Ԃ��B
	if (GetRange(m_pos, target.m_pos) < m_radius + target.m_radius)
	{
		return true;
	}
	return false;
}

float CircleCol::GetRange(Vec2 pos, Vec2 targetPos)
{
	//�Ԃ�l������p�̕ϐ�
	float ans;
	//�v�Z�p�̃x�N�g��
	Vec2 ansVector;
	ansVector = (targetPos - pos);
	ans = ansVector.Length();
	return ans;
}
