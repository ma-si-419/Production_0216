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
	//�A����A�{�^����������Ȃ��悤��
	bool m_isAKeyDown;
};

