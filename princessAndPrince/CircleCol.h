#pragma once
#include "Vec2.h"

/// <summary>
/// �~�`���Ǘ�����N���X
/// </summary>
class CircleCol
{
public:
	CircleCol();
	virtual ~CircleCol();

	// �`��
	void Draw(int radius, unsigned int color, bool isFill) const;

	/// <summary>
	/// ���S���W�Ɣ��a��ۑ�
	/// </summary>
	/// <param name="pos">���S���W: x</param>
	/// <param name="radius">���a</param>
	void SetCenter(Vec2 pos, float radius);

	Vec2 GetCenter() const;		// �~�`�̒��S���W

	/// <summary>
	/// �~�`���m�̓����蔻��
	/// </summary>
	/// <param name="target">���葤</param>
	/// <returns>true : ��������, false : �������ĂȂ�</returns>
	bool IsCollsion(const CircleCol& target);
	
	float GetRange(Vec2 pos, Vec2 targetPos);
private:
	//�~�`�̒����̍��W
	Vec2 m_pos;
	//�~�̔��a
	int m_radius;
};
