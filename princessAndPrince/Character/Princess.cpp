#include "Princess.h"
#include "CircleCol.h"
#include "DxLib.h"
#include "Game.h"
#include "Enemy.h"
#include "SceneMain.h"
#include "MagicBase.h"
#include "UserData.h"
#include "Particle.h"
namespace
{
	// �ړ����x
	constexpr float kSpeed = 3.0f;

	// �L�����N�^�[�̃T�C�Y
	constexpr int kGraphWidth = 24;
	constexpr int kGraphHeight = 24;
	// �����̃T�C�Y
	constexpr float kGraphHalfWidth = kGraphWidth / 2.0f;
	constexpr float kGraphHalfHeight = kGraphHeight / 2.0f;

	// �L�����N�^�[�̃A�j���[�V����
	constexpr int kUseFrame[] = { 1,0 };
	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimFrameNum = 30;
	// �A�j���[�V�����̂P�T�C�N���̃t���[����
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//�m�b�N�o�b�N�̑傫��
	constexpr int kKnockBackScale = 28;
	//��{�I�Ȗ��@�̑傫��
	constexpr float kMagicScale = 15;
	//Hp�o�[�̉��̒���
	constexpr float kMaxBarWidth = 60;
	//Hp�o�[�̏c�̒���
	constexpr float kBarHeight = 5;
	//Hp�o�[�̃|�W�V����
	constexpr int kHpBarPosY = 60;
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
	constexpr float kCharcterScale = 5.0f;
	//�x�[�X�̍U����
	constexpr float kBaseAtk = 0.5f;
	//�h���傫��
	constexpr int kShakeWidth = 2;
	//�h��鎞��
	constexpr int kShakeTime = 15;
	//�h���X�s�[�h
	constexpr float kShakeSpeed = 2.0f;
	//�p�[�e�B�N���̐�
	constexpr int kParticleVol = 30;
	//Hp�ƌ��̗ʂ�\������{�b�N�X�̐F
	const int kBoxColor = GetColor(75, 75, 75);
	//Hp�o�[�̐F
	const int kHpBarColor = GetColor(255, 255, 100);
	//���̗ʃo�[�̐F
	const int kBloodBarColor = GetColor(255, 0, 0);
	//�{�胂�[�h�������̍U����
	constexpr float kAngryModeMagicAtk = kBaseAtk * 1.5f;
	//�{�胂�[�h���������̑傫��
	constexpr float kAngryModeMagicScale = kMagicScale * 2.0f;
	//�G�ƂԂ��������ɏo���p�[�e�B�N���̏��
	constexpr int kWhiteParticleInfo[4] = { 40,4,5,0 };
	//���񂾂Ƃ��̃A�j���t���[��
	constexpr int kDeathAnimFrame = 24;
	//�����Ă������̗�
	constexpr float kSubBloodVol = 0.02f;
	//��]����X�s�[�h
	constexpr float kRotaSpeed = 0.03f;
	//���@���g����悤�ɂȂ�X�e�[�W
	constexpr int kUseMagicStage = 1;
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
	m_shakeSpeed(kShakeSpeed),
	m_drawState(Game::WitchState::kStone),
	m_arrowGraph(0),
	m_fireMagicSe(0),
	m_hitFlag(false),
	m_magicArrowGraph(0),
	m_magicHandle(0),
	m_pMagic(),
	m_pParticle(),
	m_passBloodSe(0),
	m_scale(0),
	m_windMagicSe(0)

{
	m_pos.x = Game::kPlayScreenWidth / 2;
	m_pos.y = Game::kPlayScreenHeight / 2;
	m_basePos = m_pos;
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
	//���W���Q�l��Hp�o�[�̈ʒu��ݒ�
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	//Hp�̊������v�Z���ăo�[�̒������Z�o����
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//���̗ʃo�[�̏���
	//���Hp�o�[���������Ⴂ���W�ɕ\��������
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//�����@�̌��ʉ�������
	m_windMagicSe = m_pMain->GetWindMagicSe();
	//�����@�̌��ʉ�������
	m_fireMagicSe = m_pMain->GetFireMagicSe();
	m_passBloodSe = m_pMain->GetPassBloodSe();
	//���@��؂�ւ������̌��ʉ�������
	m_changeMagicSe = m_pMain->GetChangeMagicSe();
}

void Princess::Update()
{
	//�����蔻��̒��S���W��ݒ�
	m_circleCol.SetCenter(m_pos, static_cast<float>(m_radius));
	//HP�o�[�̏���//
	//���W���Q�l��Hp�o�[�̈ʒu��ݒ�
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	//	���ƂŃ}�W�b�N�i���o�[�ǂ��ɂ�����
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	if (m_nowState != Game::State::kDelete)
	{

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
		if (m_isMagic || m_pMain->GetAngryMode())
		{
			//�΂̏�Ԃ��璼��
			m_drawState = Game::WitchState::kMagic;
			//�A�j���[�V�����𓮂���
			m_animFrame++;
			//�X�y�V�������[�h���������瓮���𑁂�����
			if (m_pMain->GetAngryMode())m_animFrame++;
			if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 1;
			//���@�����Ԋu���v��
			m_MagicCount++;
			//�{�^���̉�����Ă���󋵂��Ƃ�
			//RB�{�^����������Ă�����
			if (m_input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] || CheckHitKey(KEY_INPUT_L))
			{
				//��]�����鏈��������
				m_angle -= kRotaSpeed;

			}
			//LB�{�^����������Ă�����
			if (m_input.Buttons[XINPUT_BUTTON_LEFT_SHOULDER] || CheckHitKey(KEY_INPUT_J))
			{
				//��]�����鏈��������
				m_angle += kRotaSpeed;
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
					m_pMagic->SetHandle(m_magicHandle);
					m_pMain->AddMagic(m_pMagic);
					PlaySoundMem(m_fireMagicSe, DX_PLAYTYPE_BACK);

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
						m_pMagic->SetHandle(m_magicHandle);
						m_pMain->AddMagic(m_pMagic);
						PlaySoundMem(m_windMagicSe, DX_PLAYTYPE_BACK);
					}
				}
			}

			//�����Ă��錌�̗ʂ����炵�Ă���
			m_nowBlood -= kSubBloodVol;

		}
		else
		{
			m_drawState = Game::WitchState::kStone;
		}
		//A�{�^���������ꂽ��
		if (m_input.Buttons[XINPUT_BUTTON_A] && !m_isLastKeyFlag || CheckHitKey(KEY_INPUT_Z) && !m_isLastKeyFlag)
		{
			if (m_pMain->GetSceneNum() > kUseMagicStage)
			{
				//�A���Ő؂�ւ��Ȃ��悤��
				m_isLastKeyFlag = true;
				//���@�̎�ނ�ύX����
				m_isFire = !m_isFire;
				//���@�����Ԋu�����Z�b�g����
				m_MagicCount = 0;
				//���ʉ���炷
				PlaySoundMem(m_changeMagicSe, DX_PLAYTYPE_BACK);
			}
		}
		else if (!m_input.Buttons[XINPUT_BUTTON_A] && !CheckHitKey(KEY_INPUT_Z))
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
		//�{�胂�[�h�̏���
		if (m_pMain->GetAngryMode())
		{
			m_atk = kAngryModeMagicAtk;
			m_scale = kAngryModeMagicScale;
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
}

void Princess::Draw() const
{
	int animEle = m_animFrame / kAnimFrameNum;

	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * static_cast<int>(m_drawState);
	//���@��ł����ɐ���\������
	if (m_isMagic || m_pMain->GetAngryMode())
	{
		//���@������������������\������
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, -m_angle, m_magicArrowGraph, true, 0, 0);
	}
	//�摜�̂ǂ���؂��邩���w�肵�āA�؂������摜��\������
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,//�؂���ʒu
		kGraphWidth, kGraphHeight,//�؂���傫��
		kCharcterScale,//�g�嗦
		0.0,//��]��
		m_handle, true, false);
	//Hp�������₷�����邽�߂Ɍ���Box�쐬
	DrawBox(static_cast<int>(m_hpBarPos.x) - kBoxSpace, static_cast<int>(m_hpBarPos.y) - kBoxSpace,//�n�_
		static_cast<int>(m_bloodBarPos.x + kMaxBarWidth) + kBoxSpace,//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		static_cast<int>(m_bloodBarPos.y + kBarHeight) + kBoxSpace,//�I�_
		kBoxColor, true);
	//Hp�����₷���悤��Hp�̌��ɍ���Box���o��
	DrawBox(static_cast<int>(m_hpBarPos.x), static_cast<int>(m_hpBarPos.y),//�n�_
		static_cast<int>(m_hpBarPos.x + kMaxBarWidth),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		static_cast<int>(m_hpBarPos.y + kBarHeight),//�I�_
		GetColor(0, 0, 0), true);
	//HP�o�[�̕\��
	DrawBox(static_cast<int>(m_hpBarPos.x), static_cast<int>(m_hpBarPos.y),//�n�_
		static_cast<int>(m_hpBarPos.x) + static_cast<int>(m_hpBarWidth),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		static_cast<int>(m_hpBarPos.y + kBarHeight),//�I�_
		kHpBarColor, true);
	//���̗ʂ����₷���悤�Ɍ��̗ʂ̌��ɔ���Box���o��
	DrawBox(static_cast<int>(m_bloodBarPos.x), static_cast<int>(m_bloodBarPos.y),//�n�_
		static_cast<int>(m_bloodBarPos.x + kMaxBarWidth),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		static_cast<int>(m_bloodBarPos.y + kBarHeight),//�I�_
		GetColor(0, 0, 0), true);
	//���̗ʃo�[�̕\��
	DrawBox(static_cast<int>(m_bloodBarPos.x), static_cast<int>(m_bloodBarPos.y),//�n�_
		static_cast<int>(m_bloodBarPos.x + m_bloodBarWidth),//�n�_�̈ʒu�Ƀo�[�̒����𑫂�
		static_cast<int>(m_bloodBarPos.y + kBarHeight),//�I�_
		kBloodBarColor, true);


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
	knockBack *= kKnockBackScale * enemy.GetSpd();
	//�Փ˓_�̍��W
	m_hitPos = (enemy.GetPos() + m_pos) / 2;
	//�����G�t�F�N�g���o��
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_hitPos, //�p�[�e�B�N���̏�������
			static_cast<float>(kWhiteParticleInfo[0]), static_cast<float>(kWhiteParticleInfo[1]), kWhiteParticleInfo[2], kWhiteParticleInfo[3]);
		m_pMain->AddParticle(m_pParticle);
	}
	m_nowHp -= enemy.GetAtk() - m_def;
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
	}
	enemy.HitPrincess(knockBack);
}

void Princess::ReceiveBlood(float blood)
{
	m_nowBlood += blood;
	if (blood > 0)
	{

		PlaySoundMem(m_passBloodSe, DX_PLAYTYPE_BACK);
	}
}

bool Princess::IsDeath()
{
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
		m_nowState = Game::State::kDelete;
		m_drawState = Game::WitchState::kBreak;
		m_animFrame = kDeathAnimFrame;
		return true;
	}
	return false;
}