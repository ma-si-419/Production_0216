#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class Princess;
class Enemy;
class TreasureBox;
class Player : public Character
{

public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw() const;

	//�G�ɂԂ������ۂ̏���
	void HitEnemy(Enemy enemy);
	//�󔠂ɂԂ������Ƃ��̏���
	void HitTreasure(TreasureBox* treasureBox);
	//���݂�Hp�̊����v�Z����֐�
	float GetHpRate() { return m_nowHp / m_hp; }
	//�񕜗ʂ��v�Z����
	float GetHealRate() { return m_hp / 300; }
	//���ݎ����Ă��錌�̗ʂ̊������v�Z����
	float GetBloodRate() { return m_nowBlood / m_maxBlood; }
	//�|��Ă��邩�ǂ������擾����
	bool GetDeathFlag() { return m_deathFlag; }
	//�����Ă���S�[���h�̗ʂ��擾����
	int GetGold() { return m_gold; }
	//�����Ă���o���l�̗ʂ��擾����
	int GetExp() { return m_exp; }
	//�A�C�e�����擾�������̏���
	void PickUpItem(Game::ItemKinds kind);
	//�����Ă��錌��n���֐�
	void GiveBlood(Princess* princess);
private:
	//�����Ă��邩�ǂ���
	bool m_isMove;
	//Hp�o�[�̈ʒu
	Vec2 m_hpBarPos;
	//���̗ʂ�\���o�[�̈ʒu
	Vec2 m_bloodBarPos;
	//Hp�o�[�̒���
	float m_hpBarWidth;
	//���݂�HP�̗�
	float m_nowHp;
	//���̗ʃo�[�̒���
	float m_bloodBarWidth;
	//���ݎ����Ă��錌�̗�
	float m_nowBlood;
	//���Ă錌�̗�
	float m_maxBlood;
	//���ݓ|��Ă��邩�ǂ����t���O
	bool m_deathFlag;
	//�O�̃t���[���x�̃L�[�������Ă������ۑ�����ϐ�
	int m_lastPad;
	//�|�ꂽ���̍��W������ϐ�
	Vec2 m_deathPos;
	//���肵��������ۑ�����
	int m_gold;
	//���肵���o���l��ۑ�����
	int m_exp;
};