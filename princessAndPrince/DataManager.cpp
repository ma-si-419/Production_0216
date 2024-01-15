#include "DataManager.h"
#include "DxLib.h"

DataManager::DataManager()
{
	//�J�[�\���𓮂������̌��ʉ�
	soundMap["approveSe"] = LoadSoundMem("data/sound/approveSe.mp3");
	//�J�[�\���𓮂������̌��ʉ�
	soundMap["cancelSe"] = LoadSoundMem("data/sound/cancelSe.mp3");
	//�J�[�\���𓮂������̌��ʉ�
	soundMap["cursorSe"] = LoadSoundMem("data/sound/cursorSe.mp3");
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
	//�v���C���[�̃O���t�B�b�N
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//�v�����Z�X�̃O���t�B�b�N
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//�G�l�~�[�̃O���t�B�b�N
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
	//�w�i�̃O���t�B�b�N
	graphMap["bgGraph"] = LoadGraph("data/image/_bg.png");

}

DataManager::~DataManager()
{
}
