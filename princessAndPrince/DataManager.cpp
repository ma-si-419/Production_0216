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
	soundMap["fieldBgm"] = LoadSoundMem("data/sound/fieldBgm.wav");
	//�{�X���o�Ă�������Bgm
	soundMap["bossBgm"] = LoadSoundMem("data/sound/bossBgm.wav");
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
	//�������[�h���n�߂�Ƃ��ɖ炷���ʉ�
	soundMap["specialModeSe"] = LoadSoundMem("data/sound/specialModeSe.mp3");
	//�����@�̌��ʉ�
	soundMap["windMagicSe"] = LoadSoundMem("data/sound/windMagic.mp3");
	//�|�[�V�������E�������̌��ʉ�
	soundMap["portionSe"] = LoadSoundMem("data/sound/portionSe.mp3");
	//��_����Ȃ��Ƃ���ɂԂ��������̌��ʉ�
	soundMap["hitSe"] = LoadSoundMem("data/sound/hitSe.mp3");
	//�����E�������̌��ʉ�
	soundMap["bloodSe"] = LoadSoundMem("data/sound/bloodSe1.mp3");
	//����n�����Ƃ��̌��ʉ�
	soundMap["passBloodSe"] = LoadSoundMem("data/sound/passBloodSe.mp3");
	//�V���b�v��BGM
	soundMap["shopBgm"] = LoadSoundMem("data/sound/shopBgm.wav");
	//�Z���N�g�V�[����Bgm
	soundMap["selectBgm"] = LoadSoundMem("data/sound/selectBgm.wav");
	//�^�C�g���V�[����Bgm
	soundMap["titleBgm"] = LoadSoundMem("data/sound/titleBgm.wav");
	//�v���C���[�̃O���t�B�b�N
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//�v�����Z�X�̃O���t�B�b�N
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//�G�l�~�[�̃O���t�B�b�N
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
	//�w�i�̃O���t�B�b�N
	graphMap["bgGraph"] = LoadGraph("data/image/fieldBg.png");
	//���l�̃O���t�B�b�N
	graphMap["traderGraph"] = LoadGraph("data/image/trader.png");
	//�V���b�v�̔w�i
	graphMap["shopBgGraph"] = LoadGraph("data/image/shopBg.png");
	//�Z���N�g�V�[���̔w�i
	graphMap["selectBgGraph"] = LoadGraph("data/image/selectBg.png");
	//�A�C�e���̉摜
	graphMap["itemGraph"] = LoadGraph("data/image/item.png");
	//�A�C�e���̃t���[��
	graphMap["itemFrameGraph"] = LoadGraph("data/image/frame.png");
	//�S�[���h��X�e�[�W�����o���Ƃ��Ɍ��ɏo���{�b�N�X
	graphMap["backBoxGraph"] = LoadGraph("data/image/backBox.png");
	

}
