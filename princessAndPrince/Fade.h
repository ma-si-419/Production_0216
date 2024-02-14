#pragma once
enum class fadeState
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
	//�t�F�[�h�̏�Ԃ�Ԃ�
	fadeState GetFadeState() { return m_fadeState; }
	//�t�F�[�h���J�n����
	void StartFadeOut() { m_fadeState = fadeState::kFadeOut; }
	//�t�F�[�h���J�n���Ȃ�
	void StopFade() { m_fadeState = fadeState::kFadeIn; }
private:
	//�����x
	int m_alphaNum;
	//�t�F�[�h�̏�Ԃ��Ǘ�����
	fadeState m_fadeState;
};

