#pragma once
#include <memory>
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
class SceneMain;
class MagicBase;
class Player;
class TreasureBox;
class Particle;
class Enemy : public Character
{
public:
	enum Kinds
	{
		goblin,
		boar,
		doragon,
		skeleton,
		snowman,
		bossGoblin,
		bossBoar,
		bossDoragon,
		bossSkeleton,
		bossSnowman
	};

public:
	Enemy(SceneMain* pMain);
	virtual ~Enemy();
	//����������
	void Init(int kinds);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�v���C���[�ƂԂ������ۂ̏���
	void HitPlayer(Player& player,bool weak);
	//���@�ƂԂ������Ƃ��̏���
	void HitMagic(MagicBase* magic);
	//�v�����Z�X�ƂԂ������ۂ̃m�b�N�o�b�N
	void HitPrincess(Vec2 knockBack) { m_knockBack = knockBack; }
	//��_�̓����蔻���Ԃ�
	CircleCol GetWeakCircle() { return m_weakCircle; }
private:
	//SceneMain�̃|�C���^������
	SceneMain* m_pMain;
	//�^�[�Q�b�g�̃|�W�V����
	Vec2 m_targetPos;
	//�G�̎��
	Kinds m_kind;
	//��������
	Vec2 m_moveVec;
	//���������Ă��邩�t���O
	bool m_isLeftFlag;
	//���@��v���C���[�ɘA���Ńq�b�g���Ȃ����߂ɃJ�E���g��p�ӂ���
	int m_hitCount;
	//�󔠂̃|�C���^
	TreasureBox* m_pTreasureBox;
	//�傫���L�������ǂ����𔻒f����
	bool m_isBoss;
	//�L�����̑傫��
	float m_scale;
	//�g���摜��Y���W������
	int m_srcY;
	//��_�̓����蔻����Ƃ�
	CircleCol m_weakCircle;
	//��_�̒��S���W
	Vec2 m_weakPos;
	//���������Ƃ��ɂǂ̃G�t�F�N�g���o�������f����
	bool m_isHitFlag;
	//��_�ɓ����������ǂ����𔻒f����
	bool m_isHitWeakFlag;
	//�Փ˂����_�̍��W
	Vec2 m_hitPos;
	//�G�t�F�N�g���o�����Ԃ��J�E���g����(��ŏ�������)
	int m_effectCount;
	//�����Ă��邨���̗�
	int m_haveGold;
	//�����Ă���o���l�̗�
	int m_haveExp;
	//�p�[�e�B�N���̃|�C���^
	Particle* m_pParticle;
};

