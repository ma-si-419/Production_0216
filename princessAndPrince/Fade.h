#pragma once
enum class fadeState
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
	//フェードの状態を返す
	fadeState GetFadeState() { return m_fadeState; }
	//フェードを開始する
	void StartFadeOut() { m_fadeState = fadeState::kFadeOut; }
	//フェードを開始しない
	void StopFade() { m_fadeState = fadeState::kFadeIn; }
private:
	//透明度
	int m_alphaNum;
	//フェードの状態を管理する
	fadeState m_fadeState;
};

