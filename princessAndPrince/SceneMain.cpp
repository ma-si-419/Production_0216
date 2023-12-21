#include <DxLib.h>
#include <cassert>
#include "Pad.h"
#include "CircleCol.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "MyString.h"

#include "MagicBase.h"
#include "Blood.h"
#include "TreasureBox.h"
#include "Player.h"
#include "Princess.h"
#include "Enemy.h"
#include "UI.h"
#include "UserData.h"
namespace
{
	//�A�C�e���̍ő吔
	constexpr int kMaxItem = 1024;
	//�����ɑ��݂��閂�@�̍ő吔
	constexpr int kMaxMagicValue = 32;
	//���Ă錌�̗ʂ̍ő吔
	constexpr float kMaxBlood = 10;
	//�G�l�~�[�����܂��C���^�[�o��(������)
	constexpr int kEnemyPopInterval = 50;
	//�󔠂̓����ɑ��݂���ő吔
	constexpr int kMaxTreasureBox = 16;
	//�G�̍ő吔
	constexpr int kMaxEnemy = 256;
	//�z��̃T�C�Y
	constexpr int kArraySize = 81;
	//�N���A������̎���
	constexpr int kClearTime = 180;
	//�|���{�X�̐�(������)
	constexpr int kBossCount = 1;
}
SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0),
	m_clearFlag(false),
	m_bossCount(kBossCount),
	m_killBossCount(0),
	m_clearTime(0),
	m_nextEnemyKind(0),
	m_nextEnemyPopTime(0)
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
	m_pEnemy.resize(kMaxEnemy);
	//�A�C�e���̍ő吔��ݒ�
	m_pItem.resize(kMaxItem);
	//���@�̍ő吔��ݒ�
	m_pMagic.resize(kMaxMagicValue);
	//�󔠂̍ő吔��ݒ�
	m_pTreasure.resize(kMaxTreasureBox);
	//UI�̃R���X�g���N�^
	m_pUi = new UI(m_pPlayer);
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	{
		//�t�@�C�����J��
		std::ifstream ifs("./data/EnemyInfo.txt");
		//�A���Ă����l��Ԃ��z��
		vector<string> tempS;
		//�z����쐬
		char str[kArraySize];
		//�����������s���ǂݍ���
		while (ifs.getline(str, kArraySize))
		{
			//����
			tempS = MyString::split(str, ",");
			popEnemy tempEnemy;
			tempEnemy.enemyKind = std::stoi(tempS[0]);
			tempEnemy.popTime = std::stof(tempS[1]);
			m_popEnemyList.push(tempEnemy);
		}
		//�ŏ��ɏo�Ă���G�l�~�[�̏�������
		popEnemy firstEnemyInfo = m_popEnemyList.top();
		m_nextEnemyPopTime = firstEnemyInfo.popTime;
		m_nextEnemyKind = firstEnemyInfo.enemyKind;
		//�X�^�b�N�̒��̓ǂݎ������������
		m_popEnemyList.pop();
		//�t�@�C�������
		ifs.close();

	}
	m_pPlayer->Init();
	m_pPrincess->Init();
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
	//�G�l�~�[�̃X�^�b�N���Ȃ��Ȃ�܂ŉ�
	if (!m_popEnemyList.empty())
	{
		m_enemyPopTimeCount++;
		if (m_enemyPopTimeCount > m_nextEnemyPopTime * 20)
		{
			//�J�E���g��������
			m_enemyPopTimeCount = 0;
			//�G�l�~�[���o��������
			CreateEnemy(m_nextEnemyKind);
			//���ɏo�Ă���G�l�~�[�̏�������
			popEnemy temp = m_popEnemyList.top();
			m_enemyPopTimeCount = temp.popTime;
			m_nextEnemyKind = temp.enemyKind;
			m_popEnemyList.pop();
		}
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
					enemy->m_nowState != Game::kHit)//�G�l�~�[��kDelete����Ȃ��Ƃ��̂�
				{
					//�G�l�~�[�̃_���[�W�������s��
					enemy->HitPlayer(*m_pPlayer,IsCollision(m_pPlayer->GetColCircle(),enemy->GetWeakCircle()));
					//�v���C���[�̃_���[�W�������s��
					m_pPlayer->HitEnemy(*enemy,IsCollision(m_pPlayer->GetColCircle(),enemy->GetWeakCircle()));
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
					m_pPlayer->PickUpItem(item);
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
			if (treasure->m_nowState != Game::kDelete)
			{
				treasure->Update();
				//�v���C���[�ƂԂ�������
				if (IsCollision(m_pPlayer->GetColCircle(), treasure->GetColCircle()))
				{
					m_pPlayer->HitTreasure(treasure);
					treasure->HitPlayer();
				}
				//���@�ƂƂԂ�������
				for (auto& magic : m_pMagic)
				{
					if (magic &&//magic��null����Ȃ�������
						IsCollision(magic->GetCircleCol(), treasure->GetColCircle()))
					{
						treasure->HitMagic();
						treasure->m_nowState = Game::kHitMagic;
					}
				}

			}
			else//�A�C�e�������݂��Ȃ���ԂɂȂ�����
			{
				treasure = nullptr;
			}
		}
	}
	//�N���A����
	if (m_killBossCount >= m_bossCount)
	{
		m_clearTime++;

		m_clearFlag = true;
		if (m_clearTime > kClearTime)
		{
			UserData::userGold += m_pPlayer->GetGold();
			UserData::userExp += m_pPlayer->GetExp();
			m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
		}
	}
}

void SceneMain::Draw()
{
	
	//�v���C��ʂ̔w�i
	DrawGraph(0, 0, m_bgHandle, true);
	
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
	m_pPlayer->Draw();
	for (const auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			enemy->Draw();

		}
	}
	DrawBox(960, 0, Game::kScreenWidth, Game::kPlayScreenHeight, GetColor(100, 100, 100), true);
	m_pPrincess->Draw();
	m_pUi->Draw();
	//�N���A������
	if (m_clearFlag)
	{
		int stringWidth = GetDrawStringWidth("�Q�[���N���A", -1);
		DrawString((Game::kPlayScreenWIdth - stringWidth) / 2, 200, "�Q�[���N���A", GetColor(0, 0, 0));
		DrawString(300, 500, "�l���o���l", GetColor(0, 0, 0));
		DrawString(300, 600, "�l���S�[���h", GetColor(0, 0, 0));
		DrawFormatString(600, 500, GetColor(0, 0, 0), "%d", m_pPlayer->GetGold());
		DrawFormatString(600, 600, GetColor(0, 0, 0), "%d", m_pPlayer->GetExp());

	}
	//UI�̕\���i���j
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

bool SceneMain::CreateEnemy(int enemyKind)
{
	//�A�C�e���̔z��̒�������for������
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		//�g�p���Ȃ玟�̃`�F�b�N��
		if (m_pEnemy[i]) continue;
		//�����ɗ����Ƃ������Ƃ�m_pShot[i] == nullptr
		m_pEnemy[i] = make_shared<Enemy>(this);
		m_pEnemy[i]->SetHandle(m_enemyHandle);
		m_pEnemy[i]->Init(enemyKind);
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

