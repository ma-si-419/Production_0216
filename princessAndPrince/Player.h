#pragma once
#include "Vec2.h"
#include "CircleCol.h"
#include "Character.h"
#include <memory>
class Princess;
class Enemy;
class TreasureBox;
class SceneMain;
class ItemBase;
class Player : public Character
{

public:
	Player(SceneMain* pMain);
	~Player();

	void Init();
	void Update();
	void Draw() const;

	//�G�ɂԂ������ۂ̏���
	void HitEnemy(Enemy enemy,bool weak);
	//�󔠂ɂԂ������Ƃ��̏���
	void HitTreasure(TreasureBox* treasureBox);
	//���݂�Hp�̊����v�Z����֐�
	float GetHpRate() { return m_nowHp / m_hp; }
	//�񕜗ʂ��v�Z����
	float GetHealRate() { return m_hp / 300; }
	//���ݎ����Ă��錌�̗ʂ̊������v�Z����
	float GetBloodRate() { return m_nowBlood / m_maxBlood; }
	//�|��Ă��邩�ǂ������擾����
	bool GetDeathFlag() { return m_isDeathFlag; }
	//�����Ă���S�[���h�̗ʂ��擾����
	int GetGold() { return m_gold; }
	//�Q�[���I�[�o�[�̎��͎����Ă���S�[���h�𔼕����炷
	void SubGoldGameOver() { m_gold *= 0.5; }
	//�Q�[���I�[�o�[�̎��͎����Ă���o���l�𔼕����炷
	void SubExpGameOver() { m_exp *= 0.5; }
	//�O���t�n���h����Ԃ�
	int GetHandle() { return m_handle; }
	//�����Ă���S�[���h�����炷
	void SubGold(int subNum) { m_gold -= subNum; }
	//�����Ă���o���l�̗ʂ��擾����
	int GetExp() { return m_exp; }
	//�����Ă���o���l�����炷
	void SubExp(int subNum) { m_exp-= subNum; }
	//�h��͂��擾����
	float GetDef() { return m_def; }
	//�A�C�e�����擾�������̏���
	void PickUpItem(std::shared_ptr<ItemBase> item);
	//�o���l�𑝂₷�֐�
	void PickUpExp(int exp) { m_exp += exp; }
	//�S�[���h�𑝂₷�֐�
	void PickUpGold(int gold) { m_gold += gold; }
	//�����Ă��錌��n���֐�
	void GiveBlood(Princess* princess);
	//���񂾂Ƃ��̏������쐬����
	void DeathMove();
	//�N���A�������̉��o
	void ClearDance();
	//�����Ă��������O���ɂ���
	void TurnFront() { m_dir = Game::kDirDown; }
	//�������Ă���x�N�g�����擾����
	Vec2 GetMoveVec() { return m_moveVec; }
	//�{�胂�[�h���n�߂�Ƃ��ɂ��
	void SetSpecialMode() { m_dir = Game::kDirDeath; m_animFrame = 24; }
	void StartSpecialMode() { m_dir = Game::kDirDown; m_animFrame = 0; }
	//�{�胂�[�h���̃O���t�B�b�N
	void SetPlayerAngryGraph(int graph) { m_playerAngryGraph = graph; }
	void SetAngryFireGraph(int graph) { m_angryFireGraph = graph; }
private:
	//�V�[�����C���̃|�C���^������
	SceneMain* m_pMain;
	//�����Ă��邩�ǂ���
	bool m_isMove;
	//Hp�o�[�̈ʒu
	Vec2 m_hpBarPos;
	//���̗ʂ�\���o�[�̈ʒu
	Vec2 m_bloodBarPos;
	//�ړ��x�N�g��
	Vec2 m_moveVec;
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
	bool m_isDeathFlag;
	//�O�̃t���[���x�̃L�[�������Ă������ۑ�����ϐ�
	int m_lastPad;
	//�|�ꂽ���̍��W������ϐ�
	Vec2 m_deathPos;
	//���肵��������ۑ�����
	int m_gold;
	//���肵���o���l��ۑ�����
	int m_exp;
	//�������鎞�Ԃ��J�E���g����ϐ�
	int m_revivalCount;
	//�N���A���̉��o�̎��ɃJ�E���g����ϐ�
	int m_danceCount;
	//�N���A���̉��o�ŉ���������J�E���g����
	int m_turnCount;
	//�R�C�����E�������̌��ʉ�
	int m_coinSe;
	//�o���l���E�������̌��ʉ�
	int m_expSe;
	//�|�[�V�������Ƃ������̌��ʉ�
	int m_portionSe;
	//�����E�������̌��ʉ�
	int m_bloodSe;
	//�{�胂�[�h���̃O���t�B�b�N
	int m_playerAngryGraph;
	//�{�胂�[�h���Ɍ��ɏo���O���t�B�b�N
	int m_angryFireGraph;
	//���𔽓]�����邩�ǂ���
	bool m_isAngryFireReverseFlag;
};