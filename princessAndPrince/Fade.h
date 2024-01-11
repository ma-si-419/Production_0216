#pragma once
class Fade
{
public:
	Fade();
	~Fade();
	//XVˆ—
	void Update();
	//•`‰æˆ—
	void Draw();
	bool FadeIn();
	bool FadeOut();
private:
	//“§–¾“x
	int m_alphaNum;
};

