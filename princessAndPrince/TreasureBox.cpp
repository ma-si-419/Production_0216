#include "TreasureBox.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "Portion.h"
#include "SceneMain.h"
#include "Player.h"
#include "Particle.h"
namespace
{
	//���Ƃ����̗�
	constexpr int kDropBlood = 3;
	//���Ƃ������̗�
	constexpr int kDropGold = 5;
	//���Ƃ��o���l�̗�
	constexpr int kDropExp = 5;
	//�O���t�B�b�N�̔����̑傫��
	constexpr int kHalfGraphSize = 40;
	//�󔠂̑傫��
	constexpr int kColScale = 45;
	//���@�Ƀq�b�g����C���^�[�o��
	constexpr int kHitMagicInterval = 30;
	//�m�b�N�o�b�N�̑傫��
	constexpr int kKnockBackScale = 3;
	//�p�[�e�B�N���̐�
	constexpr int kParticleVol = 30;
}
TreasureBox::TreasureBox(SceneMain* sceneMain) :
	m_pMain(sceneMain),
	m_hp(GetRand(2) + 3),
	m_colScale(kColScale),
	m_handle(-1),
	m_isExist(true),
	m_knockBackVec()
{
}

TreasureBox::~TreasureBox()
{
}

void TreasureBox::Init(Vec2 pos)
{
	m_nowState = Game::kNormal;
	//�S�������ʒu�ɏo�Ă��܂�Ȃ��悤�ɏ����΂炯������
	m_pos.x = pos.x + GetRand(30) - 15;
	m_pos.y = pos.y + GetRand(30) - 15;

	//��ʊO�ɏo�Ă��܂�Ȃ��悤�ɂ���
	if (m_pos.x > Game::kPlayScreenWidth)
	{
		m_pos.x = Game::kPlayScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y���W�������������s��
	if (m_pos.y > Game::kPlayScreenHeight)
	{
		m_pos.y = Game::kPlayScreenHeight;
	}
	else if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}
}

void TreasureBox::Update()
{
	if (m_nowState != Game::kDelete)
	{
		m_pos -= m_knockBackVec;
		//�m�b�N�o�b�N����
		if (m_knockBackVec.x != 0 || m_knockBackVec.y != 0)
		{
			//�m�b�N�o�b�N���鎞�ԃJ�E���g
			m_knockBackTime++;
			//�K��̎��Ԃ��߂�����
			if (m_knockBackTime > 5)
			{
				//�m�b�N�o�b�N�̑傫����0�ɂ���
				m_knockBackVec *= 0;
				//���ԃJ�E���g��0�ɂ���
				m_knockBackTime = 0;
			}
		}
		m_circleCol.SetCenter(m_pos, m_colScale);
		if (m_hp < 0)
		{
			//�ǂ̏������s���邩�ݒ肷��
			int randomNumber = GetRand(3);

			if (randomNumber == 0)
			{
				for (int i = 0; i < kDropBlood; i++)
				{
					//���̃������̊m��
					std::shared_ptr<Blood> pBlood
						= std::make_shared<Blood>();
					//���̏���������
					pBlood->Init(m_pos);
					//���𗎂Ƃ�����
					m_pMain->AddItem(pBlood);
				}
			}
			else if (randomNumber == 1)
			{
				for (int i = 0; i < kDropExp; i++)
				{
					//�o���l�̃������̊m��
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//�o���l�̏���������
					pExp->Init(m_pos);
					pExp->SetExp(5);
					//�o���l�𗎂Ƃ�����
					m_pMain->AddItem(pExp);
				}
			}
			else if (randomNumber == 2)
			{
				for (int i = 0; i < kDropGold; i++)
				{
					//�����̃������m��
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//�����̏���������
					pGold->Init(m_pos);
					pGold->SetPrice(5);

					//�����𗎂Ƃ�����
					m_pMain->AddItem(pGold);
				}
			}
			else if (randomNumber == 3)
			{
				//�̗͉񕜃A�C�e���𗎂Ƃ����������
				//�����̃������m��
				std::shared_ptr<Portion> pPortion
					= std::make_shared<Portion>();
				//�����̏���������
				pPortion->Init(m_pos);
				//�����𗎂Ƃ�����
				m_pMain->AddItem(pPortion);
			}
			//��Ԃ�ω�������
			m_nowState = Game::kDelete;

		}
		if (m_nowState == Game::kHitMagic)
		{
			m_magicCount++;
			if (m_magicCount > kHitMagicInterval)
			{
				m_nowState = Game::kNormal;
				m_magicCount = 0;
			}
		}
	}
}

void TreasureBox::Draw()
{
	
	//�F�̂���Box�ŉ�����
	DrawBox((int)m_pos.x - kHalfGraphSize, (int)m_pos.y - kHalfGraphSize,
		m_pos.x + kHalfGraphSize, m_pos.y + kHalfGraphSize,
		GetColor(255, 255, 0), true);
	//�����蔻��̕\��
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}

void TreasureBox::HitPlayer(Player* player)
{
	m_hp--;
	//�m�b�N�o�b�N����������
	m_knockBackVec = player->GetMoveVec() * -1;
	m_knockBackVec.Normalize();
	m_knockBackVec *= kKnockBackScale;
	//�Փ˓_�̍��W
	m_hitPos = (player->GetPos() + m_pos) / 2;
	//�����G�t�F�N�g���o��
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_hitPos, 40.0f, 4.0f, 5, 0);
		m_pMain->AddParticle(m_pParticle);
	}
}

void TreasureBox::HitMagic()
{
	m_nowState = Game::kHitMagic;
	//�����G�t�F�N�g���o��
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_pos, 40.0f, 4.0f, 5, 0);
		m_pMain->AddParticle(m_pParticle);
	}
	m_hp--;
	if (m_hp < 0)
	{
		//�ǂ̏������s���邩�ݒ肷��
		int randomNumber = GetRand(3);

		if (randomNumber == 0)
		{
			for (int i = 0; i < kDropBlood; i++)
			{
				//���̃������̊m��
				std::shared_ptr<Blood> pBlood
					= std::make_shared<Blood>();
				//���̏���������
				pBlood->Init(m_pos);
				//���𗎂Ƃ�����
				m_pMain->AddItem(pBlood);
			}
		}
		else if (randomNumber == 1)
		{
			for (int i = 0; i < kDropExp; i++)
			{
				//�o���l�̃������̊m��
				std::shared_ptr<Exp> pExp
					= std::make_shared<Exp>();
				//�o���l�̏���������
				pExp->Init(m_pos);
				pExp->SetExp(5);
				//�o���l�𗎂Ƃ�����
				m_pMain->AddItem(pExp);
			}
		}
		else if (randomNumber == 2)
		{
			for (int i = 0; i < kDropGold; i++)
			{
				//�����̃������m��
				std::shared_ptr<Gold> pGold
					= std::make_shared<Gold>();
				//�����̏���������
				pGold->Init(m_pos);
				pGold->SetPrice(5);

				//�����𗎂Ƃ�����
				m_pMain->AddItem(pGold);
			}
		}
		else if (randomNumber == 3)
		{
			//�̗͉񕜃A�C�e���𗎂Ƃ����������
			//�����̃������m��
			std::shared_ptr<Portion> pPortion
				= std::make_shared<Portion>();
			//�����̏���������
			pPortion->Init(m_pos);
			//�����𗎂Ƃ�����
			m_pMain->AddItem(pPortion);
		}
		//��Ԃ�ω�������
		m_nowState = Game::kDelete;

	}
}
