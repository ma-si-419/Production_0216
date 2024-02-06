#pragma once
#include "Vec2.h"
class Particle
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�\���ʒu</param>
	/// <param name="lange">�ǂ�ʈړ�����������邩</param>
	/// <param naem="speed">�ړ����x</param>
	/// <param naem="speed">�傫��</param>
	/// <param name="color">�F�̕� 0.��1.��2.���F</param>
	Particle(Vec2 pos,float lange,float speed,int scale,int color);
	//�f�X�g���N�^
	~Particle();
	//����������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//���݂��Ă��邩�ǂ�����Ԃ�
	bool GetIsExist() { return m_isExist; }

private:
	//�����ʒu
	Vec2 m_initialPos;
	//�\���ʒu
	Vec2 m_pos;
	//�ړ���
	Vec2 m_dirVec;
	//�傫��
	int m_scale;
	//�\�����锼�a
	float m_appearLange;
	//���݂��Ă��邩�ǂ����t���O
	bool m_isExist;
	//�F
	int m_color;
	//�ړ����x
	float m_spd;
	//�����p�[�e�B�N�����ǂ���
	bool m_isWhite;
};

