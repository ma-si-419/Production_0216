#pragma once
#include "Scene.h"
#include "Game.h"
class Player;
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& sceneManager,DataManager& DataManager);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();
private:
	//今選択しているシーンを保存する
	int m_stageSelectNum;
	//今選択している項目を保存する
	int m_pauseSelectNum;
	//連続でキーが押されてないように
	bool m_isKeyDown;
	//カーソルが連続で移動しないように
	bool m_isSelectKeyDown;
	//ポーズを開いているかどうか
	bool m_isPause;
	//ステータス画面を開いているかどうか
	bool m_isStatus;
	//ステージを決定した後に連打しないように
	bool m_isSelectScene;
	//決定ボタンを押したときの効果音
	int m_appSe;
	//カーソルを動かしたときの効果音
	int m_cursorSe;
	//キャンセルボタンを押したときの効果音
	int m_cancelSe;
	//プレイヤーのグラフィック
	int m_playerGraph;
	//Princessのグラフィック
	int m_princessGraph;
	//背景のグラフィック
	int m_bgGraph;
	//選んでいるシーンの背景
	int m_selectSceneBgGraph;
	//プレイヤーが向いている方向
	Game::Dir m_dir;
	//プレイヤーのアニメーション
	int m_animFrame;
	//bgm
	int m_bgm;
};

