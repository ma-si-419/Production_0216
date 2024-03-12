#pragma once
#include "CircleCol.h"
#include "Game.h"
#include "Vec2.h"
class Princess;
class MagicBase
{
public:
	//�R���X�g���N�^
	MagicBase(Princess* pPrincess,float scale);
	//�f�X�g���N�^
	virtual ~MagicBase();
	//����������
	void Init(int magicNum);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�����蔻����擾����
	CircleCol GetCircleCol() { return m_circleCol; }
	//�n���h�����擾����
	void SetHandle(int handle) { m_handle = handle; }
	//�U���͂��擾����
	float GetAtk();
	//��Ԃ��Ǘ�����
	Game::State m_nowState;
	//���@�̎�ނ��擾����
	bool GetMagicKind() { return m_isFire; }
private:
	//�v�����Z�X�̃|�C���^��ۑ�����ϐ�
	Princess* m_pPrincess;
	//�����̃|�W�V����
	Vec2 m_princessPos;
	//���@�̃|�W�V����
	Vec2 m_magicPos;
	//���@�̌������x�N�g��
	Vec2 m_moveVec;
	//�~�`�̓����蔻��
	CircleCol m_circleCol;
	//�����@�̍U����
	float m_fireAtk;
	//�����@�̍U����
	float m_windAtk;
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
	//�O���t�B�b�N
	int m_handle;
	//�摜�𔽓]�����邩�ǂ���
	bool m_turnFlag;
	//�摜�̊p�x
	float m_graphAngle;
};

