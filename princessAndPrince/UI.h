#pragma once
#include <memory>
class Player;
class Princess;
class SceneMain;
class UI
{
public:
	//�R���X�g���N�^
	UI(Player* pPlayer,Princess* pPrincess,SceneMain* pMain);
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
	//�v�����Z�X�̃|�C���^��ۑ�����
	Princess* m_pPrincess;
	//�V�[�����C���̃|�C���^��ۑ�����
	SceneMain* m_pMain;
};

