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
	//�Q�[���I�[�o�[���ɍs��UI�\��
	void GameOverUI();
	bool IsEnd() { return m_isClearUIEnd; };
	//�S�[���h��\������
	void ShowGold() { m_isShowGold = true; }
	//�{�^������������N���A��\������
	void ShowLeaveButton() { m_isLeaveButton = true; }
	//���l���E�����ɂ���
	int AlignmentRight(int num);
	//status��\������
	void DrawStatus();
	//�l�������S�[���h�ƌo���l��\������
	void DrawGetItem();
	//���@�̃O���t�B�b�N��ݒ肷��
	void SetMagicGraph(int graph) { m_magicGraph = graph; }
	//���@��Ui��\������w�i��ݒ肷��
	void SetMagicBgGraph(int graph) { m_magicUiBgGraph = graph; }
	//�{��Q�[�W��\������Ui�̃O���t�B�b�N
	void SetAngryGaugeGraph(int graph) { m_angryGaugeUiGraph = graph; }
	//�{��Q�[�W�����܂������̉摜
	void SetAngryButtonGraph(int graph) { m_angryButtonGraph = graph; }
	//�{��Q�[�W���g���Ȃ��Ƃ��̃O���t�B�b�N
	void SetStoneAngryGaugeGraph(int graph) { m_stoneAngryGaugeGraph = graph; }
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
	//�{�^���̃O���t�B�b�N
	int m_buttonsGraph;
	//���@�̃O���t�B�b�N
	int m_magicGraph;
	//���@��Ui��\������w�i�̃O���t�B�b�N
	int m_magicUiBgGraph;
	//�{��Q�[�W��\������UI�̃O���t�B�b�N
	int m_angryGaugeUiGraph;
	//�{��Q�[�W���g���Ȃ��Ƃ��ɕ\������O���t�B�b�N
	int m_stoneAngryGaugeGraph;
	//�{��Q�[�W��h�炷���Ɏg��
	int m_angryGaugeUiShiftPosX;
	bool m_isAngryGaugeUiShake;
	//�{��Q�[�W�����܂������̃{�^���\��
	int m_angryButtonGraph;
	//�{��Q�[�W�����܂������̃{�^�����g�k����l
	double m_angryButtonRatio;
	//���{�^�����g�債�Ă��邩�ǂ���
	bool m_isButtonZoom;
};

