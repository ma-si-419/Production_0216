#pragma once
#include "Scene.h"
#include<list>
using namespace std;
struct ItemPrice
{
	int playerItemPrice;
	int princessItemPrice;
};
class SceneShop : public Scene
{
public:
	SceneShop(SceneManager& sceneManager,DataManager& DataManager);
	virtual ~SceneShop();

	void Init();
	void Update(Pad& pad);
	void Draw();
	//���I�����Ă��鍀�ڂ�ۑ�����
	int m_stageSelectNum;
	//�A���ŃL�[��������ĂȂ��悤��
	bool m_isKeyDown;
	//�J�[�\�����A���ňړ����Ȃ��悤��
	bool m_isSelectKeyDown;
	//�����������ۑ�����
	int m_loopCount;
	//�v���C���[�̃A�C�e���̒l�i�̔z��
	int m_playerItemPriceList[20];
	//�����̃A�C�e���̒l�i�̔z��
	int m_princessItemPriceList[20];
	//�J�[�\���𓮂������Ƃ��̌��ʉ�
	int m_cursorSe;
	//�t�F�[�h���ɘA�łł��Ȃ��悤��
	bool m_isFade;
	//B�{�^�����������Ƃ��̉�
	int m_cancelSe;
	//�A�C�e���𔃂������̌��ʉ�
	int m_buySe;
	//�A�C�e���𔃂��Ȃ��������̌��ʉ�
	int m_missBuySe;
	//���l�̃O���t�B�b�N
	int m_traderGraph;
	//�v���C���[�̃O���t�B�b�N
	int m_playerGraph;
	//Princess�̃O���t�B�b�N
	int m_princessGraph;
	//Shop�̔w�i
	int m_bgGraph;
	//�����Ă����Ƃ��ɕ��͂�\��������
	bool m_isShowString;
	//�A�C�e���𔃂������ǂ���
	bool m_isBuy;
	//�A�C�e���̃O���t�B�b�N
	int m_itemGraph;
	//�A�C�e���̃t���[���̃O���t�B�b�N
	int m_itemFrameGraph;
};

