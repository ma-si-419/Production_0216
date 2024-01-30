#pragma once
#include "Scene.h"
#include "Game.h"
class Player;
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& sceneManager, DataManager& DataManager);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();

	//シーン選択時の演出
	void MoveScene(bool up);
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
	//ショップのアニメーション
	int m_shopAnimFrame;
	//ショップのアニメーションを切り取る座標
	int m_shopSrcX;
	//bgm
	int m_bgm;
	//背景画像を切り取る座標
	int m_cutBgPosY;
	//演出の間動かせないようにする
	bool m_isStaging;
	//シーンが上がっているか下がっているか
	bool m_isSceneUp;
	//ショップのグラフィック
	int m_shopGraph;
	//選んでいるシーンのフレーム
	int m_selectSceneFrame;
	//ステージ名の表記
	int m_backBoxGraph;
	//ステージ変更をしたかどうか
	bool m_isChangeStage;
};
