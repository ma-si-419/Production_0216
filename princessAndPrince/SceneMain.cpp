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
	constexpr int kMaxItem = 256;
	constexpr int kMaxMagicValue = 32;
	constexpr float kMaxBlood = 10;
	constexpr int kEnemyPopInterval = 50;
}
SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_isMusicFlag(true),
	m_isTouch(false),
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

	CircleCol playerCircle = m_pPlayer->GetColCircle();
	CircleCol princessCircle = m_pPrincess->GetColCircle();
	for (auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			//�G�l�~�[��IsExist��false�̊Ԃ���Update����
			if (enemy->IsExist())
			{
				enemy->Update();
				//�v���C���[�ƃG�l�~�[���Ԃ������Ƃ�
				if (!m_pPlayer->GetDeathFlag() && playerCircle.IsCollsion(enemy->GetColCircle()))
				{
					enemy->OnDamage(*m_pPlayer);
					m_pPlayer->OnDamage(*enemy);
				}
				//�����ƃG�l�~�[���Ԃ������Ƃ�
				if (princessCircle.IsCollsion(enemy->GetColCircle()))
				{
					m_pPrincess->OnDamage(*enemy);
				}
			}
		}
	}

	//������Player���Ԃ�������
	if (princessCircle.IsCollsion(playerCircle) &&
		!m_pPrincess->IsMagic())
	{
		m_pPlayer->GiveBlood(m_pPrincess);
		m_isTouch = true;
	}
	for (auto& item : m_pItem)
	{
		//item��null����Ȃ��ꍇ
		if (item)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (item->IsExist())
			{
				item->Update();
				if (playerCircle.IsCollsion(item->GetColCircle()))
				{
					m_pPlayer->PickUpItem(item->GetKind());
					item->DeleteItem();
				}
			}
		}
	}
	for (auto& magic : m_pMagic)
	{
		//item��null����Ȃ��ꍇ
		if (magic)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (magic->IsExist())
			{
				magic->Update();

			}
			else
			{
				magic = nullptr;
			}
		}
	}
	for (auto& treasure : m_pTreasure)
	{
		//item��null����Ȃ��ꍇ
		if (treasure)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (treasure->IsExist())
			{
				treasure->Update();
				//�����蔻�菈�����쐬����
				//���@�ƃv���C���[

			}
			else
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
			if (item->IsExist())
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
			if (magic->IsExist())
			{
				magic->Draw();

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
		m_pEnemy[i] = make_shared<Enemy>(this, m_pPlayer);
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

