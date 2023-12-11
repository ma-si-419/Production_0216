#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
class SceneMain;
class TreasureBox
{
public:
	//�R���X�g���N�^
	TreasureBox(SceneMain* sceneMain);
	//�f�X�g���N�^
	~TreasureBox();
	//����������
	void Init(Vec2 pos);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�󔠂̗̑͂̌��炷
	void SubHp() { m_hp--; }
	//�󔠂̗̑͂����炷
	int GetHp() { return m_hp; }
	//�󔠂̒��g���o��
	void OpenTreasure();
	//��Ԃ��Ǘ�����
	Game::State m_nowState;
private:
	//SceneMain�̃|�C���^
	SceneMain* m_pMain;
	//�\���ʒu
	Vec2 m_pos;
	//�O���t�B�b�N�n���h��
	int m_handle;
	//�����蔻��
	CircleCol m_circleCol;
	//���݂��Ă��邩�ǂ���
	bool m_isExist;
	//�󔠂̗̑�
	int m_hp;
	//�����蔻��̑傫��
	int m_colScale;
};

