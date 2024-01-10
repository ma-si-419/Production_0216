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
#include "Particle.h"
#include "Enemy.h"
#include "UI.h"
#include "UserData.h"
namespace
{
	//�A�C�e���̍ő吔
	constexpr int kMaxItem = 512;
	//�p�[�e�B�N���̍ő吔
	constexpr int kMaxParticle = 512;
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
	constexpr int kClearTime = 360;
	//�|���{�X�̐�(������)
	constexpr int kBossCount = 1;
	//�������[�h�̃Q�[�W�̍ő��
	constexpr float kMaxSpecialGauge = 100.0f;
	//�p�[�e�B�N�����o�Ă���Ԋu
	constexpr int kParticleInterval = 5;
	//�|�[�Y��ʂ̍��ڐ�
	constexpr int kMaxPauseNum = 1;
	//�N���A���̃��[�v���n�܂�܂ł̎���
	constexpr int kStartLoopTime = 60;
	//�N���A���̃S�[���h�̃��[�v���n�܂�܂ł̎���
	constexpr int kStartGoldLoopTime = 60 + kStartLoopTime;
	//�o���p�[�e�B�N���̐�
	constexpr int kParticleVol = 30;
	//�N���A���Ɏ~�߂鎞��
	constexpr int kResultTime = 60 + kClearTime;
	//�N���A���̉��o�̎���
	constexpr int kDanceTime = 60 + kResultTime;
}
SceneMain::SceneMain(SceneManager& manager, int stageNum) :
	Scene(manager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0),
	m_isClearFlag(false),
	m_bossCount(kBossCount),
	m_killBossCount(0),
	m_clearTime(0),
	m_nextEnemyKind(0),
	m_nextEnemyPopTime(0),
	m_specialGauge(99.1f),
	m_isSpecialMode(false),
	m_isPause(false),
	m_isStop(false),
	m_particleCount(0),
	m_isResult(false),
	m_pauseSelectNum(0),
	m_isExpLoop(false),
	m_isGoldLoop(false),
	m_isEnd(false),
	m_isClearString(false),
	m_startLoopTimeCount(0)
{
	//�v���C���[�̃O���t�B�b�N�̃��[�h
	m_playerHandle = LoadGraph("data/image/Monkey.png");
	//Player�̃R���X�g���N�^
	m_pPlayer = new Player(this);
	//�v���C���[�̃����o�ϐ��ɃA�N�Z�X
	m_pPlayer->SetHandle(m_playerHandle);
	//�v�����Z�X�̃O���t�B�b�N�̃��[�h
	m_princessHandle = LoadGraph("data/image/Princess.png");
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
	//�p�[�e�B�N���̍ő吔��ݒ�
	m_pParticleArray.resize(kMaxParticle);
	//UI�̃R���X�g���N�^
	m_pUi = new UI(m_pPlayer, m_pPrincess, this);
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
	//���y�Đ�
//	if (m_flag)
//	{
//		PlaySoundFile("data/sound/mainBgm.mp3", DX_PLAYTYPE_LOOP);
//		m_flag = false;
//	}
//�|�[�Y�≉�o���ȂǈȊO�̏ꍇ������

	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//�f�o�b�O�p
	if (m_input.Buttons[XINPUT_BUTTON_LEFT_THUMB])
	{
		CountKillBoss();
	}
	if (!m_isPause)
	{
		//�|�[�Y�{�^���������ꂽ��
		if (m_input.Buttons[XINPUT_BUTTON_START])
		{
			m_isPause = true;
		}
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
			//�{�X��|������G���o�Ă��Ȃ��悤�ɂ���
			if (m_killBossCount < m_bossCount)
			{
				m_enemyPopTimeCount++;
			}
			//�ݒ肵�����ԂɂȂ�����
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
		//���U���g��Ԃ̂Ƃ��͎~�܂�悤�ɂ���
		if (!m_isStop)
		{
			m_pPlayer->Update();
			m_pPrincess->Update();
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
							enemy->m_nowState != Game::kHitPlayer)//�G�l�~�[��kDelete����Ȃ��Ƃ��̂�
						{
							//�G�l�~�[�̃_���[�W�������s��
							enemy->HitPlayer(*m_pPlayer, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
							//�v���C���[�̃_���[�W�������s��
							m_pPlayer->HitEnemy(*enemy, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
							//�X�y�V�����Q�[�W���}�b�N�X����Ȃ�������Q�[�W���㏸������
							if (!m_isSpecialMode)
							{
								//�G�̍U���͂ɉ����ăQ�[�W���㏸������
								AddSpecialGauge(enemy->GetAtk());
							}
							//�G�l�~�[�̏�Ԃ𐄈ڂ�����
							enemy->m_nowState = Game::kHitPlayer;
						}
						//�����ƃG�l�~�[���Ԃ������Ƃ�
						if (IsCollision(m_pPrincess->GetColCircle(), enemy->GetColCircle()))
						{
							//�����̃_���[�W�������s��,�G�l�~�[�̃m�b�N�o�b�N���s��
							m_pPrincess->HitEnemy(*enemy);
							if (!m_isSpecialMode)
							{
								AddSpecialGauge(enemy->GetAtk());
							}
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
				if (item->m_nowState != Game::kDelete)
				{
					item->Update();
					//�A�C�e���ƃv���C���[���ڐG���A�A�C�e���̏�Ԃ�kNone����Ȃ�������
					if (IsCollision(m_pPlayer->GetColCircle(), item->GetColCircle()) &&
						item->m_nowState != Game::kNone)
					{
						m_pPlayer->PickUpItem(item);
					}
				}
			}
		}
		//��
		for (auto& treasure : m_pTreasure)
		{
			//treasure��null����Ȃ��ꍇ
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
						treasure->HitPlayer(m_pPlayer);
					}
					//���@�ƂƂԂ�������
					for (auto& magic : m_pMagic)
					{
						if (magic &&//magic��null����Ȃ�������
							IsCollision(magic->GetCircleCol(), treasure->GetColCircle()) &&
							treasure->m_nowState != Game::kHitMagic)
						{
							treasure->HitMagic();
						}
					}

				}
				else//�A�C�e�������݂��Ȃ���ԂɂȂ�����
				{
					treasure = nullptr;
				}
			}
		}
		//�p�[�e�B�N��
		for (auto& particle : m_pParticleArray)
		{
			//item��null����Ȃ��ꍇ
			if (particle)
			{
				//�A�C�e�������݂��Ă���ꍇ
				if (particle->GetIsExist())
				{
					particle->Update();
				}
				else//�A�C�e�������݂��Ȃ���ԂɂȂ�����
				{
					particle = nullptr;
				}
			}
		}
		if (m_specialGauge >= kMaxSpecialGauge)
		{
			if (m_input.Buttons[XINPUT_BUTTON_Y])
			{
				m_isSpecialMode = true;
			}
		}
		//�������[�h��������������
		if (m_isSpecialMode)
		{
			//�������Q�[�W�����炵�Ă���
			m_specialGauge -= 0.2f;
			//�J�E���g��i�߂�
			m_particleCount++;
			//�w�i�ɕ\������p�[�e�B�N���𐶐�����
			if (m_particleCount > kParticleInterval)
			{
				m_particleCount = 0;
				m_pParticle = new Particle(m_pPrincess->GetPos(), 2000, 10, 2, 2);
				AddParticle(m_pParticle);
			}
			//�Q�[�W��0�ɂȂ�����
			if (m_specialGauge < 0)
			{
				//�������[�h���I��点��
				m_isSpecialMode = false;
				m_specialGauge = 0;
			}
		}
	}
	//�|�[�Y���̏���
	else if (m_isPause)
	{
		//B�{�^�����������烁�C���V�[���ɖ߂�
		if (m_input.Buttons[XINPUT_BUTTON_B])
		{
			m_isPause = false;
		}
		//A�{�^������������I�����Ă��鍀�ڂɉ����ď������s��
		if (m_input.Buttons[XINPUT_BUTTON_A])
		{
			switch (m_pauseSelectNum)
			{
			case 0:
				m_isPause = false;
				break;
			case 1:
				m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
				break;

			}
		}
		//��L�[�������ꂽ��
		if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_isSelectKeyDown)
		{
			m_pauseSelectNum--;
			if (m_pauseSelectNum < 0)
			{
				m_pauseSelectNum = kMaxPauseNum;
			}
			m_isSelectKeyDown = true;
		}
		//���L�[�����͂��ꂽ��
		else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && !m_isSelectKeyDown)
		{
			m_pauseSelectNum++;
			if (m_pauseSelectNum > kMaxPauseNum)
			{
				m_pauseSelectNum = 0;
			}
			m_isSelectKeyDown = true;
		}
		//��L�[�Ɖ��L�[�������ꂽ��
		else if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN])
		{
			m_isSelectKeyDown = false;
		}

	}
	///////////////////////////////////////////////////////////////////////////////
	//�N���A����
	if (m_killBossCount >= m_bossCount)
	{
		//���Ԃ��J�E���g��������
		m_clearTime++;
	}
	//�A�C�e�����Ƃ鎞�Ԃ��Ƃ�
	if (m_clearTime > kClearTime)
	{
		//�������[�h���~�߂�
		m_isSpecialMode = false;
		//�v���C���[�ƃG�l�~�[�̓������~�߂�
		m_isStop = true;
	}
	//�������~�܂��ď���������
	if (m_clearTime > kResultTime && m_clearTime < kDanceTime)
	{
		//�}�b�v��ɂ���G������
		for (const auto& enemy : m_pEnemy)
		{
			if (enemy && enemy->m_nowState != Game::kDelete)
			{
				enemy->m_nowState = Game::kDelete;
				Vec2 temp;
				//������Ƃ��ɃG�t�F�N�g���o��
				temp = enemy->GetPos();
				//�����G�t�F�N�g���o��
				for (int i = 0; i < kParticleVol; i++)
				{
					m_pParticle = new Particle(temp, 40.0f, 4.0f, 5, 0);
					AddParticle(m_pParticle);
				}

			}
		}
		//�v���C���[��O�Ɍ�����
		m_pPlayer->TurnFront();
	}
	//�G����������ɏ��������Ԃ��J����
	if (m_clearTime > kDanceTime && !m_isResult)
	{
		//�v���C���[�̃N���A���̍s��������
		m_pPlayer->ClearDance();
	}
	if (m_isResult)
	{
		m_isClearFlag = true;
		int temp;
		m_startLoopTimeCount++;
		//�o���l��0�ɂȂ�܂ł܂킷
		if (m_pPlayer->GetExp() != 0 && m_isExpLoop &&
			m_startLoopTimeCount > kStartLoopTime)//���[�v���n�܂�܂ŏ������Ԃ��Ƃ�
		{
			//���炷�ʂ����߂�
			temp = GetDigits(m_pPlayer->GetExp());
			m_pPlayer->SubExp(temp);
			UserData::userExp += temp;
		}
		else if (m_pPlayer->GetExp() == 0 && m_isExpLoop)
		{
			//�o���l�̃��[�v���I�������S�[���h�̃��[�v�ɍs��
			m_isExpLoop = false;
			m_isGoldLoop = true;
			m_pUi->ShowGold();
		}

		if (m_pPlayer->GetGold() != 0 && m_isGoldLoop &&
			m_startLoopTimeCount > kStartGoldLoopTime)
		{
			//���炷�ʂ����߂�
			temp = GetDigits(m_pPlayer->GetGold());
			m_pPlayer->SubGold(temp);
			UserData::userGold += temp;
		}
		else if (m_pPlayer->GetGold() == 0 && m_isGoldLoop)
		{
			m_isEnd = true;
		}
		if (m_isEnd && m_input.Buttons[XINPUT_BUTTON_A])
		{
			m_manager.ChangeScene(std::make_shared<SceneSelect>(m_manager));
		}
	}
	///////////////////////////////////////////////////////////////////////////////
}

void SceneMain::Draw()
{

	//�v���C��ʂ̔w�i
	DrawGraph(0, 0, m_bgHandle, true);
	if (m_isSpecialMode)
	{
		//�������[�h�ɓ�������w�i���Â�����
		DrawBox(0, 0, Game::kPlayScreenWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
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
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState != Game::kDelete &&
				item->m_nowState != Game::kNone)
			{
				item->Draw();
			}
		}
	}
	for (auto& treasure : m_pTreasure)
	{
		//treasure��null����Ȃ��ꍇ
		if (treasure)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (treasure->m_nowState != Game::kDelete)
			{
				treasure->Draw();
			}
		}
	}
	for (const auto& enemy : m_pEnemy)
	{
		if (enemy)
		{
			enemy->Draw();

		}
	}
	m_pPlayer->Draw();
	for (auto& particle : m_pParticleArray)
	{
		//particle��null����Ȃ��ꍇ
		if (particle)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (particle->GetIsExist())
			{
				particle->Draw();
			}
		}
	}
	m_pPrincess->Draw();
	m_pUi->Draw();
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState == Game::kNone)
			{
				item->Draw();
			}
		}
	}
	//ClearDance���I�������
	if (m_isClearString)
	{
		m_pUi->SceneClearUI();
	}
	//�|�[�Y��ʂ��J���Ă�����
	if (m_isPause)
	{
		int stringWidth = GetDrawStringWidth("�Â���", 0);
		DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 700,
			"�Â���", GetColor(0, 0, 0));
		stringWidth = GetDrawStringWidth("��߂�", 0);
		DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 800,
			"��߂�", GetColor(0, 0, 0));
		switch (m_pauseSelectNum)
		{
		case 0:
			stringWidth = GetDrawStringWidth("�Â���", 0);
			DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 700,
				"�Â���", GetColor(255, 0, 0));
			break;
		case 1:
			stringWidth = GetDrawStringWidth("��߂�", 0);
			DrawString(Game::kPlayScreenWidth / 2 - stringWidth, 800,
				"��߂�", GetColor(255, 0, 0));
			break;

		}
	}
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

void SceneMain::AddSpecialGauge(float gauge)
{
	float addGauge;
	addGauge = gauge - m_pPlayer->GetDef();
	//�Œ�ł��P�͑�����悤�ɂ���
	if (addGauge < 0)
	{
		addGauge = 1;
	}
	m_specialGauge += addGauge;
	//���E�l�𒴂�����
	if (m_specialGauge > kMaxSpecialGauge)
	{
		//���݂̒l�����E�l�ɂ���
		m_specialGauge = kMaxSpecialGauge;
	}
}

int SceneMain::GetDigits(int num)
{
	if (num >= 10000)
	{
		return 10000;
	}
	else if (num >= 1000)
	{
		return 1000;
	}
	else if (num >= 100)
	{
		return 100;
	}
	else if (num >= 10)
	{
		return 10;
	}
	else if (num >= 1)
	{
		return 1;
	}
	else if (num <= 0)
	{
		return 0;
	}
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
	//�󔠂̔z��̒���
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

bool SceneMain::AddParticle(Particle* pParticle)
{
	//�p�[�e�B�N���̔z��̒���
	for (int i = 0; i < m_pParticleArray.size(); i++)
	{
		//�g�p���Ȃ玟�̃`�F�b�N��
		if (m_pParticleArray[i]) continue;
		//�����ɗ����Ƃ������Ƃ�m_pParticle[i] == nullptr
		m_pParticleArray[i] = pParticle;
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

