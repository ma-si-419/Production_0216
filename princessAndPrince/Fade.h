#pragma once
class Fade
{
public:
	Fade();
	~Fade();
	//更新処理
	void Update();
	//描画処理
	void Draw();
	bool FadeIn();
	bool FadeOut();
private:
	//透明度
	int m_alphaNum;
};

