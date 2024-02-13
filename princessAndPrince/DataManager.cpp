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
	soundMap["attackSe"] = LoadSoundMem("data/sound/attackSe.mp3");
	//�_���X����Ƃ��̌��ʉ�
	soundMap["danceSe"] = LoadSoundMem("data/sound/clearSe.mp3");
	//���U���g��ʂ̃S�[���h�̌��ʉ�
	soundMap["resultGoldSe"] = LoadSoundMem("data/sound/goldSe.mp3");
	//���U���g��ʂ̌o���l�̌��ʉ�
	soundMap["resultExpSe"] = LoadSoundMem("data/sound/expSe.mp3");
	//�t�B�[���h��Bgm
	soundMap["fieldBgm"] = LoadSoundMem("data/sound/fieldBgm.wav");
	//�{�X���o�Ă�������Bgm
	soundMap["bossBgm"] = LoadSoundMem("data/sound/bossBgm.wav");
	//�_���X�̑O�̓G�����Ȃ��Ȃ鎞�̉�
	soundMap["beforeDanceSe"] = LoadSoundMem("data/sound/enemyDelete.mp3");
	//�R�C�����E�������̉�
	soundMap["coinSe"] = LoadSoundMem("data/sound/coinSe.mp3");
	//�o���l���E�������̉�
	soundMap["expSe"] = LoadSoundMem("data/sound/dropExpSe.mp3");
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
	soundMap["bloodSe"] = LoadSoundMem("data/sound/bloodSe.mp3");
	//����n�����Ƃ��̌��ʉ�
	soundMap["passBloodSe"] = LoadSoundMem("data/sound/passBloodSe.mp3");
	//�V���b�v��BGM
	soundMap["shopBgm"] = LoadSoundMem("data/sound/shopBgm.wav");
	//�Z���N�g�V�[����Bgm
	soundMap["selectBgm"] = LoadSoundMem("data/sound/selectBgm.wav");
	//�^�C�g���V�[����Bgm
	soundMap["titleBgm"] = LoadSoundMem("data/sound/titleBgm.wav");
	//�����@��Se
	soundMap["fireMagicSe"] = LoadSoundMem("data/sound/fireSe.mp3");
	//Princess�ɂԂ���������Se
	soundMap["hitPrincessSe"] = LoadSoundMem("data/sound/hitPrincessSe.mp3");
	//���C���V�[���Ɉڍs����Ƃ��ɂȂ炷��
	soundMap["moveMainSceneSe"] = LoadSoundMem("data/sound/moveMainSceneSe.mp3");
	//�v���C���[���|�ꂽ���ɖ炷��(���)
	soundMap["playerTurnSe"] = LoadSoundMem("data/sound/playerTurnSe.mp3");
	soundMap["playerDeathSe"] = LoadSoundMem("data/sound/playerDeathSe.mp3");
	//�v���C���[�������オ��Ƃ��ɂȂ炷��
	soundMap["playerStandUpSe"] = LoadSoundMem("data/sound/playerStandUpSe.mp3");
	//�P�����񂾂Ƃ��̌��ʉ�
	soundMap["princessDeathSe"] = LoadSoundMem("data/sound/princessDeathSe.mp3");
	//�Z�[�u�V�[����BGM
	soundMap["saveSceneBgm"] = LoadSoundMem("data/sound/saveSceneBgm.wav");
	//�v���C���[�̃O���t�B�b�N
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//�v�����Z�X�̃O���t�B�b�N
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//�G�l�~�[�̃O���t�B�b�N
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
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
	//�w�i�摜�܂Ƃ�
	graphMap["allFieldBgGraph"] = LoadGraph("data/image/allFieldBg.png");
	//�V���b�v�摜
	graphMap["shopGraph"] = LoadGraph("data/image/shop.png");
	//�I��ł���V�[���̃t���[��
	graphMap["selectSceneFrameGraph"] = LoadGraph("data/image/selectSceneFrame.png");
	//�^�C�g���V�[���̔w�i
	graphMap["titleSceneBgGraph"] = LoadGraph("data/image/titleBg.png");
	//�Q�[���J�n���ɏo��READY
	graphMap["READYGraph"] = LoadGraph("data/image/READY.png");
	//�|�[�Y���ɏo��PAUSE
	graphMap["PAUSEGraph"] = LoadGraph("data/image/PAUSE.png");
	//�^�C�g���̃��S
	graphMap["titleGraph"] = LoadGraph("data/image/title.png");
	//�{�^����UI
	graphMap["buttonsGraph"] = LoadGraph("data/image/buttons.png");
	//�Z���N�g��ʂ̃{�^������
	graphMap["selectSceneButtonUiGraph"] = LoadGraph("data/image/buttonsUi.png");
	//�V�[�����C����Ui��\������w�i
	graphMap["mainUiBgGraph"] = LoadGraph("data/image/mainUi.png");
	//���@��Ui��\������w�i�̃O���t�B�b�N
	graphMap["magicUiBgGraph"] = LoadGraph("data/image/witchMagicUi.png");
	//�{��Q�[�W��Ui
	graphMap["angryGaugeUiGraph"] = LoadGraph("data/image/angryGauge.png");
	//�{�胂�[�h���̉��q
	graphMap["angryMonkeyGraph"] = LoadGraph("data/image/angryMonkey.png");
	//�{�胂�[�h���̃G�t�F�N�g
	graphMap["angryFireGraph"] = LoadGraph("data/image/angryFire.png");
	//�r�b�N���}�[�N
	graphMap["exclamationMarkGraph"] = LoadGraph("data/image/exclamationMark.png");
	//�����{�b�N�X
	graphMap["boxGraph"] = LoadGraph("data/image/box.png");
	//������A�C�e���̉E��ɏo���摜
	graphMap["canBuyItemMarkGraph"] = LoadGraph("data/image/canBuyItemMark.png");
	//�Z�[�u�f�[�^��I�������ʂ̔w�i
	graphMap["sceneSaveGraph"] = LoadGraph("data/image/saveDataScene.png");
	//�Z�[�u�f�[�^�̃t���[���̉摜
	graphMap["saveDataFrameGraph"] = LoadGraph("data/image/saveDataFrame.png");
	//�Z�[�u�V�[���̃J�[�\��
	graphMap["saveSceneCursorGraph"] = LoadGraph("data/image/cursor.png");
	//���@�̕������������
	graphMap["magicArrowGraph"] = LoadGraph("data/image/magicArrow.png");
	//�{��Q�[�W�����܂����Ƃ��̃{�^���\���摜
	graphMap["angryButtonMarkGraph"] = LoadGraph("data/image/angryButtonMark.png");
	//�{��Q�[�W���g���Ȃ��Ƃ��̉摜
	graphMap["stoneAngryGaugeGraph"] = LoadGraph("data/image/stoneAngryGauge.png");
	//�V���b�v�ɕ\������߂�{�^��
	graphMap["backButtonGraph"] = LoadGraph("data/image/backButton.png");
	//�`���[�g���A���摜
	graphMap["tutorialGraph1"] = LoadGraph("data/image/tutorial1.png");
	graphMap["tutorialGraph2"] = LoadGraph("data/image/tutorial2.png");
	graphMap["tutorialGraph3"] = LoadGraph("data/image/tutorial3.png");
	graphMap["tutorialGraph4"] = LoadGraph("data/image/tutorial4.png");
	graphMap["tutorialGraph5"] = LoadGraph("data/image/tutorial5.png");
	graphMap["tutorialGraph6"] = LoadGraph("data/image/tutorial6.png");
	graphMap["tutorialGraph7"] = LoadGraph("data/image/tutorial7.png");
	//�X�g�[���[�摜
	graphMap["storyGraph1"] = LoadGraph("data/image/story1.png");
	graphMap["storyGraph2"] = LoadGraph("data/image/story2.png");
	graphMap["storyGraph3"] = LoadGraph("data/image/story3.png");
	graphMap["storyGraph4"] = LoadGraph("data/image/story4.png");
	graphMap["storyGraph5"] = LoadGraph("data/image/story5.png");
	graphMap["storyGraph6"] = LoadGraph("data/image/story6.png");
	graphMap["storyGraph7"] = LoadGraph("data/image/story7.png");
	graphMap["storyGraph8"] = LoadGraph("data/image/story8.png");

}
