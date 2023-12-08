#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
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
	//�擾�����A�C�e���������֐�
	void DeleteItem() { m_isExist = false; }

protected:
	//�\���ʒu
	Vec2 m_pos;
	//�O���t�B�b�N�n���h��
	int m_handle;
	//�����蔻��
	CircleCol m_circleCol;
	//�A�C�e���̎��
	Game::ItemKinds m_kind;
	//���݂��Ă��邩�ǂ���
	bool m_isExist;
	//�����蔻��̑傫��
	int m_colScale;

};

