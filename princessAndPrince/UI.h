#pragma once
class Player;
class UI
{
public:
	//�R���X�g���N�^
	UI(Player* pPlayer);
	//�f�X�g���N�^
	~UI();
	//����������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
private:
	//�v���C���[�̃|�C���^��ۑ�����
	Player* m_pPlayer;

};

