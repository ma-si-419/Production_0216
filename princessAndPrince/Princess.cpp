#include "Princess.h"
#include "CircleCol.h"
#include "DxLib.h"
#include "Game.h"
#include "Enemy.h"
#include "SceneMain.h"
#include "MagicBase.h"
#include "UserData.h"
namespace
{
	// �ړ����x
	constexpr float kSpeed = 3.0f;

	// �L�����N�^�[�̃T�C�Y
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	// �����̃T�C�Y
	constexpr float kGraphHalfWidth = kGraphWidth / 2.0f;
	constexpr float kGraphHalfHeight = kGraphHeight / 2.0f;

	// �L�����N�^�[�̃A�j���[�V����
	constexpr int kUseFrame[] = { 1,0 };
	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimFrameNum = 60;
	// �A�j���[�V�����̂P�T�C�N���̃t���[����
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//�m�b�N�o�b�N�̑傫��
	constexpr int kKnockBackScale = 6;
	//��{�I�Ȗ��@�̑傫��
	constexpr float kMagicScale = 20;
	//Hp�o�[�̉��̒���
	constexpr float kMaxBarWidth = 60;
	//Hp�o�[�̏c�̒���
	constexpr float kBarHeight = 5;
	//Hp�o�[�̃|�W�V����
	constexpr int kHpBarPosY = 40;
	//���Ă錌�̗ʂ̏��
	constexpr float kMaxBlood = 10;
	//�o�[�̌��ɕ\������Box�̗]���̑傫��
	constexpr int kBoxSpace = 3;
	//�����������Ă���������������̒���
	constexpr float kBarLen = 200;
	//�^���������Ă���ꍇ�̎��̐��l
	constexpr float kDownAngle = 4.71f;
	//���@��łԊu
	constexpr int kMagicInterval = 40;
	//�����ɏo�������@�̐�
	constexpr int kWindVol = 2;
	//�L�����̊g�嗦
	constexpr float kCharcterScale = 6.0f;
	//�x�[�X�̍U����
	constexpr float kBaseAtk = 1.0f;
	//�h���傫��
	constexpr int kShakeWidth = 2;
	//�h��鎞��
	constexpr int kShakeTime = 15;
	//�h���X�s�[�h
	constexpr float kshakeSpeed = 2.0f;
}
Princess::Princess(SceneMain* pMain) :
	m_hpBarWidth(0),
	m_bloodBarWidth(0),
	m_maxBlood(kMaxBlood),
	m_nowBlood(0),
	m_nowHp(m_hp),
	m_isLastKeyFlag(false),
	m_isFire(true),
	m_angle(kDownAngle),
	m_result(0, 0),
	m_MagicCount(0),
	m_isMagic(false),
	m_pMain(pMain),
	m_shakeTimeCount(kShakeTime),
	m_shakeSpeed(kshakeSpeed)
{
	m_pos.x = Game::kPlayScreenWidth / 2;
	m_pos.y = Game::kPlayScreenHeight / 2;
	m_basePos = m_pos;
	m_dir = Game::kDirDown;
	m_animFrame = kAnimFrameNum;
	m_radius = Game::kRadius;
}

Princess::~Princess()
{
}

void Princess::Init()
{
	m_hp = 50;
	m_nowHp = m_hp;
	m_atk = kBaseAtk;
	m_def = 0;
	m_scale = kMagicScale;
}

void Princess::Update()
{
	//�����蔻��̒��S���W��ݒ�
	m_circleCol.SetCenter(m_pos, m_radius);
	//HP�o�[�̏���//
	//���W���Q�l��Hp�o�[�̈ʒu��ݒ�
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	//	���ƂŃ}�W�b�N�i���o�[�ǂ��ɂ�����
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//���̗ʂ�����l�𒴂��Ă�����
	if (m_nowBlood >= kMaxBlood)
	{
		m_isMagic = true;
		m_nowBlood = kMaxBlood;
	}
	else if (m_nowBlood <= 0)
	{
		m_isMagic = false;
		m_nowBlood = 0;
	}
	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//���@��ł��Ă���Œ�
	if (m_isMagic || m_pMain->GetSpecialMode())
	{
		m_MagicCount++;
		//�{�^���̉�����Ă���󋵂��Ƃ�
		//RB�{�^����������Ă�����
		if (m_input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER])
		{
			//��]�����鏈��������
			m_angle -= 0.03f;

		}
		//LB�{�^����������Ă�����
		if (m_input.Buttons[XINPUT_BUTTON_LEFT_SHOULDER])
		{
			//��]�����鏈��������
			m_angle += 0.03f;
		}
		//�����@��I�����Ă���ꍇ
		if (m_isFire)
		{
			//�J�E���g���萔�𒴂�����
			if (m_MagicCount > kMagicInterval)
			{
				//�J�E���g���[���ɂ���
				m_MagicCount = 0;
				//���@������
				m_pMagic = new MagicBase(this, m_scale);
				m_pMagic->Init(0);
				m_pMain->AddMagic(m_pMagic);

			}
		}
		//�����@��I�����Ă���ꍇ
		else
		{
			//�����@�̃C���^�[�o���͒������邽�߂ɂQ�{����
			if (m_MagicCount > kMagicInterval * 2)
			{
				//�J�E���g���[���ɂ���
				m_MagicCount = 0;
				//�����@�͕��������ɏo��̂�
				for (int i = 0; kWindVol > i; i++)
				{
					m_pMagic = new MagicBase(this, m_scale);
					m_pMagic->Init(i);
					m_pMain->AddMagic(m_pMagic);
				}
			}
		}

		//�����Ă��錌�̗ʂ����炵�Ă���
		m_nowBlood -= 0.02f;

	}
	//A�{�^���������ꂽ��
	if (m_input.Buttons[XINPUT_BUTTON_A] && !m_isLastKeyFlag || CheckHitKey(KEY_INPUT_Z))
	{
		//�A���Ő؂�ւ��Ȃ��悤��
		m_isLastKeyFlag = true;
		//���@�̎�ނ�ύX����
		m_isFire = !m_isFire;
		//���@�����Ԋu�����Z�b�g����
		m_MagicCount = 0;
	}
	else if (!m_input.Buttons[XINPUT_BUTTON_A])
	{
		//�A���Ő؂�ւ��Ȃ��悤��
		m_isLastKeyFlag = false;
	}
	//���̗ʃo�[�̏���
	//���Hp�o�[���������Ⴂ���W�ɕ\��������
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//���̗ʂ̊������v�Z���ăo�[�̒������Z�o����
	m_bloodBarWidth = kMaxBarWidth * GetBloodRate();
	//�������Ă���������v�Z����
	m_result.x = cosf(m_angle);
	m_result.y = sinf(m_angle);
	m_result *= kBarLen;
	m_magicVec.x = m_pos.x + m_result.x;
	m_magicVec.y = m_pos.y - m_result.y;
	//�������[�h�̏���
	if (m_pMain->GetSpecialMode())
	{
		m_atk = kBaseAtk * 1.5f;
		m_scale = kMagicScale * 2.0f;
	}
	else
	{
		m_atk = kBaseAtk;
		m_scale = kMagicScale;
	}
	//����������
	m_shakeTimeCount++;
	if (m_pos.x < m_basePos.x - kShakeWidth)
	{
		m_shakeSpeed *= -1;
	}
	else if (m_pos.x > m_basePos.x + kShakeWidth)
	{
		m_shakeSpeed *= -1;
	}
	if (m_shakeTimeCount < kShakeTime)
	{
		m_pos.x += m_shakeSpeed;
	}
	else
	{
		m_pos = m_basePos;
	}
	//�|�[�Y����߂����Ƃ��ɖ��@���؂�ւ��Ȃ��悤��
	if (m_pMain->GetPause())
	{
		m_isLastKeyFlag = true;
	}
}

void Princess::Draw() const
{
	int animEle = m_animFrame / kAnimFrameNum;

	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;

	//���@��ł����ɐ���\������
	if (m_isMagic)
	{
		DrawLine(m_pos.x, m_pos.y,//�n�_
			m_pos.x + m_result.x, m_pos.y - m_result.y,//�v�Z���ʂ��n�_�ɑ����ďI�_�ɂ���
			GetColor(0, 0, 0));
	}
	//�摜�̂ǂ���؂��邩���w�肵�āA�؂������摜��\������
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,//�؂���ʒu
		kGraphWidth, kGraphHeight,//�؂���傫��
		kCharcterScale,//�g�嗦
		0.0,//��]��
		m_handle, true, false);
	//Hp�������₷�����邽�߂Ɍ���Box�쐬
	DrawBox((int)m_hpBarPos.x - kBoxSpace, (int)m_hpBarPos.y - kBoxSpace,//�n�_
		(int)m_bloodBarPos.x + kMaxBarWidth + kBoxSpace,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		(int)m_bloodBarPos.y + (int)kBarHeight + kBoxSpace,//�I�_
		GetColor(75, 75, 75), true);
	//Hp�����₷���悤��Hp�̌��ɍ���Box���o��
	DrawBox((int)m_hpBarPos.x, (int)m_hpBarPos.y,//�n�_
		(int)m_hpBarPos.x + (int)kMaxBarWidth,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		(int)m_hpBarPos.y + (int)kBarHeight,//�I�_
		GetColor(0, 0, 0), true);
	//HP�o�[�̕\��
	DrawBox((int)m_hpBarPos.x, (int)m_hpBarPos.y,//�n�_
		(int)m_hpBarPos.x + (int)m_hpBarWidth,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		(int)m_hpBarPos.y + (int)kBarHeight,//�I�_
		GetColor(255, 255, 100), true);
	//���̗ʂ����₷���悤�Ɍ��̗ʂ̌��ɔ���Box���o��
	DrawBox((int)m_bloodBarPos.x, (int)m_bloodBarPos.y,//�n�_
		(int)m_bloodBarPos.x + (int)kMaxBarWidth,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		(int)m_bloodBarPos.y + (int)kBarHeight,//�I�_
		GetColor(0, 0, 0), true);
	//���̗ʃo�[�̕\��
	DrawBox((int)m_bloodBarPos.x, (int)m_bloodBarPos.y,//�n�_
		(int)m_bloodBarPos.x + (int)m_bloodBarWidth,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		(int)m_bloodBarPos.y + (int)kBarHeight,//�I�_
		GetColor(255, 0, 0), true);


#ifdef _DEBUG
	m_circleCol.Draw(m_radius, 0x0000ff, false);
#endif
}

void Princess::HitEnemy(Enemy& enemy)
{
	m_shakeTimeCount = 0;
	Vec2 knockBack;
	knockBack = m_pos - enemy.GetPos();
	knockBack.Normalize();
	knockBack *= kKnockBackScale;
	m_nowHp -= enemy.GetAtk() - m_def;
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
	}
	enemy.HitPrincess(knockBack);
}

bool Princess::IsDeath()
{
	if (m_nowHp <= 0)
	{
		return true;
	}
	return false;
}
