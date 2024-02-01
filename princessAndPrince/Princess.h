#pragma once
#include"Vec2.h"
#include"CircleCol.h"
#include"Game.h"
#include"Character.h"
class Enemy;
class SceneMain;
class MagicBase;
class Particle;
class Princess : public Character
{
public:
	Princess(SceneMain* pMain);
	virtual ~Princess();

	void Init();
	void Update();
	void Draw() const;
	//�_���[�W��H������Ƃ��̏���
	void HitEnemy(Enemy& enemy);
	//���݂�Hp�̊����v�Z����֐�
	float GetHpRate() { return m_nowHp / m_hp; }
	//���ݎ����Ă��錌�̗ʂ̊������v�Z����
	float GetBloodRate() { return m_nowBlood / m_maxBlood; }
	//�����Ă��錌�̗ʂ��擾����
	float GetBlood() { return m_nowBlood; }
	//���@�̃n���h���ݒ肷��
	void SetMagicHandle(int handle) { m_magicHandle = handle; }
	//Player���猌���󂯎��Ƃ��Ɏg��
	void ReceiveBlood(float blood);
	//�̗͂�0���ǂ������f����֐�
	bool IsDeath();
	//�����@��ł��Ă��邩���擾����
	bool IsMagic() { return m_isMagic; }
	//���@��ł��Ă���p�x���擾����
	Vec2 GetMagicDir() { return m_magicVec; }
	//���@�̎�ނ��擾����
	bool GetMagicKind() { return m_isFire; }
	//�U���͂��擾����
	float GetAtk() { return m_atk; }
	//�΂̏�Ԃɂ���
	void TransStone() {m_drawState = Game::kStone; }
	//�������[�h���n�߂�Ƃ��ɌĂ�
	void StartSpecialMode() { m_drawState = Game::kMagic; m_animFrame = 0; }
private:
	//���C���V�[���̃|�C���^��ۑ�����ϐ�
	SceneMain* m_pMain;
	//�p�[�e�B�N���̃|�C���^
	Particle* m_pParticle;
	//Hp�o�[�̈ʒu
	Vec2 m_hpBarPos;
	//���̗ʃo�[�̂���
	Vec2 m_bloodBarPos;
	//���@�̃|�C���^��ۑ�����ϐ�
	MagicBase* m_pMagic;
	//���ݎ����Ă��錌�̗�
	float m_nowBlood;
	//���̗ʂ̍ő�l
	float m_maxBlood;
	//���̗ʃo�[�̒���
	float m_bloodBarWidth;
	//Hp�o�[�̒���
	float m_hpBarWidth;
	//���݂�HP�̗�
	float m_nowHp;
	//�����@��ł��Ă��邩
	bool m_isMagic;
	//���ǂ����̖��@��ł��Ă��邩���f����ϐ�
	bool m_isFire;
	//�O�̃t���[�����L�[�������Ă������ۑ�����ϐ�
	bool m_isLastKeyFlag;
	//�v�Z���ʂ�ۑ�����ϐ�
	Vec2 m_result;
	//�����Ă�������̊p�x
	float m_angle;
	//���@��łԊu���J�E���g����ϐ�
	int m_MagicCount;
	//���@�������̏I�_��ۑ�����ϐ�
	Vec2 m_magicVec;
	//���@�̑傫��
	float m_scale;
	//�G�ɂԂ����ėh��鎞�Ԃ��J�E���g����
	int m_shakeTimeCount;
	//�Ԃ��������ɗ��t���O
	bool m_hitFlag;
	//��{�I�Ȗ����̈ʒu
	Vec2 m_basePos;
	//�h���X�s�[�h
	float m_shakeSpeed;
	//�G�l�~�[�Ƃ̏Փ˓_
	Vec2 m_hitPos;
	//�����̕`�悷���Ԃ�\��
	Game::WitchState m_drawState;
	//���@�̃O���t�B�b�N�n���h��
	int m_magicHandle;
	//�����@�̌��ʉ�
	int m_windMagicSe;
	//�v���C���[���猌���󂯎�������ɂȂ炷���ʉ�
	int m_passBloodSe;
	//�����@�̌��ʉ�
	int m_fireMagicSe;
};

