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
	//�V�[���N���A���ɍs��UI�\��
	void SceneClearUI();
	bool IsEnd() { return m_isClearUIEnd; };
	//�S�[���h��\������
	void ShowGold() { m_isShowGold = true; }
	//�{�^������������N���A��\������
	void ShowLeaveButton() { m_isLeaveButton = true; }
private:
	//�v���C���[�̃|�C���^��ۑ�����
	Player* m_pPlayer;
	//�v�����Z�X�̃|�C���^��ۑ�����
	Princess* m_pPrincess;
	//�V�[�����C���̃|�C���^��ۑ�����
	SceneMain* m_pMain;
	//�N���A��UI��\���������ǂ�����\��
	bool m_isClearUIEnd;
	//�\������Ԃ̎��Ԃ��J�E���g����
	int m_timeCount;
	//�S�[���h��\������^�C�~���O
	bool m_isShowGold;
	//�{�^������������N���A���o���^�C�~���O
	bool m_isLeaveButton;
};

