#include "TreasureBox.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "Portion.h"
#include "SceneMain.h"
namespace
{
	//���Ƃ����̗�
	constexpr int kDropBlood = 3;
	//���Ƃ������̗�
	constexpr int kDropGold = 5;
	//���Ƃ��o���l�̗�
	constexpr int kDropExp = 5;
	//�O���t�B�b�N�̔����̑傫��
	constexpr int kHalfGraphSize = 20;
	//���@�Ƀq�b�g����C���^�[�o��
	constexpr int kHitMagicInterval = 30;
}
TreasureBox::TreasureBox(SceneMain* sceneMain) :
	m_pMain(sceneMain),
	m_hp(GetRand(2) + 3),
	m_colScale(25),
	m_handle(-1),
	m_isExist(true)
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
	if (m_pos.x > Game::kScreenWidth)
	{
		m_pos.x = Game::kScreenWidth;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
	}
	//y���W�������������s��
	if (m_pos.y > Game::kScreenHeight)
	{
		m_pos.y = Game::kScreenHeight;
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

void TreasureBox::HitPlayer()
{
	m_hp--;
	//�m�b�N�o�b�N����������
}

void TreasureBox::HitMagic()
{
	m_hp--;
}
