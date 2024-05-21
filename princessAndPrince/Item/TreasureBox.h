#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
class SceneMain;
class Player;
class Particle;
class TreasureBox
{
public:
	//�R���X�g���N�^
	TreasureBox(SceneMain* sceneMain);
	//�f�X�g���N�^
	~TreasureBox();
	//����������
	void Init(Vec2 pos);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�󔠂̗̑͂̌��炷
	void SubHp() { m_hp--; }
	//�󔠂̗̑͂����炷
	int GetHp() { return m_hp; }
	//Player�ƂԂ������Ƃ�
	void HitPlayer(Player* player);
	//���@�ƂԂ������Ƃ�
	void HitMagic();
	//�����̏ꏊ��Ԃ�
	Vec2 GetPos() { return m_pos; }
	//�����蔻���Ԃ�
	CircleCol GetColCircle() { return m_circleCol; }
	//�m�b�N�o�b�N�̑傫����Ԃ�
	float GetKnockBackPow() { return m_knockBackPow; }
	//�O���t�B�b�N�n���h����ݒ肷��
	void SetHandle(int handle) { m_handle = handle; }
	//�o���l�Ƃ����̗ʂ�ݒ肷��
	void SerGoldAndExp(int gold, int exp) { m_gold = gold; m_exp = exp; }
	//��Ԃ��Ǘ�����
	Game::State m_nowState;


private:
	enum Dropkind  
	{
		kPortion,
		kExp,
		kGold,
		kBlood
	};
	//SceneMain�̃|�C���^
	SceneMain* m_pMain;
	//�\���ʒu
	Vec2 m_pos;
	//�v���C���[�ƏՓ˂����_�̍��W
	Vec2 m_hitPos;
	//�p�[�e�B�N���̃|�C���^
	Particle* m_pParticle;
	//�O���t�B�b�N�n���h��
	int m_handle;
	//�����蔻��
	CircleCol m_circleCol;
	//���݂��Ă��邩�ǂ���
	bool m_isExist;
	//�󔠂̗̑�
	int m_hp;
	//�����蔻��̑傫��
	int m_colScale;
	//���@�ɓ��������Ƃ��ɃJ�E���g����ϐ�
	int m_magicCount;
	//�m�b�N�o�b�N�̃x�N�g��
	Vec2 m_knockBackVec;
	//�m�b�N�o�b�N���鎞�Ԃ��J�E���g����ϐ�
	int m_knockBackTime;
	//�m�b�N�o�b�N�̑傫�������߂�ϐ�
	float m_knockBackPow;
	//�o�Ă��邨���̋��z
	int m_gold;
	//�o�Ă���o���l�̗�
	int m_exp;
};
