#pragma once
#include "Game.h"
#include "CircleCol.h"
#include "Vec2.h"

class Character
{
public:
	//�R���X�g���N�^
	Character();
	//�f�X�g���N�^
	virtual ~Character();
	//����������
	virtual void Init();
	//�X�V����
	virtual void Update();
	//�`�揈��
	virtual void Draw();
	//�����o�ϐ��ɃA�N�Z�X����
	virtual void SetHandle(int handle) { m_handle = handle; }
	//���݈ʒu���擾����
	virtual Vec2 GetPos()const { return m_pos; }
	//�U���͂��擾����
	virtual int GetAtk() { return m_atk; }
	//�����蔻����擾����
	virtual CircleCol GetColCircle() { return m_circleCol; }
protected:
	/*�����o�ϐ�*/
	//�U����
	int m_atk;
	//�̗�
	float m_hp;
	//���x
	float m_spd;
	//�h���
	int m_def;
	//�O���t�B�b�N�n���h��
	int m_handle;
	//�����蔻��̑傫��
	int m_radius;
	//�����蔻��
	CircleCol m_circleCol;
	//�ʒu���
	Vec2 m_pos;
	//�m�b�N�o�b�N
	Vec2 m_knockBack;
	//�m�b�N�o�b�N���Ă��鎞��
	int m_knockBackTime;
	//�A�j���[�V����
	int m_animFrame;
	//�����Ă��������ݒ肷��Ƃ��ɕK�v�ɂȂ�ϐ�
	int m_dirX;
	//�����Ă��������ݒ肷��Ƃ��ɕK�v�ɂȂ�ϐ�
	int m_dirY;


	//�����������Ă������
	Game::Dir m_dir;

	/*�����o�֐�*/
	void GetDir(int x, int y)
	{

		if (x == -1 && y == -1)
		{
			m_dir = Game::kDirLeftUp;
		}
		else if (x == 0 && y == -1)
		{
			m_dir = Game::kDirUp;
		}
		else if (x == 1 && y == -1)
		{
			m_dir = Game::kDirRightUp;
		}
		else if (x == 1 && y == 0)
		{
			m_dir = Game::kDirRight;
		}
		else if (x == 1 && y == 1)
		{
			m_dir = Game::kDirRightDown;
		}
		else if (x == 0 && y == 1)
		{
			m_dir = Game::kDirDown;
		}
		else if (x == -1 && y == 1)
		{
			m_dir = Game::kDirLeftDown;
		}
		else if (x == -1 && y == 0)
		{
			m_dir = Game::kDirLeft;
		}
	}
};