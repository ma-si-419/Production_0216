#pragma once
class Fade
{
public:
	Fade();
	~Fade();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	bool FadeIn();
	bool FadeOut();
private:
	//�����x
	int m_alphaNum;
};

