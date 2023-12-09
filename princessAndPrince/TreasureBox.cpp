#include "TreasureBox.h"
#include "DxLib.h"
#include "Blood.h"
#include "Exp.h"
#include "Gold.h"
#include "SceneMain.h"

TreasureBox::TreasureBox(SceneMain* sceneMain) :
	m_pMain(sceneMain),
	m_hp(GetRand(2) + 3),
	m_colScale(30),
	m_handle(-1),
	m_isExist(true)
{
}

TreasureBox::~TreasureBox()
{
}

void TreasureBox::Init(Vec2 pos)
{
	m_isExist = true;
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
	if (m_hp < 0)
	{
		//�ǂ̏������s���邩�ݒ肷��
		int randomNumber = GetRand(3);

		if (randomNumber == 0)
		{
			//���̃������̊m��
			std::shared_ptr<Blood> pBlood
				= std::make_shared<Blood>();
			//���̏���������
			pBlood->Init(m_pos);
			//���𗎂Ƃ�����
			m_pMain->AddItem(pBlood);
		}
		else if (randomNumber == 1)
		{
			//�o���l�̃������̊m��
			std::shared_ptr<Exp> pExp
				= std::make_shared<Exp>();
			//�o���l�̏���������
			pExp->Init(m_pos);
			//�o���l�𗎂Ƃ�����
			m_pMain->AddItem(pExp);
		}
		else if (randomNumber == 2)
		{
			//�����̃������m��
			std::shared_ptr<Gold> pGold
				= std::make_shared<Gold>();
			//�����̏���������
			pGold->Init(m_pos);
			//�����𗎂Ƃ�����
			m_pMain->AddItem(pGold);
		}
		else if (randomNumber == 3)
		{
			//�̗͉񕜃A�C�e���𗎂Ƃ����������
		}



		//���݂��Ă���t���O��false�ɂ���
		m_isExist = false;
	}
}

void TreasureBox::Draw()
{
	//�F�̂���Box�ŉ�����
	DrawBox((int)m_pos.x, (int)m_pos.y,
		m_pos.x + m_colScale, m_pos.y + m_colScale,
		GetColor(255, 255, 0), true);
}

void TreasureBox::OpenTreasure()
{

}
