#pragma once
#include "Scene.h"
class SceneManager;
class SceneTitle : public Scene 
{
public:
	//コンストラクタ
	SceneTitle(SceneManager& sceneManager,DataManager& DataManager);
	//デストラクタ
	virtual ~SceneTitle();
	//初期化処理
	void Init();
	//更新処理
	void Update(Pad& pad);
	//描画処理
	void Draw();
private:
	//タイトル画像
	int m_handle;
	//連続でキーが押されないようにするための変数
	bool m_isKeyDown;
	//ボタンを押したときの効果音
	int m_appSe;
};

