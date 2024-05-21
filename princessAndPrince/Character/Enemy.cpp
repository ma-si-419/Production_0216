#include "Game.h"
#include "Enemy.h"
#include "Player.h"
#include "MagicBase.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "TreasureBox.h"
#include "SceneMain.h"
#include "Particle.h"
#include<memory>
namespace
{
	// �L�����N�^�[�̃T�C�Y
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;
	// �L�����N�^�[�̃A�j���[�V����
	constexpr int kUseFrame[] = { 0,1 };
	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimFrameNum = 60;
	// �A�j���[�V�����̂P�T�C�N���̃t���[����
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	//�m�b�N�o�b�N�̑傫��
	constexpr int kKnockBackScale = 3;
	//�G�l�~�[���󔠂𗎂Ƃ��m��(Max100)
	constexpr int kDropProb = 30;
	//�}�W�b�N�Ƀq�b�g����C���^�[�o��
	constexpr int kHitMagicInterval = 30;
	//�v���C���[�Ƀq�b�g����C���^�[�o��
	constexpr int kHitPlayerInterval = 8;
	//���܂������̍ő吔
	constexpr int kMaxDir = 3;
	//�{�X�̃T�C�Y
	constexpr float kBossSize = 2.0f;
	//�ʏ�̓G�̃T�C�Y
	constexpr float kEnemySize = 1.0f;
	//��_�̓����蔻������炷�x�N�g��
	constexpr int kShiftPosX = 20;
	constexpr int kShiftPosY = 0;
	//�G�t�F�N�g���o������(��ŏ���)
	constexpr int kEffectTime = 10;
	//�G�t�F�N�g�̑傫��(������)
	constexpr int kEffectSize = 60;
	//�p�[�e�B�N���̐�
	constexpr int kParticleVol = 30;
	//�p�[�e�B�N���̑傫��
	constexpr int kParticleScale = 5;
	//�p�[�e�B�N���̕\������钷��
	constexpr float kParticleLange = 40.0f;
	//�p�[�e�B�N���̑���
	constexpr float kParticleSpeed = 4.0f;
	//���܂��ꏊ�̎��(0���琔����)
	constexpr int kBornPosKindNum = 3;
	//�m�b�N���鎞��
	constexpr int kKnockBackTime = 5;
	//��{�I�ȑ傫��
	constexpr double kNormalRatio = 4.0;
	//��_�ɓ����������̃m�b�N�o�b�N�̑傫��
	constexpr float kWeakKnockBackRatio = 0.6f;
	//�����o�Ă���X�e�[�W
	constexpr int kBloodReleaseStageNum = 1;
}
Enemy::Enemy(SceneMain* pMain) :
	m_targetPos(Game::kPlayScreenWidth / 2, Game::kPlayScreenHeight / 2),
	m_pMain(pMain),
	m_hitCount(0),
	m_isHitFlag(false),
	m_effectCount(0),
	m_haveGold(0),
	m_haveExp(0),
	m_isBoss(false),
	m_isHitWeakFlag(false),
	m_isLeftFlag(false),
	m_kind(goblin),
	m_pTreasureBox(nullptr),
	m_scale(0.0f),
	m_srcY(0),
	m_knockBackPow(0),
	m_itemHandle(0),
	m_colScale(0),
	m_pParticle()
{
	m_animFrame = 0;
	m_nowState = Game::State::kNormal;
}
Enemy::~Enemy()
{
}
void Enemy::Init(int kinds)
{
	m_animFrame = 0;
	int bornPos = GetRand(kBornPosKindNum);
	m_radius = Game::kRadius;
	////////////////////////////
	/*�G�l�~�[�̃X�e�[�^�X�ݒ�*/
	////////////////////////////
	if (kinds == static_cast<int>(goblin))
	{
		m_hp = 8;
		m_atk = 2.5f;
		m_spd = 0.2f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 0;
		m_haveGold = GetRand(20) + 20;
		m_haveExp = GetRand(5) + 4;
		m_kind = goblin;
		m_isBoss = false;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(boar))
	{
		m_hp = 5;
		m_atk = 1.5f;
		m_spd = 0.7f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 1;
		m_haveGold = GetRand(25) + 15;
		m_haveExp = GetRand(5) + 2;
		m_kind = boar;
		m_isBoss = false;
		m_knockBackPow = 4;

	}
	else if (kinds == static_cast<int>(doragon))
	{
		m_hp = 20;
		m_atk = 20.0f;
		m_spd = 0.09f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 2;
		m_haveGold = GetRand(150) + 100;
		m_haveExp = GetRand(30) + 30;
		m_kind = doragon;
		m_isBoss = false;
		m_knockBackPow = 4;

	}
	else if (kinds == static_cast<int>(blackSkeleton))
	{
		m_hp = 20;
		m_atk = 4.5f;
		m_spd = 0.4f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 3;
		m_kind = blackSkeleton;
		m_haveGold = GetRand(80) + 60;
		m_haveExp = GetRand(40) + 20;
		m_isBoss = false;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(snowman))
	{
		m_hp = 200;
		m_atk = 7.5f;
		m_spd = 0.1f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 4;
		m_kind = snowman;
		m_haveGold = GetRand(100) + 50;
		m_haveExp = GetRand(100) + 100;
		m_isBoss = false;
		m_knockBackPow = 1;
	}
	else if (kinds == static_cast<int>(redGoblin))
	{
		m_hp = 13;
		m_atk = 3.0f;
		m_spd = 0.25f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 5;
		m_haveGold = GetRand(30) + 30;
		m_haveExp = GetRand(15) + 15;
		m_kind = redGoblin;
		m_isBoss = false;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(skeleton))
	{
		m_hp = 11;
		m_atk = 3.5f;
		m_spd = 0.5f;
		m_scale = kEnemySize;
		m_colScale = kEnemySize;
		m_srcY = 6;
		m_kind = skeleton;
		m_haveGold = GetRand(20) + 20;
		m_haveExp = GetRand(10) + 10;
		m_isBoss = false;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossGoblin))
	{
		m_hp = 48;
		m_atk = 6.0f;
		m_spd = 0.2f;
		m_scale = kBossSize;
		m_colScale = kBossSize;
		m_srcY = 0;
		m_kind = bossGoblin;
		m_haveGold = GetRand(150) + 100;
		m_haveExp = GetRand(40) + 30;
		m_isBoss = true;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossBoar))
	{
		m_hp = 30;
		m_atk = 3.0f;
		m_spd = 0.4f;
		m_scale = kBossSize;
		m_colScale = kBossSize;
		m_srcY = 1;
		m_kind = bossBoar;
		m_haveGold = GetRand(300) + 200;
		m_haveExp = GetRand(50) + 50;
		m_isBoss = true;
		m_knockBackPow = 3;

	}
	else if (kinds == static_cast<int>(bossDoragon))
	{
		m_hp = 200;
		m_atk = 30.0f;
		m_spd = 0.09f;
		m_scale = kBossSize;
		m_colScale = kBossSize;
		m_srcY = 2;
		m_kind = bossDoragon;
		m_haveGold = GetRand(500) + 800;
		m_haveExp = GetRand(100) + 100;
		m_isBoss = true;
		m_knockBackPow = 3;
	}
	else if (kinds == static_cast<int>(bossBlackSkeleton))
	{
		m_hp = 60;
		m_atk = 7.0f;
		m_spd = 0.3f;
		m_scale = kBossSize;
		m_colScale = 1.5f;
		m_srcY = 3;
		m_kind = bossBlackSkeleton;
		m_haveGold = GetRand(400) + 400;
		m_haveExp = GetRand(100) + 80;
		m_isBoss = true;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossSnowman))
	{
		m_hp = 600;
		m_atk = 10.0f;
		m_spd = 0.1f;
		m_scale = kBossSize;
		m_colScale = kBossSize;
		m_srcY = 4;
		m_kind = bossSnowman;
		m_haveGold = GetRand(2000) + 2000;
		m_haveExp = GetRand(500) + 500;
		m_isBoss = true;
		m_knockBackPow = 1;
	}
	else if (kinds == static_cast<int>(bossRedGoblin))
	{
		m_hp = 60;
		m_atk = 5.5f;
		m_spd = 0.25f;
		m_scale = kBossSize;
		m_colScale = kBossSize;
		m_srcY = 5;
		m_kind = bossRedGoblin;
		m_haveGold = GetRand(300) + 200;
		m_haveExp = GetRand(80) + 60;
		m_isBoss = true;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossSkeleton))
	{
		m_hp = 50;
		m_atk = 4.5f;
		m_spd = 0.5f;
		m_scale = kBossSize;
		m_colScale = 1.5f;
		m_srcY = 6;
		m_kind = bossSkeleton;
		m_haveGold = GetRand(300) + 200;
		m_haveExp = GetRand(100) + 100;
		m_isBoss = true;
		m_knockBackPow = 2;
	}
	else if (kinds == static_cast<int>(bossDemonKing))
	{
		m_hp = 500;
		m_atk = 12.0f;
		m_spd = 0.3f;
		m_scale = kBossSize;
		m_colScale = kBossSize;
		m_srcY = 7;
		m_kind = bossDemonKing;
		m_haveGold = GetRand(8000) + 8000;
		m_haveExp = GetRand(1000) + 1000;
		m_isBoss = true;
		m_knockBackPow = 2;
	}

	//�o���ʒu��ݒ肷��
	switch (bornPos)
	{
		//����������o�Ă���
	case 0:
		m_pos.x = -kGraphWidth * 2;
		m_pos.y = static_cast<float>(GetRand(Game::kPlayScreenHeight));
		break;
		//���������o�Ă���
	case 1:
		m_pos.x = static_cast<float>(GetRand(Game::kPlayScreenWidth));
		m_pos.y = -kGraphHeight * 2;
		break;
		//�E��������o�Ă���
	case 2:
		m_pos.x = Game::kPlayScreenWidth + kGraphWidth * 2;
		m_pos.y = static_cast<float>(GetRand(Game::kPlayScreenHeight));
		break;
		//����������o�Ă���
	case 3:
		m_pos.x = static_cast<float>(GetRand(Game::kPlayScreenWidth));
		m_pos.y = Game::kPlayScreenHeight + kGraphHeight * 2;
	default:
		break;
	}
}

void Enemy::Update()
{
	//��Ԃ�kDelete����Ȃ��ꍇ�̂ݓ���
	if (m_nowState != Game::State::kDelete)
	{
		//�A�j���[�V�����̍X�V����
		m_animFrame++;
		if (kAnimFrameCycle <= m_animFrame)	m_animFrame = 0;
		//�m�b�N�o�b�N����
		if (m_knockBack.x != 0 || m_knockBack.y != 0)
		{
			//�m�b�N�o�b�N���鎞�ԃJ�E���g
			m_knockBackTime++;
			//�K��̎��Ԃ��߂�����
			if (m_knockBackTime > kKnockBackTime)
			{
				//�m�b�N�o�b�N�̑傫����0�ɂ���
				m_knockBack *= 0;
				//���ԃJ�E���g��0�ɂ���
				m_knockBackTime = 0;
				//�G�����ʂƂ��̃^�C�~���O��
				//�m�b�N�o�b�N������̂ق������h���������̂ł����Ɏ��S����������
				if (m_hp < 0)
				{
					//��Ԃ�ω�������
					m_nowState = Game::State::kDelete;
					if (m_pMain->GetSceneNum() > 1)
					{
						//���̃������̊m��
						std::shared_ptr<Blood> pBlood
							= std::make_shared<Blood>();

						//���𗎂Ƃ�����
						pBlood->Init(m_pos);
						pBlood->SetHandle(m_itemHandle);
						m_pMain->AddItem(pBlood);
					}
					//�o���l�̃������̊m��
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//�o���l�𗎂Ƃ�����
					pExp->Init(m_pos);
					pExp->SetExp(m_haveExp);
					pExp->SetHandle(m_itemHandle);
					//�{�X�̗��Ƃ��A�C�e����������؂�����W��ς���
					if (m_isBoss)pExp->SetBossItem();
					m_pMain->AddItem(pExp);
					//�����̃������m��
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//�����𗎂Ƃ�����
					pGold->Init(m_pos);
					pGold->SetPrice(m_haveGold);
					pGold->SetHandle(m_itemHandle);
					if (m_isBoss)pGold->SetBossItem();
					m_pMain->AddItem(pGold);
					//�m���𒴂�����
					if (GetRand(100) < kDropProb && !m_isBoss)
					{
						m_pTreasureBox = new TreasureBox(m_pMain);
						m_pTreasureBox->Init(m_pos);
						m_pTreasureBox->SetHandle(m_itemHandle);
						m_pTreasureBox->SerGoldAndExp(m_haveGold, m_haveExp);
						//���C���ɕ󔠂𐶐�����֐����쐬����
						m_pMain->AddTreasure(m_pTreasureBox);
					}
					//�{�X��������{�X��|�����ƃV�[�����C���ɓ`����
					if (m_isBoss)
					{
						m_pMain->CountKillBoss();
					}
				}
			}
		}
		//���@�ɘA���œ�����Ȃ��悤�ɏ�Ԃ�ω�������
		if (m_nowState == Game::State::kHitMagic)
		{
			m_hitCount++;
			if (m_hitCount > kHitMagicInterval)
			{
				m_nowState = Game::State::kNormal;
				m_hitCount = 0;
			}
		}
		//�v���C���[�ɘA���œ�����Ȃ��悤�ɏ�Ԃ�ω�������
		if (m_nowState == Game::State::kHitPlayer)
		{
			m_hitCount++;
			if (m_hitCount > kHitPlayerInterval)
			{
				m_nowState = Game::State::kNormal;
				m_hitCount = 0;
			}
		}
		if (m_nowState != Game::State::kStop)
		{
			//�ړ��ʂ̌v�Z
			m_moveVec = m_targetPos - m_pos;
			m_moveVec.Normalize();
			m_moveVec *= m_spd;
			//�ړ�����
			m_pos += m_moveVec;
			m_pos -= m_knockBack;
			//�����蔻��̍X�V
			m_circleCol.SetCenter(m_pos, m_radius * m_colScale);
			//��_��Y���W�̍X�V
			m_weakPos.y = m_pos.y + kShiftPosY;
			//��_�����蔻��̍X�V
			m_weakCircle.SetCenter(m_weakPos, m_radius * m_colScale);
		}
		if (m_moveVec.x > 0)
		{
			m_isLeftFlag = false;
			//��_��X���W�̍X�V
			m_weakPos.x = m_pos.x - kShiftPosX;
		}
		else
		{
			m_isLeftFlag = true;
			//��_��X���W�̍X�V
			m_weakPos.x = m_pos.x + kShiftPosX;

		}
	}
}
void Enemy::Draw()
{
	//��Ԃ�kDelete����Ȃ��ꍇ�̂ݓ�����
	if (m_nowState != Game::State::kDelete)
	{

		int animEle = m_animFrame / kAnimFrameNum;

		int srcX = kGraphWidth * kUseFrame[animEle];
		int srcY = kGraphHeight * m_srcY;

		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			kNormalRatio * m_scale,
			0.0,
			m_handle, true, m_isLeftFlag);
#ifdef _DEBUG
		m_circleCol.Draw(static_cast<int>(m_radius * m_colScale), 0x0000ff, false);
		m_weakCircle.Draw(static_cast<int>(m_radius * m_colScale), 0xff0000, false);
#endif
	}
}
void Enemy::HitPlayer(Player& player, bool weak)
{
	//�v���C���[�̍��W���Q�l�ɂ��āA�m�b�N�o�b�N�̕��������߂�
	m_knockBack = player.GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= m_knockBackPow * kKnockBackScale;
	m_isHitFlag = true;
	//���_���o��(�Փ˓_�̍��W)
	m_hitPos = (player.GetPos() + m_pos) / 2;
	//��_�ɓ������Ă�����
	if (weak)
	{
		m_hp -= (player.GetAtk() - m_def) * 2;
		//�Ԃ��G�t�F�N�g���o��
		for (int i = 0; i < kParticleVol; i++)
		{
			m_pParticle = new Particle(m_hitPos, kParticleLange, kParticleSpeed, kParticleScale, static_cast<int>(Game::ParticleKind::kRed));
			m_pMain->AddParticle(m_pParticle);
		}
		m_knockBack *= kWeakKnockBackRatio;
	}
	//�������Ă��Ȃ�������
	else
	{
		m_hp -= player.GetAtk() - m_def;
		//�����G�t�F�N�g���o��
		for (int i = 0; i < kParticleVol; i++)
		{
			m_pParticle = new Particle(m_hitPos, kParticleLange, kParticleSpeed, kParticleScale, static_cast<int>(Game::ParticleKind::kWhite));
			m_pMain->AddParticle(m_pParticle);
		}
	}
}
void Enemy::HitMagic(MagicBase* magic)
{
	//�̗͂����炷
	m_hp -= magic->GetAtk();
	//�Ԃ������Ƃ��̃G�t�F�N�g���o��
	m_isHitFlag = true;
	m_isHitWeakFlag = false;
	//�����G�t�F�N�g���o��
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_pos, kParticleLange, kParticleSpeed, kParticleScale, static_cast<int>(Game::ParticleKind::kWhite));
		m_pMain->AddParticle(m_pParticle);
	}
	//�������@�������@��������
	if (magic->GetMagicKind())
	{
		//�Ԃ��������@������
		magic->m_nowState = Game::State::kDelete;
	}
	//���@�Ŏ��񂾏ꍇ�̏����������ɓ���Ă���
	if (m_hp < 0)
	{
		//�X�e�[�W�Q�܂ł͌��𗎂Ƃ��Ȃ��悤�ɂ���
		if (m_pMain->GetSceneNum() > kBloodReleaseStageNum)
		{
			//���̃������̊m��
			std::shared_ptr<Blood> pBlood
				= std::make_shared<Blood>();
			//���𗎂Ƃ�����
			pBlood->Init(m_pos);
			pBlood->SetHandle(m_itemHandle);
			m_pMain->AddItem(pBlood);
		}
		//�o���l�̃������̊m��
		std::shared_ptr<Exp> pExp
			= std::make_shared<Exp>();
		//�o���l�𗎂Ƃ�����
		pExp->SetExp(m_haveExp);
		pExp->Init(m_pos);
		pExp->SetHandle(m_itemHandle);
		if (m_isBoss)pExp->SetBossItem();
		m_pMain->AddItem(pExp);
		//�����̃������m��
		std::shared_ptr<Gold> pGold
			= std::make_shared<Gold>();
		//�����𗎂Ƃ�����
		pGold->SetPrice(m_haveGold);
		pGold->Init(m_pos);
		pGold->SetHandle(m_itemHandle);
		if (m_isBoss)pGold->SetBossItem();
		m_pMain->AddItem(pGold);
		if (GetRand(100) < kDropProb && !m_isBoss)
		{
			m_pTreasureBox = new TreasureBox(m_pMain);
			m_pTreasureBox->Init(m_pos);
			m_pTreasureBox->SetHandle(m_itemHandle);
			m_pTreasureBox->SerGoldAndExp(m_haveGold, m_haveExp);
			//���C���ɕ󔠂𐶐�����֐����쐬����
			m_pMain->AddTreasure(m_pTreasureBox);
		}
		//�{�X��|������{�X��|�����ƃV�[�����C���ɋ�����
		if (m_isBoss)
		{
			m_pMain->CountKillBoss();
		}
		m_nowState = Game::State::kDelete;
	}
}