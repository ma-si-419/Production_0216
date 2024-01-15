#pragma once
enum fadeState
{
	kFadeIn,//�t�F�[�h�C��������
	kNormal,
	kFadeOut//�t�F�[�h�A�E�g������
};
class Fade
{
public:
	Fade();
	~Fade();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	void FadeIn();
	void FadeOut();
	//�t�F�[�h�̏�Ԃ�Ԃ�
	bool GetFadeState() { return m_fadeState; }
	//�t�F�[�h���J�n����
	void StartFadeOut() { m_fadeState = fadeState::kFadeOut; }
private:
	//�����x
	int m_alphaNum;
	//�t�F�[�h�̏�Ԃ��Ǘ�����
	fadeState m_fadeState;
};

