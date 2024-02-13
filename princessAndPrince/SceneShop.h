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
	SceneShop(SceneManager& sceneManager,DataManager& DataManager,int selectSceneNum);
	virtual ~SceneShop();

	void Init();
	void Update(Pad& pad);
	void Draw();
	/// <summary>
	/// �����ɉ����Đ��l���E�����ɂ���
	/// </summary>
	/// <param name="num">�E�����ɂ��������l</param>
	/// <returns>�E�����ɂ��������l*�t�H���g�̑傫��</returns>
	int ArrRight(int num);
	//�������擾����
	int GetDigits(int num);
	//�A��A�C�e���ɐԊۂ�t����
	void DrawRedCircle();

private:
	//���I�����Ă��鍀�ڂ�ۑ�����
	int m_itemSelectNum;
	//�A���ŃL�[��������ĂȂ��悤��
	bool m_isKeyDown;
	//�J�[�\�����A���ňړ����Ȃ��悤��
	bool m_isSelectKeyDown;
	//�����������ۑ�����
	int m_loopCount;
	//�v���C���[�̃A�C�e���̒l�i�̔z��
	int m_playerItemPriceList[21];
	//�����̃A�C�e���̒l�i�̔z��
	int m_princessItemPriceList[21];
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
	//�J�[�\���𓮂��������ǂ���
	bool m_isMoveCursor;
	//�A�C�e���̃O���t�B�b�N
	int m_itemGraph;
	//�A�C�e���̃t���[���̃O���t�B�b�N
	int m_itemFrameGraph;
	//�����S�[���h��\�����鎞�̌��̃{�b�N�X
	int m_backBoxGraph;
	//�A�C�e���̃t���[�����g��k������ϐ�
	float m_itemFrameRatio;
	//�A�C�e���t���[�����g�債�Ă��邩�k�����Ă��邩
	int m_isFrameRatio;
	//Bgm
	int m_bgm;
	//�Z���N�g�V�[���őI��ł����V�[����ۑ�����
	int m_selectSceneNum;
	//�I�񂾏��i�̃��x����MAX��������
	bool m_isMax;
	//�����S�[���h�̗�
	int m_subGold;
	//�\������S�[���h�̗�
	int m_showGold;
	//���炵�Ă���ԏ�������h�炷
	int m_shakeGoldPosY;
	//������A�C�e���̉E��ɂ���}�[�N
	int m_canBuyItemMark;
	//�߂�{�^���̃O���t�B�b�N
	int m_backButtonGraph;
};

