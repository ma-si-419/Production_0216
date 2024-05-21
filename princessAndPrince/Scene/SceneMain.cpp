#include "DxLib.h"
#include "Pad.h"
#include "CircleCol.h"

#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
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
	constexpr int kClearTime = 240;
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
	constexpr int kResultTime = 30 + kClearTime;
	//�N���A���̉��o�̎���
	constexpr int kDanceTime = 60 + kResultTime;
	//���x���A�b�v�̕������o��Y���W
	constexpr int kLevelUpPosY = 380;
	//���x���A�b�v�̕������オ����EY���W
	constexpr int kLevelUpPosYMax = 330;
	//�������[�h���n�߂�܂ł̎���
	constexpr int kSpecialModeStartTime = 30;
	//�t�H���g�̔����̑傫��
	constexpr int kFontHalfSize = 24;
	//READY��\�����鎞��
	constexpr int kReadyTime = 120;
	//�X�e�[�W�̃T�C�Y
	constexpr int kStageGraphSize = 640;
	constexpr int kAllStageSize = kStageGraphSize * 7;
	//�{�b�N�X�̉�]���鑬��
	constexpr float kBoxRotaSpeed = 0.15f;
	//�{�b�N�X�̑傫���Ȃ鑬��
	constexpr float kBoxZoomSpeed = 0.4f;
	//�e�X�e�[�W�̃{�X�̐�
	constexpr int kBossVol[8] = { 1,1,2,2,3,1,1,14 };
	//�����{�b�N�X�̏����{��
	constexpr float kBoxInitialRatio = 25.0f;
	//�e�X�e�[�W�̃`���[�g���A���̐�
	constexpr int kTutorialVol[4] = { 2,0,3,1 };
	//�e�X�e�[�W�̃`���[�g���A���������ڂ���n�܂邩
	constexpr int kStartTutorialNum[4] = { 0,0,3,6 };
	//�G�̎��
	constexpr int kEnemyKindVol = 6;
	//�{��Q�[�W�����܂�悤�ɂȂ�X�e�[�W��
	constexpr int kSpecialGaugeChargeStageNum = 3;
	//�P������ꂽ�Ƃ��ɒ��܂�{��Q�[�W�̔{��
	constexpr float kSpecialGaugeChargeRate = 1.5f;
	//�{��Q�[�W�������Ɍ����Ă����Q�[�W�̗�
	constexpr float kSpecialGaugeSubRate = 0.3f;
	//�{��Q�[�W�������̃p�[�e�B�N���̏��
	constexpr int kSpecialModeParticleInfo[4] = { 1000,10,3,2 };
	//�����p�[�e�B�N���̏��
	constexpr int kWhiteParticleInfo[4] = { 40,4,5,0 };
	//�w�i�̉���
	constexpr int kBgWidth = 965;
	//LEVELUP�ƕ\��������W
	constexpr int kLevelupPosX = 530;
	//LEVELUP�̐F
	const int kLevelUpColor = GetColor(255, 255, 0);
	//�`���[�g���A����\��������W
	constexpr int kTutorialPosX = 85;
	constexpr int kTutorialPosY = 290;
	//�|�[�Y��ʂ�UI���W
	constexpr int kPauseUiPosX = 430;
	constexpr int kPauseUiPosY[2] = { 700,800 };
	//�`���[�g���A�����ǂ��܂ŕ\�����邩
	constexpr int kTutorialStage = 4;
	//�G�̃|�b�v����Ԋu
	constexpr int kEnemyPopFrame = 40;
	//�|�[�Y�������̍����w�i�̓����x
	constexpr int kPauseBlackBoxAlphe = 170;
	//�{�胂�[�h���ɓ��������̍����w�i�̓����x
	constexpr int kAngryModeBlackBoxAlphe = 150;

}
SceneMain::SceneMain(SceneManager& sceneManager, DataManager& DataManager, int stageNum) :
	Scene(sceneManager, DataManager),
	m_isMusicFlag(true),
	m_enemyPopTimeCount(0),
	m_isClearFlag(false),
	m_bossCount(kBossCount),
	m_killBossCount(0),
	m_clearTime(0),
	m_nextEnemyKind(0),
	m_nextEnemyPopTime(0),
	m_specialGauge(0),
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
	m_startLoopTimeCount(0),
	m_attackSe(-1),
	m_weakAttackSe(-1),
	m_isBossFlag(false),
	m_danceMusic(-1),
	m_resultGold(-1),
	m_resultExp(-1),
	m_isWitchParticle(false),
	m_levelUpPosY(kLevelUpPosY),
	m_isUpLevel(false),
	m_isGameOver(false),
	m_isHalfExp(true),
	m_isHalfGold(true),
	m_lastSpace(true),
	m_selectScene(stageNum),
	m_pauseGraph(0),
	m_isShowReady(false),
	m_readyCount(0),
	m_isShowTutorial(true),
	m_isLastKey(false),
	m_isLastSe(true),
	m_boxAngle(0),
	m_boxRatio(kBoxInitialRatio),
	m_isMoveBox(true),
	m_startTutorialNum(0),
	m_isDeathTutorial(false),
	m_specialModeStartCount(0),
	m_pParticle(),
	m_expList(),
	m_isBackSelectScene(false),
	m_isGameOverString(false),
	m_isSelectKeyDown(false)
{
	//�v���C���[�̃O���t�B�b�N�̃��[�h
	m_playerHandle = m_dataManager.SearchGraph("playerGraph");
	//Player�̃R���X�g���N�^
	m_pPlayer = new Player(this);
	//�v���C���[�̃����o�ϐ��ɃA�N�Z�X
	m_pPlayer->SetHandle(m_playerHandle);
	m_pPlayer->SetPlayerAngryGraph(m_dataManager.SearchGraph("angryMonkeyGraph"));
	m_pPlayer->SetAngryFireGraph(m_dataManager.SearchGraph("angryFireGraph"));
	m_pPlayer->SetTurnSe(m_dataManager.SearchSound("playerTurnSe"));
	m_pPlayer->SetDeathSe(m_dataManager.SearchSound("playerDeathSe"));
	m_pPlayer->SetStandUpSe(m_dataManager.SearchSound("playerStandUpSe"));
	//�v�����Z�X�̃O���t�B�b�N�̃��[�h
	m_princessHandle = m_dataManager.SearchGraph("princessGraph");
	//Princess�̃R���X�g���N�^
	m_pPrincess = new Princess(this);
	//�v�����Z�X�̃����o�ϐ��ɃA�N�Z�X
	m_pPrincess->SetHandle(m_princessHandle);
	m_pPrincess->SetMagicGraph(m_dataManager.SearchGraph("itemGraph"));
	m_pPrincess->SetArrowGraph(m_dataManager.SearchGraph("arrowGraph"));
	m_pPrincess->SetMagicArrowGraph(m_dataManager.SearchGraph("magicArrowGraph"));
	//�G�̃O���t�B�b�N�̃��[�h
	m_enemyHandle = m_dataManager.SearchGraph("enemyGraph");
	//�w�i�̃O���t�B�b�N�̃��[�h
	m_bgHandle = m_dataManager.SearchGraph("allFieldBgGraph");
	//�A�C�e���̃O���t�B�b�N�̃��[�h
	m_itemHandle = m_dataManager.SearchGraph("itemGraph");
	//�|�[�Y���J�����Ƃ��ɏo��摜�̃��[�h
	m_pauseGraph = m_dataManager.SearchGraph("PAUSEGraph");
	//�{�^���O���t�B�b�N
	m_buttonsGraph = m_dataManager.SearchGraph("buttonsGraph");
	//Ui��\������w�i
	m_uiBgGraph = m_dataManager.SearchGraph("mainUiBgGraph");
	//�Ԃ������Ƃ��̉��̃��[�h
	m_attackSe = m_dataManager.SearchSound("attackSe");
	//�_���X�̉��̃��[�h
	m_danceMusic = m_dataManager.SearchSound("danceSe");
	//���U���g�̌��ʉ��̃��[�h
	m_resultGold = m_dataManager.SearchSound("resultGoldSe");
	m_resultExp = m_dataManager.SearchSound("resultExpSe");
	//�}�b�v�̉��y�̃��[�h
	m_fieldBgm = m_dataManager.SearchSound("fieldBgm");
	//�{�X���o�Ă������̉��y�̃��[�h
	m_bossBgm = m_dataManager.SearchSound("bossBgm");
	//�_���X�̑O��Se�̃��[�h
	m_beforeDanceSe = m_dataManager.SearchSound("beforeDanceSe");
	//����{�^�����������Ƃ���Se�̃��[�h
	m_appSe = m_dataManager.SearchSound("approveSe");
	//�J�[�\���𓮂������Ƃ��̉��̃��[�h
	m_cursorSe = m_dataManager.SearchSound("cursorSe");
	//B�{�^�����������Ƃ��̉�
	m_cancelSe = m_dataManager.SearchSound("cancelSe");
	//�R�C�����E�������̉�
	m_coinSe = m_dataManager.SearchSound("coinSe");
	//�R�C�����E�������̉�
	m_expSe = m_dataManager.SearchSound("expSe");
	//�����@���o�����Ƃ��̌��ʉ�
	m_windMagicSe = m_dataManager.SearchSound("windMagicSe");
	//�����@���o�����Ƃ��̌��ʉ�
	m_fireMagicSe = m_dataManager.SearchSound("fireMagicSe");
	//�G�ƃv�����Z�X���Ԃ��������̌��ʉ�
	m_hitPrincessSe = m_dataManager.SearchSound("hitPrincessSe");
	//�������[�h���n�߂�Ƃ��ɂȂ炷���ʉ�
	m_specialModeSe = m_dataManager.SearchSound("specialModeSe");
	//�|�[�V�������Ƃ������̌��ʉ�
	m_portionSe = m_dataManager.SearchSound("portionSe");
	//�����E�������̌��ʉ�
	m_bloodSe = m_dataManager.SearchSound("bloodSe");
	//����n�������̌��ʉ�
	m_passBloodSe = m_dataManager.SearchSound("passBloodSe");
	//�{�胂�[�h���ɕ\������摜
	m_angryMarkGraph = m_dataManager.SearchGraph("angryMarkGraph");
	//�Q�[���J�n���ɕ\������摜
	m_readyGraph = m_dataManager.SearchGraph("READYGraph");
	//�Q�[���I�[�o�[���̌��ʉ�
	m_princessDeathSe = m_dataManager.SearchSound("princessDeathSe");
	//�����{�b�N�X�̉摜
	m_boxGraph = m_dataManager.SearchGraph("boxGraph");
	//�`���[�g���A���摜
	m_tutorialGraph[0] = m_dataManager.SearchGraph("tutorialGraph1");
	m_tutorialGraph[1] = m_dataManager.SearchGraph("tutorialGraph2");
	m_tutorialGraph[2] = m_dataManager.SearchGraph("tutorialGraph3");
	m_tutorialGraph[3] = m_dataManager.SearchGraph("tutorialGraph4");
	m_tutorialGraph[4] = m_dataManager.SearchGraph("tutorialGraph5");
	m_tutorialGraph[5] = m_dataManager.SearchGraph("tutorialGraph6");
	m_tutorialGraph[6] = m_dataManager.SearchGraph("tutorialGraph7");

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
	//���@�̃O���t�B�b�N��ݒ�
	m_pUi->SetMagicGraph(m_itemHandle);
	//���@��Ui�̔w�i��ݒ肷��
	m_pUi->SetMagicBgGraph(m_dataManager.SearchGraph("magicUiBgGraph"));
	//�{��Q�[�W��Ui��ݒ肷��
	m_pUi->SetAngryGaugeGraph(m_dataManager.SearchGraph("angryGaugeUiGraph"));
	//�{��Q�[�W�����܂�������UI��ݒ肷��
	m_pUi->SetAngryButtonGraph(m_dataManager.SearchGraph("angryButtonMarkGraph"));
	//�{��Q�[�W���g���Ȃ�����UI��ݒ肷��
	m_pUi->SetStoneAngryGaugeGraph(m_dataManager.SearchGraph("stoneAngryGaugeGraph"));
	//�I�΂ꂽ�V�[���ɂ���ĕ\������`���[�g���A����ݒ肷��
	if (m_selectScene < kTutorialStage)
	{
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
	}
	//5�X�e�[�W�ڂ���̓`���[�g���A����\�����Ȃ�
	else
	{
		m_tutorialNum = 0;
		m_nowShowTutorialNum = 0;
	}
	switch (m_selectScene)
	{
	case 0://1�X�e�[�W�ڂ̃`���[�g���A�����
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
		break;
	case 1:
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_nowShowTutorialNum = 0;
		break;
	case 2:
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
		break;
	case 3:
		m_tutorialNum = kTutorialVol[m_selectScene];
		m_startTutorialNum = kStartTutorialNum[m_selectScene];
		m_nowShowTutorialNum = m_startTutorialNum;
		break;
	default:
		m_tutorialNum = 0;
		m_nowShowTutorialNum = 0;
		break;
	}
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	{
		//�\������`���[�g���A�����Ȃ�������
		if (m_tutorialNum == 0)
		{
			m_isShowTutorial = false;
			m_isShowReady = true;
		}
		//�o�Ă���G�̏���ݒ肷��
		m_bossCount = kBossVol[m_selectScene];
		SetEnemyInfo(m_selectScene);
		//�t�@�C�����J��
		std::ifstream ifs("./data/expLevel.txt");
		int loopCount = 0;
		//�A���Ă����l��Ԃ��z��
		vector<string> tempS;
		//�z����쐬
		char str[kArraySize];
		//�����������s���ǂݍ���
		while (ifs.getline(str, kArraySize))
		{
			int temp;
			temp = std::stoi(str);
			m_expList[loopCount] = temp;
			loopCount++;
		}
		ifs.close();
	}
	m_pPlayer->Init();
	m_pPrincess->Init();
}


void SceneMain::Update(Pad& pad)
{
	//�|�[�Y�≉�o���ȂǈȊO�̏ꍇ������

	XINPUT_STATE m_input;
	GetJoypadXInputState(DX_INPUT_PAD1, &m_input);
	//�f�o�b�O�p
#ifdef _DEBUG
	if (m_input.Buttons[XINPUT_BUTTON_LEFT_THUMB] || CheckHitKey(KEY_INPUT_0))
	{
		CountKillBoss();
	}
#endif 
	if (m_boxRatio > 0)
	{
		m_boxAngle -= kBoxRotaSpeed;
		m_boxRatio -= kBoxZoomSpeed;
	}
	else if (m_isMoveBox)
	{
		m_boxRatio = 0;
		m_isMoveBox = false;
	}
	//�ŏ��Ɏ��񂾂Ƃ��Ƀ`���[�g���A����\������
	if (m_isDeathTutorial)
	{
		if (CheckHitKey(KEY_INPUT_RETURN) || m_input.Buttons[XINPUT_BUTTON_A])
		{
			m_isDeathTutorial = false;
			m_isStop = false;
			m_isLastKey = true;
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
		}
	}
	//�V�[���ړ��̉��o���I�������
	if (!m_isMoveBox && !m_isDeathTutorial)
	{
		//�G���^�[�L�[���������玟�̃`���[�g���A���Ɉڍs����
		if (CheckHitKey(KEY_INPUT_RETURN) && !m_isLastKey && m_isShowTutorial ||
			m_input.Buttons[XINPUT_BUTTON_A] && !m_isLastKey && m_isShowTutorial)
		{
			m_nowShowTutorialNum++;
			m_isLastKey = true;
			PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
			//���ׂẴ`���[�g���A����\��������
			if (m_startTutorialNum + m_tutorialNum <= m_nowShowTutorialNum)
			{
				//�`���[�g���A�����I������
				m_isShowTutorial = false;
				//READY��\������
				m_isShowReady = true;
			}
		}
		//�G���^�[�L�[���A���œ��͂���Ȃ��悤��
		else if (!CheckHitKey(KEY_INPUT_RETURN) && !m_input.Buttons[XINPUT_BUTTON_A])
		{
			m_isLastKey = false;
		}
	}
	//READY���\������Ă����
	if (m_isShowReady)
	{
		m_readyCount++;
		if (m_readyCount > kReadyTime)
		{
			m_isShowReady = false;
		}
	}
	if (!m_isPause && !m_isShowReady && !m_isShowTutorial && m_boxRatio == 0)
	{
		//���y�Đ�
		if (m_isMusicFlag && !m_isBossFlag)
		{
			PlaySoundMem(m_fieldBgm, DX_PLAYTYPE_LOOP);
			m_isMusicFlag = false;
		}
		else if (m_isMusicFlag && m_isBossFlag)
		{
			StopSoundMem(m_fieldBgm);
			PlaySoundMem(m_bossBgm, DX_PLAYTYPE_LOOP);
			m_isMusicFlag = false;
		}
		//�|�[�Y�{�^���������ꂽ��
		if (!m_pPrincess->IsDeath())
		{
			if (m_input.Buttons[XINPUT_BUTTON_START] || CheckHitKey(KEY_INPUT_P))
			{
				m_isPause = true;
			}
		}
		//�V�[���ړ�
		if (m_pPrincess->IsDeath())
		{
			//���y�̍Đ����~�߂�
			StopSoundFile();
			if (!m_isResult)
			{
				PlaySoundMem(m_princessDeathSe, DX_PLAYTYPE_BACK);
			}
			for (auto& enemy : m_pEnemy)
			{
				if (enemy)
				{
					if (enemy->m_nowState != Game::State::kDelete)
						//�G�l�~�[�̈ړ����~�߂�
						enemy->m_nowState = Game::State::kStop;
				}
			}
			StopSoundMem(m_fieldBgm);
			StopSoundMem(m_bossBgm);
			m_pPlayer->m_nowState = Game::State::kStop;
			m_isGameOver = true;
			//���U���g��ʂɈڍs����
			m_isResult = true;
		}
		//���U���g��Ԃ̂Ƃ��͎~�܂�悤�ɂ���
		if (!m_isStop)
		{
			//�G�l�~�[�̃X�^�b�N���Ȃ��Ȃ�܂ŉ�
			if (!m_popEnemyList.empty())
			{
				//�{�X��|������G���o�Ă��Ȃ��悤�ɂ���
				if (m_killBossCount < m_bossCount && !m_pPrincess->IsDeath())
				{
					m_enemyPopTimeCount++;
				}
				//�ݒ肵�����ԂɂȂ�����
				if (m_enemyPopTimeCount > m_nextEnemyPopTime * kEnemyPopFrame)
				{
					//�J�E���g��������
					m_enemyPopTimeCount = 0;
					//�G�l�~�[���o��������
					CreateEnemy(m_nextEnemyKind);
					//�o�Ă���G���{�X�������特�y��ς���
					if (m_nextEnemyKind > kEnemyKindVol && !m_isBossFlag)
					{
						m_isBossFlag = true;
						m_isMusicFlag = true;
					}
					//���ɏo�Ă���G�l�~�[�̏�������
					popEnemy temp = m_popEnemyList.top();
					m_nextEnemyPopTime = temp.popTime;
					m_nextEnemyKind = temp.enemyKind;
					m_popEnemyList.pop();
				}
			}
			m_pPlayer->Update();
			m_pPrincess->Update();
			m_pUi->Update();
			//�G�l�~�[�̃A�b�v�f�[�g
			for (auto& enemy : m_pEnemy)
			{
				if (enemy)
				{
					//��Ԃ�kDelete����Ȃ��ꍇ�̂ݓ���
					if (enemy->m_nowState != Game::State::kDelete)
					{
						enemy->Update();
						//�v���C���[�ƃG�l�~�[���Ԃ������Ƃ�
						if (m_pPlayer->m_nowState != Game::State::kDelete &&//�v���C���[������ł��Ȃ��Ƃ���
							IsCollision(m_pPlayer->GetColCircle(), enemy->GetColCircle()) &&//�v���C���[�ƃG�l�~�[���Ԃ�������
							enemy->m_nowState != Game::State::kHitPlayer && enemy->m_nowState != Game::State::kStop)//�G�l�~�[��kDelete����Ȃ��Ƃ��̂�
						{
							//�G�l�~�[�̃_���[�W�������s��
							enemy->HitPlayer(*m_pPlayer, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
							//�v���C���[�̃_���[�W�������s��
							m_pPlayer->HitEnemy(*enemy, IsCollision(m_pPlayer->GetColCircle(), enemy->GetWeakCircle()));
							//�X�y�V�����Q�[�W���}�b�N�X����Ȃ�������Q�[�W���㏸������
							if (!m_isSpecialMode && m_selectScene >= kSpecialGaugeChargeStageNum)
							{
								//�G�̍U���͂ɉ����ăQ�[�W���㏸������
								AddSpecialGauge(enemy->GetAtk());
							}
							//�G�l�~�[�̏�Ԃ𐄈ڂ�����
							enemy->m_nowState = Game::State::kHitPlayer;
							PlaySoundMem(m_attackSe, DX_PLAYTYPE_BACK);
						}
						//�����ƃG�l�~�[���Ԃ������Ƃ�
						if (IsCollision(m_pPrincess->GetColCircle(), enemy->GetColCircle()) &&
							m_pPrincess->m_nowState != Game::State::kDelete && enemy->m_nowState != Game::State::kStop)
						{
							//�����̃_���[�W�������s��,�G�l�~�[�̃m�b�N�o�b�N���s��
							m_pPrincess->HitEnemy(*enemy);
							PlaySoundMem(m_hitPrincessSe, DX_PLAYTYPE_BACK);
							if (!m_isSpecialMode && m_selectScene > kSpecialGaugeChargeStageNum)
							{
								//�P������ꂽ�Ƃ��͒��܂�Q�[�W�ʂ��������₷
								AddSpecialGauge(enemy->GetAtk() * kSpecialGaugeChargeRate);
							}
						}
						//���@�̏���
						for (auto& magic : m_pMagic)
						{

							if (magic &&//magic��null����Ȃ��ꍇ
								IsCollision(magic->GetCircleCol(), enemy->GetColCircle()) &&//Magic��Enemy���Ԃ�������
								enemy->m_nowState != Game::State::kHitMagic)//��Ԃ�kHitMagic����Ȃ�������
							{
								//�G�l�~�[�̏�Ԃ�ω�������
								enemy->m_nowState = Game::State::kHitMagic;
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
				if (magic->m_nowState != Game::State::kDelete)
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
				if (item->m_nowState != Game::State::kDelete)
				{
					item->Update();
					//�A�C�e���ƃv���C���[���ڐG���A�A�C�e���̏�Ԃ�kNone����Ȃ�������
					if (IsCollision(m_pPlayer->GetColCircle(), item->GetColCircle()) &&
						item->m_nowState != Game::State::kNone)
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
				if (treasure->m_nowState != Game::State::kDelete)
				{
					treasure->Update();
					//�v���C���[�ƂԂ�������
					if (IsCollision(m_pPlayer->GetColCircle(), treasure->GetColCircle()))
					{
						PlaySoundMem(m_attackSe, DX_PLAYTYPE_BACK);
						m_pPlayer->HitTreasure(treasure);
						treasure->HitPlayer(m_pPlayer);
					}
					//���@�ƂƂԂ�������
					for (auto& magic : m_pMagic)
					{
						if (magic &&//magic��null����Ȃ�������
							IsCollision(magic->GetCircleCol(), treasure->GetColCircle()) &&
							treasure->m_nowState != Game::State::kHitMagic)
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
		//�{�胂�[�h�������\��������
		if (m_specialGauge >= kMaxSpecialGauge && m_pPrincess->m_nowState != Game::State::kDelete)
		{
			if (m_lastSpace && !m_isStop)
			{
				if (m_input.Buttons[XINPUT_BUTTON_Y] || CheckHitKey(KEY_INPUT_SPACE))
				{
					m_isSpecialMode = true;
					m_isStop = true;
					m_specialModeStartCount = 0;
					m_pPlayer->SetSpecialMode();
					m_pPrincess->StartSpecialMode();
					m_lastSpace = false;
					PlaySoundMem(m_specialModeSe, DX_PLAYTYPE_BACK);
				}
			}
		}
		//�{�胂�[�h�̉��o������
		if (m_isSpecialMode && m_isStop)
		{
			m_specialModeStartCount++;
			if (m_specialModeStartCount > kSpecialModeStartTime)
			{
				m_isStop = false;
				m_pPlayer->StartSpecialMode();
				m_lastSpace = true;
			}
		}
		//�{�胂�[�h��������������
		if (m_isSpecialMode && !m_isStop)
		{
			//�������Q�[�W�����炵�Ă���
			m_specialGauge -= kSpecialGaugeSubRate;
			//�J�E���g��i�߂�
			m_particleCount++;
			//�w�i�ɕ\������p�[�e�B�N���𐶐�����
			if (m_particleCount > kParticleInterval)
			{
				m_particleCount = 0;
				m_pParticle = new Particle(m_pPrincess->GetPos(), static_cast<float>(kSpecialModeParticleInfo[0]), static_cast<float>(kSpecialModeParticleInfo[1]), kSpecialModeParticleInfo[2], kSpecialModeParticleInfo[3]);
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
		if (m_input.Buttons[XINPUT_BUTTON_B] || CheckHitKey(KEY_INPUT_ESCAPE))
		{
			PlaySoundMem(m_cancelSe, DX_PLAYTYPE_BACK);
			m_isPause = false;
		}
		//A�{�^������������I�����Ă��鍀�ڂɉ����ď������s��
		if (m_input.Buttons[XINPUT_BUTTON_A] || CheckHitKey(KEY_INPUT_RETURN))
		{
			switch (m_pauseSelectNum)
			{
			case 0:
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				m_isPause = false;
				break;
			case 1:
				m_isPause = false;
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				StopSoundMem(m_fieldBgm);
				StopSoundMem(m_bossBgm);
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectScene), true);
				return;
				break;
			}
		}
		if (!m_isSelectKeyDown)
		{
			//��L�[�������ꂽ��
			if (m_input.Buttons[XINPUT_BUTTON_DPAD_UP] || CheckHitKey(KEY_INPUT_W))
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_pauseSelectNum--;
				if (m_pauseSelectNum < 0)
				{
					m_pauseSelectNum = kMaxPauseNum;
				}
				m_isSelectKeyDown = true;
			}
			//���L�[�����͂��ꂽ��
			else if (m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] || CheckHitKey(KEY_INPUT_S))
			{
				PlaySoundMem(m_cursorSe, DX_PLAYTYPE_BACK);
				m_pauseSelectNum++;
				if (m_pauseSelectNum > kMaxPauseNum)
				{
					m_pauseSelectNum = 0;
				}
				m_isSelectKeyDown = true;
			}
		}
		//��L�[�Ɖ��L�[�������ꂽ��
		if (!m_input.Buttons[XINPUT_BUTTON_DPAD_UP] && !m_input.Buttons[XINPUT_BUTTON_DPAD_DOWN] &&
			!CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
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
		StopSoundMem(m_fieldBgm);
		StopSoundMem(m_bossBgm);
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
		if (!CheckSoundMem(m_beforeDanceSe))
		{
			PlaySoundMem(m_beforeDanceSe, DX_PLAYTYPE_BACK);
		}

		//�v�����Z�X�̈ʒu�ɔ����G�t�F�N�g���o��
		if (!m_isWitchParticle)
		{
			for (int i = 0; i < kParticleVol; i++)
			{
				m_pParticle = new Particle(m_pPrincess->GetPos(), //�p�[�e�B�N���̏�������
					static_cast<float>(kWhiteParticleInfo[0]), static_cast<float>(kWhiteParticleInfo[1]), kWhiteParticleInfo[2], kWhiteParticleInfo[3]);
				AddParticle(m_pParticle);
			}
			m_isWitchParticle = true;
		}


		//�}�b�v��ɂ���G������
		for (const auto& enemy : m_pEnemy)
		{
			if (enemy && enemy->m_nowState != Game::State::kDelete)
			{
				enemy->m_nowState = Game::State::kDelete;
				Vec2 temp;
				//������Ƃ��ɃG�t�F�N�g���o��
				temp = enemy->GetPos();
				//�����G�t�F�N�g���o��
				for (int i = 0; i < kParticleVol; i++)
				{
					m_pParticle = new Particle(m_pPrincess->GetPos(), //�p�[�e�B�N���̏�������
						static_cast<float>(kWhiteParticleInfo[0]), static_cast<float>(kWhiteParticleInfo[1]), kWhiteParticleInfo[2], kWhiteParticleInfo[3]);;
					AddParticle(m_pParticle);
				}
			}
		}
		//�v���C���[��O�Ɍ�����
		m_pPlayer->TurnFront();
		m_pPrincess->TransStone();
		PlaySoundMem(m_danceMusic, DX_PLAYTYPE_BACK);

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
		if (m_isExpLoop)
		{
			m_startLoopTimeCount++;
			//�Q�[���I�[�o�[�������甼�����炷
			if (m_isHalfExp && m_isGameOver)
			{
				m_pPlayer->SubExpGameOver();
				m_isHalfExp = false;
			}
			//�o���l��0�ɂȂ�܂ł܂킷
			if (m_pPlayer->GetExp() != 0 &&
				m_startLoopTimeCount > kStartLoopTime)//���[�v���n�܂�܂ŏ������Ԃ��Ƃ�
			{
				//���炷�ʂ����߂�
				temp = GetDigits(m_pPlayer->GetExp());
				if (temp > m_expList[UserData::userMainLevel] - UserData::userExp)
				{
					temp = m_expList[UserData::userMainLevel] - UserData::userExp;
				}
				m_pPlayer->SubExp(temp);
				UserData::userExp += temp;
				if (UserData::userExp >= m_expList[UserData::userMainLevel])
				{
					UserData::userMainLevel++;
					m_isUpLevel = true;
				}
			}
			else if (m_pPlayer->GetExp() == 0)
			{
				if (!CheckSoundMem(m_resultExp) && !m_isGoldLoop)
				{
					PlaySoundMem(m_resultExp, DX_PLAYTYPE_BACK);
				}
				//�o���l�̃��[�v���I�������S�[���h�̃��[�v�ɍs��
				m_isExpLoop = false;
				m_isGoldLoop = true;
				m_pUi->ShowGold();
			}
		}
		if (m_isGoldLoop)
		{

			if (m_isHalfGold && m_isGameOver)
			{
				m_pPlayer->SubGoldGameOver();
				m_isHalfGold = false;
			}
			if (m_pPlayer->GetGold() != 0 &&
				m_startLoopTimeCount > kStartGoldLoopTime)
			{
				//���炷�ʂ����߂�
				temp = GetDigits(m_pPlayer->GetGold());
				m_pPlayer->SubGold(temp);
				UserData::userGold += temp;
			}
			else if (m_pPlayer->GetGold() == 0)
			{
				if (!CheckSoundMem(m_resultGold) && !m_isEnd)
					PlaySoundMem(m_resultGold, DX_PLAYTYPE_BACK);
				m_isEnd = true;
				m_pUi->ShowLeaveButton();
			}
		}
		if (m_isEnd && m_input.Buttons[XINPUT_BUTTON_A] || m_isEnd && CheckHitKey(KEY_INPUT_RETURN))
		{
			if (m_isLastSe)
			{
				PlaySoundMem(m_appSe, DX_PLAYTYPE_BACK);
				m_isLastSe = false;
			}
			//�܂��N���A���ĂȂ��X�e�[�W��������N���A�������Ƃ�ۑ�����
			if (m_selectScene >= UserData::userClearStageNum && m_pPrincess->m_nowState != Game::State::kDelete)
			{
				UserData::userClearStageNum = m_selectScene + 1;
			}
			m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager, m_selectScene), true);
			return;
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	//�N���A���Ƀv���C���[�����x���A�b�v���Ă�����
	if (m_isUpLevel)
	{
		if (m_levelUpPosY >= kLevelUpPosYMax)
		{
			m_levelUpPosY--;
		}

	}
}

void SceneMain::Draw()
{

	//�v���C��ʂ̔w�i
	DrawRectExtendGraph(0, 0, kBgWidth, Game::kPlayScreenHeight, 0, kAllStageSize - (kStageGraphSize * m_selectScene), kStageGraphSize, kStageGraphSize, m_bgHandle, true);
	if (m_isSpecialMode)
	{
		//�{�胂�[�h�ɓ�������w�i���Â�����
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAngryModeBlackBoxAlphe);
		DrawBox(0, 0, kBgWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	for (auto& magic : m_pMagic)
	{
		//magic��null����Ȃ��ꍇ
		if (magic)
		{
			//�A�C�e�������݂��Ă���ꍇ
			if (magic->m_nowState != Game::State::kDelete)
			{
				magic->Draw();

			}
		}
	}
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState != Game::State::kDelete &&
				item->m_nowState != Game::State::kNone)
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
			if (treasure->m_nowState != Game::State::kDelete)
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
	m_pPrincess->Draw();
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
	m_pUi->Draw();
	for (auto& item : m_pItem)
	{
		if (item)
		{
			if (item->m_nowState == Game::State::kNone)
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
	if (m_isGameOver)
	{
		m_pUi->GameOverUI();
	}
	//���x���A�b�v���Ă����烌�x���A�b�v�ƕ\������
	if (m_isUpLevel)
	{
		DrawString(kLevelupPosX, m_levelUpPosY, "LEVEL UP", kLevelUpColor);
	}
	//�|�[�Y��ʂ��J���Ă�����
	if (m_isPause)
	{
		//�����{�b�N�X��\������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kPauseBlackBoxAlphe);
		DrawBox(0, 0, kBgWidth, Game::kPlayScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//����������\������
		int stringWidth = GetStringLength("�Â���") * kFontHalfSize / 2;
		DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[0],
			"�Â���", GetColor(255, 255, 255));
		stringWidth = GetStringLength("��߂�") * kFontHalfSize / 2;
		DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[1],
			"��߂�", GetColor(255, 255, 255));
		DrawGraph(0, 0, m_pauseGraph, true);

		//�I�΂�Ă��镶����Ԃ��\������
		switch (m_pauseSelectNum)
		{
		case 0:
			stringWidth = GetStringLength("�Â���") * kFontHalfSize / 2;
			DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[0],
				"�Â���", GetColor(255, 0, 0));
			break;
		case 1:
			stringWidth = GetStringLength("��߂�") * kFontHalfSize / 2;
			DrawString(kPauseUiPosX - stringWidth, kPauseUiPosY[1],
				"��߂�", GetColor(255, 0, 0));
			break;

		}
	}
	//�X�e�[�W�J�n����READY��\������
	if (m_isShowReady)
	{
		DrawGraph(0, 0, m_readyGraph, true);
	}
	//�X�e�[�W�̍ŏ��Ƀ`���[�g���A�����\������
	if (m_isShowTutorial)
	{
		DrawGraph(kTutorialPosX, kTutorialPosY, m_tutorialGraph[m_nowShowTutorialNum], true);
	}
	//�ŏ��Ɏ��񂾂Ƃ��̃`���[�g���A����\������
	if (m_isDeathTutorial)
	{
		DrawGraph(kTutorialPosX, kTutorialPosY, m_tutorialGraph[2], true);
	}
	//��]���鍕���{�b�N�X��\������
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kPlayScreenHeight / 2,//���W
		m_boxRatio, m_boxAngle, m_boxGraph, true, 0, 0);
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
		m_pEnemy[i]->SetItemHandle(m_itemHandle);
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
	else
	{
		return 0;
	}
}
int SceneMain::GetNextExp()
{
	return m_expList[UserData::userMainLevel] - UserData::userExp;
}

void SceneMain::SetEnemyInfo(int stageNum)
{
	//�t�@�C�����J��
	std::ifstream ifs;
	if (stageNum == 0)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo1.txt");
	}
	else if (stageNum == 1)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo2.txt");
	}
	else if (stageNum == 2)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo3.txt");
	}
	else if (stageNum == 3)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo4.txt");
	}
	else if (stageNum == 4)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo5.txt");
	}
	else if (stageNum == 5)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo6.txt");
	}
	else if (stageNum == 6)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo7.txt");
	}
	else if (stageNum == 7)
	{
		ifs = std::ifstream("./data/enemy/EnemyInfo8.txt");
	}
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
		tempEnemy.popTime = std::stoi(tempS[1]);
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