#pragma once
#include "CircleCol.h"
#include "Game.h"
#include "Vec2.h"
class Princess;
class MagicBase
{
public:
	//�R���X�g���N�^
	MagicBase(Princess* pPrincess);
	//�f�X�g���N�^
	virtual ~MagicBase();
	//����������
	void Init(int magicNum);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	
	//���݂��Ă��邩�ǂ������f����
	bool IsExist() { return m_isExist; }
	//�����蔻����擾����
	CircleCol GetCircleCol() { return m_circleCol; }
private:
	//�v�����Z�X�̃|�C���^��ۑ�����ϐ�
	Princess* m_pPrincess;
	//�����̃|�W�V����
	Vec2 m_princessPos;
	//���@�̃|�W�V����
	Vec2 m_magicPos;
	//���@�̌������x�N�g��
	Vec2 m_moveVec;
	//���݂��Ă��邩�ǂ���
	bool m_isExist;
	//�~�`�̓����蔻��
	CircleCol m_circleCol;
	//���@�̍U����
	int m_atk;
	//���@�̑傫��
	float m_scale;
	//���@�̎��
	bool m_isFire;
	//���@�̑��x
	float m_spd;
	//�����@���o�����Ɏg���ϐ�
	float m_windAngle;
	float m_windLength;
	//�v���C���[�̂ǂ���ɐ�������镗�����f����
	bool m_isRightWind;
};

