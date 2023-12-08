#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class Player;
class SceneMain;
class Enemy : public Character
{
public:
	enum Kinds
	{
		goblin,
		boar,
		doragon,
		skeleton,
		snowman
	};

public:
	Enemy(SceneMain* pMain,Player* pPlayer);
	virtual ~Enemy();
	//����������
	void Init(int kinds);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�G�����݂��Ă��邩�ǂ����𔻒f����
	bool IsExist() { return m_isExist; }
	//�v���C���[�ƂԂ������ۂ̏���
	void OnDamage(Player& player);
	//�v�����Z�X�ƂԂ������ۂ̃m�b�N�o�b�N
	void HitPrincess(Vec2 knockBack) { m_knockBack = knockBack; }
private:
	//SceneMain�̃|�C���^������
	SceneMain* m_pMain;
	//�v���C���[�̃|�C���^������
	Player* m_pPlayer;
	//�^�[�Q�b�g�̃|�W�V����
	Vec2 m_targetPos;
	//�G�̎��
	Kinds m_kind;
	//��������
	Vec2 m_moveVec;
	//���݂��Ă��邩�t���O
	bool m_isExist;
	//���������Ă��邩�t���O
	bool m_isLeftFlag;
};

