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
	constexpr int kColScale = 30;
	//���@�Ƀq�b�g����C���^�[�o��
	constexpr int kHitMagicInterval = 30;
	//�m�b�N�o�b�N�̑傫��
	constexpr int kKnockBackScale = 3;
	//�p�[�e�B�N���̐�
	constexpr int kParticleVol = 30;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPow = 3;
	//�m�b�N�o�b�N���鎞��
	constexpr int kKnockBackTime = 5;
	//���𗎂Ƃ��Ȃ��V�[��
	constexpr int kNotDropBloodScene = 2;
	//��{�I��HP
	constexpr int kBaseHp = 5;;
	//HP�̐U�ꕝ
	constexpr int kHpLange = 3;
	//�v���C���[�ƂԂ��������̊�{�I�ȃ_���[�W
	constexpr int kBaseDamage = 2;
	//�v���C���[�ƂԂ��������̃_���[�W�̐U�ꕝ
	constexpr int kDamageLange = 5;
	//��{�I�ȗ��Ƃ����̂̎�ސ�
	constexpr int kBaseDropKind = 3;
	//�󔠂̉摜�T�C�Y
	constexpr int kGraphSize = 32;
	//�󔠂̊g�嗦
	constexpr double kGraphScale = 2.0;
	//�A�C�e���𗎂Ƃ����̗��Ƃ��͈�
	constexpr int kDropPosLange = 30;
	//�Ԃ��������ɏo���p�[�e�B�N���̏��
	constexpr int kParticleInfo[4] = { 40, 4, 5, 0 };
}
TreasureBox::TreasureBox(SceneMain* sceneMain) :
	m_pMain(sceneMain),
	m_hp(GetRand(kHpLange) + kBaseHp),
	m_colScale(kColScale),
	m_handle(-1),
	m_isExist(true),
	m_knockBackVec(),
	m_knockBackPow(kKnockBackPow),
	m_exp(0),
	m_gold(0),
	m_knockBackTime(0),
	m_magicCount(0),
	m_nowState(Game::State::kNormal),
	m_pParticle()
{
}

TreasureBox::~TreasureBox()
{
}

void TreasureBox::Init(Vec2 pos)
{
	m_nowState = Game::State::kNormal;
	//�S�������ʒu�ɏo�Ă��܂�Ȃ��悤�ɏ����΂炯������
	m_pos.x = pos.x + GetRand(kDropPosLange) - kDropPosLange / 2;
	m_pos.y = pos.y + GetRand(kDropPosLange) - kDropPosLange / 2;

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
	if (m_nowState != Game::State::kDelete)
	{
		m_pos -= m_knockBackVec;
		//�m�b�N�o�b�N����
		if (m_knockBackVec.x != 0 || m_knockBackVec.y != 0)
		{
			//�m�b�N�o�b�N���鎞�ԃJ�E���g
			m_knockBackTime++;
			//�K��̎��Ԃ��߂�����
			if (m_knockBackTime > kKnockBackTime)
			{
				//�m�b�N�o�b�N�̑傫����0�ɂ���
				m_knockBackVec *= 0;
				//���ԃJ�E���g��0�ɂ���
				m_knockBackTime = 0;
			}
		}
		m_circleCol.SetCenter(m_pos, static_cast<float>(m_colScale));
		if (m_hp < 0)
		{
			//�ǂ̏������s���邩�ݒ肷��
			int randomNumber;
			//2�X�e�[�W�܂ł͌��𗎂Ƃ��Ȃ��悤�ɂ���
			if (m_pMain->GetSceneNum() < kNotDropBloodScene)
			{
				//���𗎂Ƃ��Ȃ��悤��1���炷
				randomNumber = GetRand(kBaseDropKind -1 );
			}
			else
			{
				randomNumber = GetRand(kBaseDropKind);
			}
			if (randomNumber == static_cast<int>(Dropkind::kPortion))
			{
				//�̗͉񕜃A�C�e���𗎂Ƃ����������
				//�|�[�V�����̃������m��
				std::shared_ptr<Portion> pPortion
					= std::make_shared<Portion>();
				//�|�[�V�����̏���������
				pPortion->Init(m_pos);
				pPortion->SetHandle(m_handle);
				//�|�[�V�����𗎂Ƃ�����
				m_pMain->AddItem(pPortion);
			}
			else if (randomNumber == static_cast<int>(Dropkind::kBlood))
			{
				for (int i = 0; i < kDropBlood; i++)
				{
					//���̃������̊m��
					std::shared_ptr<Blood> pBlood
						= std::make_shared<Blood>();
					//���̏���������
					pBlood->Init(m_pos);
					pBlood->SetHandle(m_handle);
					//���𗎂Ƃ�����
					m_pMain->AddItem(pBlood);
				}
			}
			else if (randomNumber == static_cast<int>(Dropkind::kExp))
			{
				for (int i = 0; i < kDropExp; i++)
				{
					//�o���l�̃������̊m��
					std::shared_ptr<Exp> pExp
						= std::make_shared<Exp>();
					//�o���l�̏���������
					pExp->Init(m_pos);
					pExp->SetExp(m_exp);
					pExp->SetHandle(m_handle);
					//�o���l�𗎂Ƃ�����
					m_pMain->AddItem(pExp);
				}
			}
			else if (randomNumber == static_cast<int>(Dropkind::kGold))
			{
				for (int i = 0; i < kDropGold; i++)
				{
					//�����̃������m��
					std::shared_ptr<Gold> pGold
						= std::make_shared<Gold>();
					//�����̏���������
					pGold->Init(m_pos);
					pGold->SetPrice(m_gold);
					pGold->SetHandle(m_handle);

					//�����𗎂Ƃ�����
					m_pMain->AddItem(pGold);
				}
			}
			//��Ԃ�ω�������
			m_nowState = Game::State::kDelete;

		}
		if (m_nowState == Game::State::kHitMagic)
		{
			m_magicCount++;
			if (m_magicCount > kHitMagicInterval)
			{
				m_nowState = Game::State::kNormal;
				m_magicCount = 0;
			}
		}
	}
}

void TreasureBox::Draw()
{

	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, static_cast<int>(Game::ItemGraph::kDropTreasure) * kGraphSize,
		kGraphSize, kGraphSize,
		kGraphScale,
		0.0,
		m_handle, true, false);
	//�����蔻��̕\��
#ifdef _DEBUG
	m_circleCol.Draw(m_colScale, 0x0000ff, false);
#endif
}

void TreasureBox::HitPlayer(Player* player)
{
	m_hp -= GetRand(kDamageLange) + kBaseDamage;
	//�m�b�N�o�b�N����������
	m_knockBackVec = player->GetMoveVec() * -1;
	m_knockBackVec.Normalize();
	m_knockBackVec *= kKnockBackScale;
	//�Փ˓_�̍��W���o��
	m_hitPos = (player->GetPos() + m_pos) / 2;
	//�����G�t�F�N�g���o��
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_hitPos, static_cast<float>(kParticleInfo[0]), static_cast<float>(kParticleInfo[1]), kParticleInfo[2], kParticleInfo[3]);
		m_pMain->AddParticle(m_pParticle);
	}
}

void TreasureBox::HitMagic()
{
	m_nowState = Game::State::kHitMagic;
	//�����G�t�F�N�g���o��
	for (int i = 0; i < kParticleVol; i++)
	{
		m_pParticle = new Particle(m_pos, static_cast<float>(kParticleInfo[0]), static_cast<float>(kParticleInfo[1]), kParticleInfo[2], kParticleInfo[3]);
		m_pMain->AddParticle(m_pParticle);
	}
	m_hp--;
	if (m_hp < 0)
	{
		//�ǂ̏������s���邩�ݒ肷��
		int randomNumber = GetRand(kBaseDropKind);

		if (randomNumber == static_cast<int>(Dropkind::kPortion))
		{
			//�̗͉񕜃A�C�e���𗎂Ƃ����������
			//�|�[�V�����̃������m��
			std::shared_ptr<Portion> pPortion
				= std::make_shared<Portion>();
			//�|�[�V�����̏���������
			pPortion->Init(m_pos);
			pPortion->SetHandle(m_handle);
			//�|�[�V�����𗎂Ƃ�����
			m_pMain->AddItem(pPortion);
		}
		else if (randomNumber == static_cast<int>(Dropkind::kBlood))
		{
			for (int i = 0; i < kDropBlood; i++)
			{
				//���̃������̊m��
				std::shared_ptr<Blood> pBlood
					= std::make_shared<Blood>();
				//���̏���������
				pBlood->Init(m_pos);
				pBlood->SetHandle(m_handle);
				//���𗎂Ƃ�����
				m_pMain->AddItem(pBlood);
			}
		}
		else if (randomNumber == static_cast<int>(Dropkind::kExp))
		{
			for (int i = 0; i < kDropExp; i++)
			{
				//�o���l�̃������̊m��
				std::shared_ptr<Exp> pExp
					= std::make_shared<Exp>();
				//�o���l�̏���������
				pExp->Init(m_pos);
				pExp->SetExp(m_exp);
				pExp->SetHandle(m_handle);
				//�o���l�𗎂Ƃ�����
				m_pMain->AddItem(pExp);
			}
		}
		else if (randomNumber == static_cast<int>(Dropkind::kGold))
		{
			for (int i = 0; i < kDropGold; i++)
			{
				//�����̃������m��
				std::shared_ptr<Gold> pGold
					= std::make_shared<Gold>();
				//�����̏���������
				pGold->Init(m_pos);
				pGold->SetPrice(m_gold);
				pGold->SetHandle(m_handle);
				//�����𗎂Ƃ�����
				m_pMain->AddItem(pGold);
			}
		}
		//��Ԃ�ω�������
		m_nowState = Game::State::kDelete;

	}
}
