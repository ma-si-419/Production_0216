#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
class Player;
class ItemBase
{
public:
	//�R���X�g���N�^
	ItemBase();
	//�f�X�g���N�^
	virtual ~ItemBase();
	//����������
	virtual void Init(Vec2 pos);
	//�X�V����
	virtual void Update();
	//�`�揈��
	virtual void Draw();

	//���݂��Ă��邩�ǂ������f����
	bool IsExist() { return m_isExist; }
	//�����蔻����擾����
	virtual CircleCol GetColCircle() { return m_circleCol; }
	//�����̃A�C�e���̎�ނ��擾����
	Game::ItemKinds GetKind() { return m_kind; }
	//��Ԃ��Ǘ�����enum
	Game::State m_nowState;
	//�����̗ʂ��擾����
	int GetPrice() { return m_haveGold; }
	//�o���l�̗ʂ��擾����
	int GetExp() { return m_haveExp; }
	//�����̗ʂ�ݒ肷��
	void SetPrice(int gold) { m_haveGold = gold; }
	//�o���l�̗ʂ�ݒ肷��
	void SetExp(int exp) { m_haveExp = exp; }
	//�O���t�B�b�N�̃n���h����ݒ肷��
	void SetHandle(int handle) { m_handle = handle; }
	//�����ƌo���l���擾�������͉�ʉE��܂ňړ�������
	void MoveItem(Player* player);
	//�{�X�̂����ƌo���l�̓O���t�B�b�N�������ς���
	void SetBossItem() { m_srcX = 64; };

protected:
	//�\���ʒu
	Vec2 m_pos;
	//�����x�N�g��
	Vec2 m_moveVec;
	//�v���C���[�̃|�C���^
	Player* m_pPlayer;
	//�O���t�B�b�N�n���h��
	int m_handle;
	//�����蔻��
	CircleCol m_circleCol;
	//�A�C�e���̎��
	Game::ItemKinds m_kind;
	//���݂��Ă��邩�ǂ���
	bool m_isExist;
	//�����蔻��̑傫��
	float m_colScale;
	//�����Ă��邨���̗�
	int m_haveGold;
	//�����Ă���o���l�̗�
	int m_haveExp;
	//�O���t�B�b�N��؂�����W
	int m_srcX;

};

