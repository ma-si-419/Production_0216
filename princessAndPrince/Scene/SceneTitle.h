#pragma once
#include "Scene.h"
class SceneManager;
class SceneTitle : public Scene 
{
public:
	//コンストラクタ
	SceneTitle(SceneManager& sceneManager,DataManager& DataManager,int selectSceneNum);
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
	//bgm
	int m_bgm;
	//背景のグラフィック
	int m_bgGraph;
	//タイトルのグラフィック
	int m_titleGraph;
	//点滅させるときにカウントする
	int m_count;
	//文字を出すかどうか
	bool m_isString;
	//セレクトシーンで選んでいたシーンを保存する
	int m_selectSceneNum;
	//プレイヤーのグラフィック
	int m_playerGraph;
	//エネミーのグラフィック
	int m_enemyGraph;
	//プリンセスのグラフィック
	int m_princessGraph;
	int m_changeWindowModeGraph;
};

