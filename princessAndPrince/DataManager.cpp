#include "DataManager.h"
#include "DxLib.h"

DataManager::DataManager()
{
	


}

DataManager::~DataManager()
{
}

void DataManager::Init()
{
	//�J�[�\���𓮂������̌��ʉ�
	soundMap["approveSe"] = LoadSoundMem("data/sound/approveSe.mp3");
	//�J�[�\���𓮂������̌��ʉ�
	soundMap["cancelSe"] = LoadSoundMem("data/sound/cancelSe.mp3");
	//�J�[�\���𓮂������̌��ʉ�
	soundMap["cursorSe"] = LoadSoundMem("data/sound/cursorSe.mp3");
	//�V���b�v�ŃA�C�e���𔃂������̌��ʉ�
	soundMap["shopSe"] = LoadSoundMem("data/sound/shopSe.mp3");
	//�������[�h�𔭓��������̌��ʉ��̌��ʉ�
	soundMap["specialModeSe"] = LoadSoundMem("data/sound/specialModeSe.mp3");
	//�G�ɂԂ��������̉�
	soundMap["attackSe"] = LoadSoundMem("data/sound/attack3.mp3");
	//�_���X����Ƃ��̌��ʉ�
	soundMap["danceSe"] = LoadSoundMem("data/sound/clearSe.mp3");
	//���U���g��ʂ̃S�[���h�̌��ʉ�
	soundMap["resultGoldSe"] = LoadSoundMem("data/sound/gold1.mp3");
	//���U���g��ʂ̌o���l�̌��ʉ�
	soundMap["resultExpSe"] = LoadSoundMem("data/sound/exp1.mp3");
	//�t�B�[���h��Bgm
	soundMap["fieldBgm"] = LoadSoundMem("data/sound/mainBgm2.mp3");
	//�{�X���o�Ă�������Bgm
	soundMap["bossBgm"] = LoadSoundMem("data/sound/mainBgm1.mp3");
	//�_���X�̑O�̓G�����Ȃ��Ȃ鎞�̉�
	soundMap["beforeDanceSe"] = LoadSoundMem("data/sound/enemyDelete.mp3");
	//�R�C�����E�������̉�
	soundMap["coinSe"] = LoadSoundMem("data/sound/coinSe.mp3");
	//�o���l���E�������̉�
	soundMap["expSe"] = LoadSoundMem("data/sound/expSe.mp3");
	//�V���b�v�ŃA�C�e���𔃂������̉�
	soundMap["buySe"] = LoadSoundMem("data/sound/buySe.mp3");
	//�V���b�v�ŃA�C�e���𔃂��Ȃ��������̉�
	soundMap["missBuySe"] = LoadSoundMem("data/sound/missBuySe.mp3");
	//�v���C���[�̃O���t�B�b�N
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//�v�����Z�X�̃O���t�B�b�N
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//�G�l�~�[�̃O���t�B�b�N
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
	//�w�i�̃O���t�B�b�N
	graphMap["bgGraph"] = LoadGraph("data/image/_bg.png");
	//���l�̃O���t�B�b�N
	graphMap["traderGraph"] = LoadGraph("data/image/trader.png");
	//�V���b�v�̔w�i
	graphMap["shopBgGraph"] = LoadGraph("data/image/shopBg.png");
	//�A�C�e���̉摜
	graphMap["itemGraph"] = LoadGraph("data/image/item.png");
	//�A�C�e���̃t���[��
	graphMap["itemFrameGraph"] = LoadGraph("data/image/frame.png");
}
