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
	constexpr int kKnockBackScale = 4;
	//�G�l�~�[���󔠂𗎂Ƃ��m��(Max100)
	constexpr int kDropProb = 20;
}
Enemy::Enemy(SceneMain* pMain) :
	m_targetPos(Game::kScreenWidth / 2, Game::kScreenHeight / 2),
	m_pMain(pMain)
{
	m_animFrame = 0;
	m_nowState = Game::kNormal;
}
Enemy::~Enemy()
{
}
void Enemy::Init(int kinds)
{
	m_animFrame = 0;
	m_pos.x = GetRand(1000) - 200;
	m_pos.y = GetRand(1000) - 200;
	m_radius = Game::kRadius;
	if (kinds == static_cast<int>(goblin))
	{
		m_hp = 2;
		m_atk = 1;
		m_spd = 0.1f;
		m_kind = goblin;
	}
	if (kinds == static_cast<int>(boar))
	{
		m_hp = 3;
		m_atk = 1;
		m_spd = 0.2f;
		m_kind = boar;
	}
	if (kinds == static_cast<int>(doragon))
	{
		m_hp = 4;
		m_atk = 1;
		m_spd = 0.3f;
		m_kind = doragon;
	}
	if (kinds == static_cast<int>(skeleton))
	{
		m_hp = 5;
		m_atk = 1;
		m_spd = 0.4f;
		m_kind = skeleton;
	}
	if (kinds == static_cast<int>(snowman))
	{
		m_hp = 6;
		m_atk = 1;
		m_spd = 0.5f;
		m_kind = snowman;
	}
}

void Enemy::Update()
{
	//��Ԃ�kDelete����Ȃ��ꍇ�̂ݓ���
	if (m_nowState != Game::kDelete)
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
			if (m_knockBackTime > 5)
			{
				//�m�b�N�o�b�N�̑傫����0�ɂ���
				m_knockBack *= 0;
				//���ԃJ�E���g��0�ɂ���
				m_knockBackTime = 0;
				//�G�����ʂƂ��̃^�C�~���O��
				//�m�b�N�o�b�N������̂ق������h���������̂ł����Ɏ��S����������
				if (m_hp < 0)
				{
					//���̃������̊m��
					std::shared_ptr<Blood> pBlood
						= std::make_shared<Blood>();
					//���𗎂Ƃ�����
					pBlood->Init(m_pos);
					m_pMain->AddItem(pBlood);
					//�o���l�̃������̊m��
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//�o���l�𗎂Ƃ�����
					pExp->Init(m_pos);
					m_pMain->AddItem(pExp);
					//�����̃������m��
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//�����𗎂Ƃ�����
					pGold->Init(m_pos);
					m_pMain->AddItem(pGold);
					if (GetRand(100) < kDropProb)
					{
						//�󔠂̃������m��
						std::shared_ptr<TreasureBox> pTreasure
							= std::make_shared<TreasureBox>(m_pMain);
						//���C���ɕ󔠂𐶐�����֐����쐬����
					}
					//��Ԃ�ω�������
					m_nowState = Game::kDelete;
				}
			}
		}
		m_moveVec = m_targetPos - m_pos;
		m_moveVec.Normalize();
		m_moveVec *= m_spd;
		m_pos += m_moveVec;
		m_pos -= m_knockBack;
		m_circleCol.SetCenter(m_pos, m_radius);
		if (m_moveVec.x > 0)
		{
			m_isLeftFlag = false;
		}
		else
		{
			m_isLeftFlag = true;
		}
	}
}
void Enemy::Draw()
{
	//��Ԃ�kDelete����Ȃ��ꍇ�̂ݓ�����
	if (m_nowState != Game::kDelete)
	{

		int animEle = m_animFrame / kAnimFrameNum;

		int srcX = kGraphWidth * kUseFrame[animEle];
		int srcY = kGraphHeight * (int)m_kind;

		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			srcX, srcY,
			kGraphWidth, kGraphHeight,
			2.0,
			0.0,
			m_handle, true, m_isLeftFlag);
#ifdef _DEBUG
		m_circleCol.Draw(m_radius, 0x0000ff, false);
#endif
	}
}

void Enemy::HitPlayer(Player& player)
{
	m_knockBack = player.GetPos() - m_pos;
	m_knockBack.Normalize();
	m_knockBack *= kKnockBackScale;
	m_hp -= player.GetAtk() - m_def;
}

void Enemy::HitMagic(MagicBase* magic)
{
	m_hp -= magic->GetAtk();
	//�������@�������@��������
	if (magic->GetMagicKind())
	{
		//�Ԃ��������@������
		magic->m_nowState = Game::kDelete;
	}
	if (m_hp < 0)
	{//���̃������̊m��
		std::shared_ptr<Blood> pBlood
			= std::make_shared<Blood>();
		//���𗎂Ƃ�����
		pBlood->Init(m_pos);
		m_pMain->AddItem(pBlood);
		//�o���l�̃������̊m��
		std::shared_ptr<Exp> pExp
			= std::make_shared<Exp>();
		//�o���l�𗎂Ƃ�����
		pExp->Init(m_pos);
		m_pMain->AddItem(pExp);
		//�����̃������m��
		std::shared_ptr<Gold> pGold
			= std::make_shared<Gold>();
		//�����𗎂Ƃ�����
		pGold->Init(m_pos);
		m_pMain->AddItem(pGold);
		if (GetRand(100) < kDropProb)
		{
			//�󔠂̃������m��
			std::shared_ptr<TreasureBox> pTreasure
				= std::make_shared<TreasureBox>(m_pMain);
			//���C���ɕ󔠂𐶐�����֐����쐬����
		}
		m_nowState = Game::kDelete;
	}
}
