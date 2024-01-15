#pragma once
enum fadeState
{
	kFadeIn,//フェードインする状態
	kNormal,
	kFadeOut//フェードアウトする状態
};
class Fade
{
public:
	Fade();
	~Fade();
	//更新処理
	void Update();
	//描画処理
	void Draw();
	void FadeIn();
	void FadeOut();
	//フェードの状態を返す
	bool GetFadeState() { return m_fadeState; }
	//フェードを開始する
	void StartFadeOut() { m_fadeState = fadeState::kFadeOut; }
private:
	//透明度
	int m_alphaNum;
	//フェードの状態を管理する
	fadeState m_fadeState;
};

