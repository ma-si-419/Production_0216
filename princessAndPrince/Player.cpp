#include <DxLib.h>
#include "Player.h"
#include "Game.h"
#include "Pad.h"
#include "SceneMain.h"
#include "CircleCol.h"
#include "TreasureBox.h"
#include "Enemy.h"
#include "Princess.h"
#include "ItemBase.h"
#include "UserData.h"

namespace
{
	// �ړ����x
	constexpr float kSpeed = 3.0f;
	//�摜�̊g�嗦
	constexpr double kGraphScale = 3.0;
	// �L�����N�^�[�̃T�C�Y
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;
	// �L�����N�^�[�̔����̃T�C�Y
	constexpr float kGraphHalfWidth = kGraphWidth / 2.0f;
	constexpr float kGraphHalfHeight = kGraphHeight / 2.0f;
	// �L�����N�^�[�̃A�j���[�V����
	constexpr int kUseFrame[] = { 1, 2, 3, 2 };
	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimFrameNum = 12;
	// �A�j���[�V�����̂P�T�C�N���̃t���[����
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//�m�b�N�o�b�N�̑傫��
	constexpr int kKnockBackScale = 3;
	//Hp�o�[�̉��̒���
	constexpr float kMaxBarWidth = 60;
	//Hp�o�[�̏c�̒���
	constexpr float kBarHeight = 5;
	//Hp�o�[�̃|�W�V����
	constexpr int kHpBarPosY = 50;
	//����������̍d������
	constexpr int kRevivalTime = 30;
	//���Ă錌�̗ʂ̍ő�l
	constexpr float kMaxBlood = 10;
	//�o�[�̌��ɕ\������Box�̗]���̑傫��
	constexpr int kBoxSpace = 3;
	//�L�����̊g�嗦
	constexpr float kCharcterScale = 6.0f;
	//�N���A���̃_���X�̎��Ԃ̒���
	constexpr int kDanceTIme = 5;
	//�N���A���̉���
	constexpr int kTurnCount = 16;
}

Player::Player(SceneMain* pMain) :
	m_isMove(false),
	m_hpBarWidth(0),
	m_bloodBarWidth(0),
	m_lastPad(0),
	m_maxBlood(kMaxBlood),
	m_nowBlood(0),
	m_nowHp(m_hp),
	m_gold(0),
	m_exp(0),
	m_moveVec(),
	m_isDeathFlag(false),
	m_pMain(pMain),
	m_turnCount(0),
	m_danceCount(0)
{
	//�������W�𖂏��ׂ̗ɐݒ�
	m_pos.x = Game::kPlayScreenWidth / 2 + 100;
	m_pos.y = Game::kPlayScreenHeight / 2;
	//�ŏ��̌������������ɐݒ�
	m_dir = Game::kDirDown;
	//�A�j���[�V�����̍ŏ��̉摜��ݒ�
	m_animFrame = kAnimFrameNum;
	//�~�̔��a��ݒ�
	m_radius = Game::kRadius;
	//��Ԃ�������
	m_nowState = Game::kNormal;

}

Player::~Player()
{

}

void Player::Init()
{
	m_atk = 2.0f + (UserData::userAtkLevel * 0.5f) + (UserData::userMainLevel * 0.1f);
	m_hp = 30 + (UserData::userMainLevel * 2.0f);
	m_spd = 1.5f + (UserData::userSpdLevel * 0.1f) + (UserData::userMainLevel * 0.02f);
	m_def = 1.0f + (UserData::userDefLevel * 0.3f) + (UserData::userMainLevel * 0.05f);
	m_nowHp = m_hp;
	//���W���Q�l��Hp�o�[�̈ʒu��ݒ�
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	//Hp�̊������v�Z���ăo�[�̒������Z�o����
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//���̗ʃo�[�̏���
	//���Hp�o�[���������Ⴂ���W�ɕ\��������
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//�R�C���̌��ʉ���ݒ肷��
	m_coinSe = m_pMain->GetCoinSe();
	m_expSe = m_pMain->GetExpSe();
	m_portionSe = m_pMain->GetPortionSe();
	m_bloodSe = m_pMain->GetBloodSe();
}

void Player::Update()
{
	// �p�b�h�̏\���L�[���g�p���ăv���C���[���ړ�������
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//�|��Ă���Ƃ��͏��X�ɉ񕜂���悤�ɂ���
	if (m_nowState == Game::kDelete)
	{
		//�������񕜂���悤�ɂ���
		m_nowHp += GetHealRate();
		//���o�K�`������Ƒ����񕜂���悤�ɂ���
		//�������ςł͂��߂Ȃ̂ŁAif����ǉ�����
		//�|��Ă����ԂłȂ��ƃ��o�K�`���ł��Ȃ��悤�ɂ���(���S���o�����邽�߂�)
		if (m_isDeathFlag)
		{

			if (pad & PAD_INPUT_UP || CheckHitKey(KEY_INPUT_W))
			{
				if (m_lastPad != 0)
				{
					//�����͂���key��ۑ�����
					m_lastPad = 0;
					//�񕜂���
					m_nowHp += GetHealRate() * 3;
					//�����_���Ƀv���C���[�𓮂���
					m_pos.x = m_deathPos.x + GetRand(2) - 1;
					m_pos.y = m_deathPos.y + GetRand(2) - 1;
				}

			}
			else if (pad & PAD_INPUT_DOWN || CheckHitKey(KEY_INPUT_S))
			{
				if (m_lastPad != 1)
				{
					//�����͂���key��ۑ�����
					m_lastPad = 1;
					//�񕜂���
					m_nowHp += GetHealRate() * 3;
					//�����_���Ƀv���C���[�𓮂���
					m_pos.x = GetRand(10) + m_deathPos.x;
					m_pos.y = GetRand(10) + m_deathPos.y;
				}
			}
			else if (pad & PAD_INPUT_LEFT || CheckHitKey(KEY_INPUT_A))
			{
				if (m_lastPad != 2)
				{
					//�����͂���key��ۑ�����
					m_lastPad = 2;
					//�񕜂���
					m_nowHp += GetHealRate() * 3;
					//�����_���Ƀv���C���[�𓮂���
					m_pos.x = GetRand(10) + m_deathPos.x;
					m_pos.y = GetRand(10) + m_deathPos.y;
				}
			}
			else if (pad & PAD_INPUT_RIGHT || CheckHitKey(KEY_INPUT_D))
			{
				if (m_lastPad != 3)
				{
					//�����͂���key��ۑ�����
					m_lastPad = 3;
					//�񕜂���
					m_nowHp += GetHealRate() * 3;
					//�����_���Ƀv���C���[�𓮂���
					m_pos.x = GetRand(10) + m_deathPos.x;
					m_pos.y = GetRand(10) + m_deathPos.y;
				}
			}
		}
		if (m_nowHp >= m_hp)
		{
			/////////////////////////////////////////////////////////////////////////
			//���݂̗̑͂��ő�l�����傫���Ȃ�Ȃ��悤��
			m_nowHp = m_hp;
			//�A�j���[�V�����𓮂������ǂ���
			bool isAnimEnd = false;
			if (m_animFrame / kAnimFrameNum == 2)
			{
				if (m_pMain->GetSpecialMode())
				{
					m_revivalCount += kRevivalTime;
				}
				m_revivalCount++;
				isAnimEnd = true;
			}
			// �����A�j���[�V�����𓮂���
			if (!isAnimEnd)
			{
				m_animFrame++;
				if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 1;
			}

			//�����̍d�����Ԃ𒴂�����
			if (m_revivalCount > kRevivalTime)
			{
				//��Ԃ�kNormal�ɕω�������
				m_nowState = Game::kNormal;
				//�����Ă���������������ɂ���
				m_dir = Game::kDirDown;
				//���񂾃t���b�O��false�ɂ���
				m_isDeathFlag = false;
				m_revivalCount = 0;
			}
			/////////////////////////////////////////////////////////////////////////
		}
	}
	//�̗͂�0�̎��ȊO�����悤�ɂ���
	if (m_nowHp > 0 && m_nowState != Game::kDelete)
	{


		m_isMove = false;

		m_moveVec.x = 0.0f;
		m_moveVec.y = 0.0f;
		if (m_nowState != Game::kStop)
		{

			//���[�U��Key���͂��擾
			if (pad & PAD_INPUT_UP || CheckHitKey(KEY_INPUT_W))
			{
				m_moveVec.y--;
				m_dirY = -1;
				m_isMove = true;
			}
			else if (pad & PAD_INPUT_DOWN || CheckHitKey(KEY_INPUT_S))
			{
				m_moveVec.y++;
				m_dirY = 1;
				m_isMove = true;
			}
			else
			{
				m_dirY = 0;
			}
			if (pad & PAD_INPUT_LEFT || CheckHitKey(KEY_INPUT_A))
			{
				m_moveVec.x--;
				m_dirX = -1;
				m_isMove = true;
			}
			else if (pad & PAD_INPUT_RIGHT || CheckHitKey(KEY_INPUT_D))
			{
				m_moveVec.x++;
				m_dirX = 1;
				m_isMove = true;
			}
			else
			{
				m_dirX = 0;
			}
		}

		//�擾����Key���͂���APlayer�L�����̕������쐬���Ă����Ԃ��B
		GetDir(m_dirX, m_dirY);

		// ���K��
		m_moveVec.Normalize();
		// �����̕ύX
		m_moveVec *= m_spd;
		//�ړ��ʂɃm�b�N�o�b�N�𑫂�
		m_moveVec -= m_knockBack;
		// ���W�Ƀx�N�g���𑫂�
		if (m_nowState != Game::kHitEnemy)
		{
			m_pos += m_moveVec;
		}
		else if (m_nowState == Game::kHitEnemy)
		{
			m_pos -= m_knockBack;
		}
		//�m�b�N�o�b�N����
		if (m_knockBack.x != 0 || m_knockBack.y != 0)
		{
			m_knockBackTime++;
			//��莞�ԃm�b�N�o�b�N������
			if (m_knockBackTime > 5)
			{
				m_nowState = Game::kNormal;
				m_knockBack *= 0;
				m_knockBackTime = 0;
			}
		}
		// �����蔻��̍X�V
		m_circleCol.SetCenter(m_pos, m_radius);

		// �΂߈ړ��̏ꍇ�����������ňړ�����悤�ɂ���
		if (m_isMove)
		{
			// �����A�j���[�V����
			m_animFrame++;
			if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 1;
		}
		else
		{
			m_animFrame = kAnimFrameNum;
		}
	}
	else
	{
		DeathMove();
	}
	// �c���̈ړ�����
	if (m_pos.y < kGraphHalfHeight * kCharcterScale)
		m_pos.y = kGraphHalfHeight * kCharcterScale;
	else if (Game::kPlayScreenHeight - (kGraphHalfHeight * kCharcterScale) < m_pos.y)
		m_pos.y = Game::kPlayScreenHeight - (kGraphHalfHeight * kCharcterScale);
	// �����̈ړ�����
	if (m_pos.x < kGraphHalfWidth * kCharcterScale)
		m_pos.x = kGraphHalfWidth * kCharcterScale;
	else if (Game::kPlayScreenWidth - (kGraphHalfWidth * kCharcterScale) < m_pos.x)
		m_pos.x = Game::kPlayScreenWidth - (kGraphHalfWidth * kCharcterScale);
	//HP�o�[�̏���//
	//���W���Q�l��Hp�o�[�̈ʒu��ݒ�
	m_hpBarPos.x = m_pos.x - kMaxBarWidth / 2;
	m_hpBarPos.y = m_pos.y - kHpBarPosY;
	//Hp�̊������v�Z���ăo�[�̒������Z�o����
	m_hpBarWidth = kMaxBarWidth * GetHpRate();
	//���̗ʃo�[�̏���
	//���Hp�o�[���������Ⴂ���W�ɕ\��������
	m_bloodBarPos.x = m_hpBarPos.x;
	m_bloodBarPos.y = m_hpBarPos.y + kBarHeight * 2;
	//���̗ʂ̊������v�Z���ăo�[�̒������Z�o����
	m_bloodBarWidth = kMaxBarWidth * GetBloodRate();
	if (m_nowBlood <= 0)
	{
		m_nowBlood = 0;
	}
	//�������[�h��Ԃ̃X�e�[�^�X�ω�
	if (m_pMain->GetSpecialMode())
	{
		m_nowHp = m_hp;
		m_atk = (2.0f + (UserData::userAtkLevel * 0.5f)) * 1.5f;
		m_spd = 4.0f;
	}
	else
	{
		m_atk = 2.0f + (UserData::userAtkLevel * 0.5f);
		m_spd = 1.5f + (UserData::userSpdLevel * 0.1f);
	}
}

void Player::Draw() const
{
	int animEle = m_animFrame / kAnimFrameNum;

	//�摜�̂ǂ���؂��邩�v�Z
	int srcX = kGraphWidth * kUseFrame[animEle];
	int srcY = kGraphHeight * m_dir;
	//�`�揈��
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		kCharcterScale,
		0.0,
		m_handle, true, false);
	////////////////////
	/* HP�o�[�\�����o */
	////////////////////
	{
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
	}

#ifdef _DEBUG
	m_circleCol.Draw(m_radius, 0x0000ff, false);
#endif
}

void Player::HitEnemy(Enemy enemy, bool weak)
{
	m_nowState = Game::kHitEnemy;
	m_knockBack = enemy.GetPos() - m_pos;
	m_knockBack.Normalize();
	//�G�l�~�[�̈ړ����x�ɉ����ăm�b�N�o�b�N����
	//�������[�h�ɓ����Ă���Ƃ��̓_���[�W�������s��Ȃ�
		//��_�ɓ������Ă�����
	float damage = 0;
	if (weak)
	{
		//�󂯂�_���[�W�𔼕��ɂ���
		damage = enemy.GetAtk() - m_def / 2;
		//�Œ�ł�0.5�_���[�W�͐H�炤�悤�ɂ���
		if (damage < 1)
		{
			damage = 0.5f;
		}
		//�G�l�~�[�̈ړ����x�ɉ����ăm�b�N�o�b�N����
		m_knockBack *= enemy.GetKnockBackPow() * kKnockBackScale;
		//��_�ɓ������Ă�����m�b�N�o�b�N�̑傫�������炷
		m_knockBack *= 0.6f;

	}
	else//��_�ɓ������Ă��Ȃ������炻�̂܂�
	{
		damage = enemy.GetAtk() - m_def;
		//�Œ�ł�0.5�_���[�W�͐H�炤�悤�ɂ���
		if (damage < 1)
		{
			damage = 0.5f;
		}
		//�G�l�~�[�̈ړ����x�ɉ����ăm�b�N�o�b�N����
		m_knockBack *= enemy.GetKnockBackPow() * kKnockBackScale;
	}
	if (!m_pMain->GetSpecialMode())
	{
		m_nowHp -= damage;
	}

	//�̗͂��}�C�i�X�ɂȂ�Ȃ��悤��
	if (m_nowHp <= 0)
	{
		m_nowHp = 0;
		//��Ԃ�kDelete�ɕω�������
		m_nowState = Game::kDelete;
	}
}
void Player::HitTreasure(TreasureBox* treasureBox)
{
	m_knockBack = m_moveVec;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale * treasureBox->GetKnockBackPow() * (GetRand(2) + 1);
	m_nowState = Game::kHitEnemy;
}
void Player::PickUpItem(std::shared_ptr<ItemBase> item)
{
	switch (item->GetKind())
	{
	case Game::kEmpty:
		printfDx("�o�O");
		break;
	case Game::kExp:
		PlaySoundMem(m_expSe, DX_PLAYTYPE_BACK);
		//�����Ă���o���l�ʂ𑝂₷
		item->m_nowState = Game::kNone;
		item->MoveItem(this);
		break;
	case Game::kGold:
		PlaySoundMem(m_coinSe, DX_PLAYTYPE_BACK);
		//�����Ă��邨���𑝂₷
		item->m_nowState = Game::kNone;
		item->MoveItem(this);
		break;
	case Game::kBlood:
		//�v���C���[�̎����̗ʂ𑝂₷
		//�����Ă���̌��̗ʂ�����l�����傫���Ȃ�������
		if (m_nowBlood < m_maxBlood)
		{
			m_nowBlood++;
		}
		PlaySoundMem(m_bloodSe, DX_PLAYTYPE_BACK);
		item->m_nowState = Game::kDelete;
		break;
	case Game::kPortion:
		PlaySoundMem(m_portionSe, DX_PLAYTYPE_BACK);
		//�v���C���[�̗̑͂�S�񕜂�����
		m_nowHp = m_hp;
		item->m_nowState = Game::kDelete;
	}
}

void Player::GiveBlood(Princess* princess)
{
	float giveBlood = 0;
	giveBlood = m_nowBlood;
	m_nowBlood -= (kMaxBlood - princess->GetBlood());
	princess->ReceiveBlood(giveBlood);
	if (m_nowBlood < 0)
	{
		m_nowBlood = 0;
	}

}

void Player::DeathMove()
{
	//�|�ꂽ���ɓ����x�N�g��
	Vec2 deathMoveVec;
	//�m�b�N�o�b�N�̈ړ����������s��
	deathMoveVec = m_knockBack * 0.1f;
	// ���W�Ƀx�N�g���𑫂�
	m_pos -= deathMoveVec;
	//�m�b�N�o�b�N����
	if (m_knockBack.x != 0 || m_knockBack.y != 0)
	{
		if (m_pMain->GetSpecialMode())
		{
			m_knockBackTime += 100;
		}
		m_knockBackTime++;
		//���S���o
		if (m_knockBackTime < 20)
		{
			m_dir = Game::kDirDown;
			m_animFrame = 0;
		}
		else if (m_knockBackTime < 40)
		{
			m_dir = Game::kDirLeft;
		}
		else if (m_knockBackTime < 60)
		{
			m_dir = Game::kDirUp;
		}
		else if (m_knockBackTime < 80)
		{
			m_dir = Game::kDirRight;
		}
		else
		{
			if (!m_pMain->GetSpecialMode())
			{
				m_dir = Game::kDirDeath;
			}
		}
		//��莞�ԃm�b�N�o�b�N������
		if (m_knockBackTime > 100)
		{
			m_knockBack *= 0;
			m_knockBackTime = 0;
			//�|�ꂽ�t���b�O�𗧂Ă�
			m_isDeathFlag = true;
			//�|�ꂽ���W��ۑ�����
			m_deathPos = m_pos;
		}
	}

}

void Player::ClearDance()
{
	m_animFrame = kAnimFrameNum;
	m_danceCount++;
	if (m_danceCount > kDanceTIme)
	{
		m_danceCount = 0;
		switch (m_dir)
		{
		case Game::kDirDown:
			m_dir = Game::kDirLeft;
			break;
		case Game::kDirLeftDown:
			m_dir = Game::kDirLeft;
			break;
		case Game::kDirLeft:
			m_dir = Game::kDirUp;
			break;
		case Game::kDirLeftUp:
			m_dir = Game::kDirUp;
			break;
		case Game::kDirUp:
			m_dir = Game::kDirRight;
			break;
		case Game::kDirRightUp:
			m_dir = Game::kDirRight;
			break;
		case Game::kDirRight:
			m_dir = Game::kDirDown;
			break;
		case Game::kDirRightDown:
			m_dir = Game::kDirDown;
			break;
		case Game::kDirDeath:
			break;
		default:
			break;
		}
		m_turnCount++;
	}
	if (m_turnCount > kTurnCount)
	{
		m_dir = Game::kDirDeath;
		m_animFrame = kAnimFrameNum * 2;
		m_pMain->ShowResult();
	}
}


