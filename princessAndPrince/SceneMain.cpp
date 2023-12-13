#include <DxLib.h>
#include <cassert>
#include "Pad.h"
#include "CircleCol.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "MagicBase.h"
#include "Blood.h"
#include "TreasureBox.h"
#include "Player.h"
#include "Princess.h"
#include "Enemy.h"
#include "Bg.h"
#include "UI.h"
namespace
{
	//�A�C�e���̍ő吔
	constexpr int kMaxItem = 256;
	//�����ɑ��݂��閂�@�̍ő吔
	constexpr int kMaxMagicValue = 32;
	//���Ă錌�̗ʂ̍ő吔
	constexpr float kMaxBlood = 10;
	//�G�l�~�[�����܂��C���^�[�o��(������)
	constexpr int kEnemyPopInterval = 50;
	//�󔠂̓����ɑ��݂���ő吔
	constexpr int kMaxTreasureBox = 16;
}
SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0)
{
	//�v���C���[�̃O���t�B�b�N�̃��[�h
	m_playerHandle = LoadGraph("data/image/Monkey.png");
	//Player�̃R���X�g���N�^
	m_pPlayer = new Player;
	//�v���C���[�̃����o�ϐ��ɃA�N�Z�X
	m_pPlayer->SetHandle(m_playerHandle);
	//�v�����Z�X�̃O���t�B�b�N�̃��[�h
	m_princessHandle = LoadGraph("data/image/Monkey.png");
	//Princess�̃R���X�g���N�^
	m_pPrincess = new Princess(this);
	//�v�����Z�X�̃����o�ϐ��ɃA�N�Z�X
	m_pPrincess->SetHandle(m_princessHandle);
	//�G�̃O���t�B�b�N�̃��[�h
	m_enemyHandle = LoadGraph("data/image/Enemy.png");
	//�w�i�̃O���t�B�b�N�̃��[�h
	m_bgHandle = LoadGraph("data/image/_bg.png");
	//�G�̍ő吔��ݒ�
	m_pEnemy.resize(30);
	//�A�C�e���̍ő吔��ݒ�
	m_pItem.resize(kMaxItem);
	//���@�̍ő吔��ݒ�
	m_pMagic.resize(kMaxMagicValue);
	//�󔠂̍ő吔��ݒ�
	m_pTreasure.resize(kMaxTreasureBox);
	//UI�̃R���X�g���N�^
	m_pUi = new UI(m_pPlayer);
	for (auto& enemy : m_pEnemy)
	{
		////Enemy�̃R���X�g���N�^
		//enemy = std::make_shared<Enemy>(this, m_pPlayer);
		//Enemy�̃����o�ϐ��ɃA�N�Z�X
		/*enemy->SetHandle(m_enemyHandle);*/
	}
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pPrincess->Init();
}

void SceneMain::End()
{
}

void SceneMain::Update(Pad& pad)
{
	//�V�[���ړ�
	if (m_pPrincess->IsDeath())
	{
		//���y�̍Đ����~�߂�
		StopSoundFile();
		//�^�C�g���V�[���Ɉڍs����
		m_manager.ChangeScene(std::make_shared<SceneTitle>(m_manager));

		return;
	}
	//Enemy�̐������񂷌�Ŏd�l�ύX
	m_enemyPopTimeCount++;
	if (m_enemyPopTimeCount > kEnemyPopInterval)
	{
		//�J�E���g��������
		m_enemyPopTimeCount = 0;
		//�G�l�~�[���o��������
		createEnemy();


	}
	m_pPlayer->Update();
	m_pPrincess->Update();
	//���y�Đ�
//	if (m_flag)
//	{
//		PlaySoundFile("data/sound/mainBgm.mp3", DX_PLAYTYPE_LOOP);
//		m_flag = false;
//	}
	bool isLeaveFlag = true;
	//�G�l�~�[�̃A�b�v�f�[�g
	for (auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			//��Ԃ�kDelete����Ȃ��ꍇ�̂ݓ���
			if (enemy->m_nowState != Game::kDelete)
			{
				enemy->Update();
				//�v���C���[�ƃG�l�~�[���Ԃ������Ƃ�
				if (m_pPlayer->m_nowState != Game::kDelete &&//�v���C���[������ł��Ȃ��Ƃ���
					IsCollision(m_pPlayer->GetColCircle(), enemy->GetColCircle()) &&//�v���C���[�ƃG�l�~�[���Ԃ�������
					enemy->m_nowState != Game::kDelete)//�G�l�~�[��kDelete����Ȃ��Ƃ��̂�
				{
					//�G�l�~�[�̃_���[�W�������s��
					enemy->HitPlayer(*m_pPlayer);
					//�v���C���[�̃_���[�W�������s��
					m_pPlayer->HitEnemy(*enemy);
					//�G�l�~�[�̏�Ԃ𐄈ڂ�����
					enemy->m_nowState == Game::kHit;
				}
				//�����ƃG�l�~�[���Ԃ������Ƃ�
				if (IsCollision(m_pPrincess->GetColCircle(), enemy->GetColCircle()))
				{
					//�����̃_���[�W�������s��,�G�l�~�[�̃m�b�N�o�b�N���s��
					m_pPrincess->HitEnemy(*enemy);
				}
				//�G�l�~�[�ƃv���C���[�����ꂽ��(�Ԃ������Ƃ��̏�����������s���Ȃ����߂�)
				if (IsCollision(m_pPlayer->GetColCircle(), enemy->GetColCircle()) &&
					enemy->m_nowState == Game::kHit)
				{
					//�G�l�~�[�̏�Ԃ�kNormal�ɕω�������
					enemy->m_nowState = Game::kNormal;
				}
				for (auto& magic : m_pMagic)
				{

					if (magic &&//magic��null����Ȃ��ꍇ
						IsCollision(magic->GetCircleCol(), enemy->GetColCircle()) &&//Magic��Enemy���Ԃ�������
						enemy->m_nowState != Game::kHitMagic)//��Ԃ�kHitMagic����Ȃ�������
					{
						//�G�l�~�[�̏�Ԃ�ω�������
						enemy->m_nowState = Game::kHitMagic;
						//���@�̃_���[�W�������s��
						enemy->HitMagic(magic);					
					}
				}
			}
		}
	}


	for (auto& magic : m_pMagic)
	{
		//���@��null����Ȃ�
		if (magic)
		{
			//��Ԃ��f���[�g�ɂȂ��Ă��Ȃ��Ƃ�
			if (magic->m_nowState != Game::kDelete)
			{
				//���@�̍X�V�������s��
				magic->Update();
			}
			else//��Ԃ��f���[�g�ɂȂ����Ƃ�
			{
				//���@�̃|�C���^������
				magic = nullptr;

			}
		}
	}

	//������Player���Ԃ�������
	if (IsCollision(m_pPlayer->GetColCircle(), m_pPrincess->GetColCircle()) &&
		!m_pPrincess->IsMagic())
	{
		m_pPlayer->GiveBlood(m_pPrincess);
	}
	for (auto& item : m_pItem)
	{
		//item��null����Ȃ��ꍇ
		if (item)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (item->m_nowState == Game::kNormal)
			{
				item->Update();
				if (IsCollision(m_pPlayer->GetColCircle(), item->GetColCircle()))
				{
					m_pPlayer->PickUpItem(item->GetKind());
					item->m_nowState = Game::kDelete;
				}
			}
		}
	}
	for (auto& treasure : m_pTreasure)
	{
		//item��null����Ȃ��ꍇ
		if (treasure)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (treasure->m_nowState == Game::kNormal)
			{
				treasure->Update();
				//�����蔻�菈�����쐬����
				if (IsCollision(m_pPlayer->GetColCircle(), treasure->GetColCircle()))
				{
					m_pPlayer->HitTreasure(treasure);
				}
				//���@�ƃv���C���[

			}
			else//�A�C�e�������݂��Ȃ���ԂɂȂ�����
			{
				treasure = nullptr;
			}
		}
	}
}

void SceneMain::Draw()
{
	DrawGraph(0, 0, m_bgHandle, false);
	for (const auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			enemy->Draw();

		}
	}
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState == Game::kNormal)
			{
				item->Draw();
			}
		}
	}
	for (auto& magic : m_pMagic)
	{
		//magic��null����Ȃ��ꍇ
		if (magic)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (magic->m_nowState != Game::kDelete)
			{
				magic->Draw();

			}
		}
	}
	for (auto& treasure : m_pTreasure)
	{
		//item��null����Ȃ��ꍇ
		if (treasure)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (treasure->m_nowState == Game::kNormal)
			{
				treasure->Draw();
			}
		}
	}
	m_pPrincess->Draw();
	m_pPlayer->Draw();
	m_pUi->Draw();
}

bool SceneMain::AddItem(std::shared_ptr<ItemBase> pItem)
{
	//�A�C�e���̔z��̒�������for������
	for (int i = 0; i < m_pItem.size(); i++)
	{
		//�g�p���Ȃ玟�̃`�F�b�N��
		if (m_pItem[i]) continue;
		//�����ɗ����Ƃ������Ƃ�m_pShot[i] == nullptr
		m_pItem[i] = pItem;
		//�o�^������I��
		return true;
	}
	//�����ɗ����A�Ƃ������Ƃ�m_pShot�Ƀ|�C���^��o�^�ł��Ȃ�����
	return false;
}

bool SceneMain::createEnemy()
{
	//�A�C�e���̔z��̒�������for������
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		//�g�p���Ȃ玟�̃`�F�b�N��
		if (m_pEnemy[i]) continue;
		//�����ɗ����Ƃ������Ƃ�m_pShot[i] == nullptr
		m_pEnemy[i] = make_shared<Enemy>(this);
		m_pEnemy[i]->SetHandle(m_enemyHandle);
		m_pEnemy[i]->Init(GetRand(4));
		//�o�^������I��
		return true;
	}
	//�����ɗ����A�Ƃ������Ƃ�m_pShot�Ƀ|�C���^��o�^�ł��Ȃ�����
	return false;
}

bool SceneMain::AddMagic(MagicBase* pMagic)
{
	//���@�̔z��̒���
	for (int i = 0; i < m_pMagic.size(); i++)
	{
		//�g�p���Ȃ玟�̃`�F�b�N��
		if (m_pMagic[i]) continue;
		//�����ɗ����Ƃ������Ƃ�m_pShot[i] == nullptr
		m_pMagic[i] = pMagic;
		//�o�^������I��
		return true;
	}
	//�����ɗ����A�Ƃ������Ƃ�m_pShot�Ƀ|�C���^��o�^�ł��Ȃ�����
	return false;
}

bool SceneMain::AddTreasure(TreasureBox* pTreasure)
{
	//���@�̔z��̒���
	for (int i = 0; i < m_pTreasure.size(); i++)
	{
		//�g�p���Ȃ玟�̃`�F�b�N��
		if (m_pTreasure[i]) continue;
		//�����ɗ����Ƃ������Ƃ�m_pShot[i] == nullptr
		m_pTreasure[i] = pTreasure;
		//�o�^������I��
		return true;
	}
	//�����ɗ����A�Ƃ������Ƃ�m_pShot�Ƀ|�C���^��o�^�ł��Ȃ�����
	return false;
}

bool SceneMain::IsCollision(CircleCol col1, CircleCol col2)
{
	if (col1.IsCollsion(col2))
	{
		return true;
	}
	return false;
}

