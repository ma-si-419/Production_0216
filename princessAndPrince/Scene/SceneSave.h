#pragma once
#include "Scene.h"
#include "DxLib.h"
class SceneSave : public Scene
{
private:
	enum class dataKind
	{
		main,
		atk,
		def,
		spd,
		fire,
		wind,
		gold,
		exp,
		stage,
		dataNum
	};

public:
	SceneSave(SceneManager& sceneManager, DataManager& DataManager, int selectSceneNum);
	virtual ~SceneSave();

	void Init();
	void Update(Pad& pad);
	void Draw();

	//ロードしたデータによって表示する数値を変える
	int GetDrawData(int selectNum, dataKind kind);
	//数値を右揃えにする
	int AlignmentRight(int num);
	//セーブデータのフレームを表示する
	void DrawFrame(int selectNum);
	//セーブデータをセットする
	void SetSaveData(int selectNum);
	//セーブデータのナンバーを表示する
	void DrawSaveDataNum();
	//ステータスを表示する
	void DrawStatus();
	//ファイルをロードする
	void LoadFile();
	//ボタンが入力されているかを返す
	bool GetPushButton(XINPUT_STATE& input);
	//フレームの拡縮をする
	void ScalingFrame();
	//カーソルを動かす
	void MoveCursor(XINPUT_STATE& input);
	//セーブデータをけす
	void DeleteSaveData(int dataNum);

private:
	
	//背景のグラフィック
	int m_bgGraph;
	//フレームのグラフィック
	int m_frameGraph;
	//エネミーの顔画像
	int m_enemyHeadGraph;
	//カーソルの画像
	int m_cursorGraph;
	//カーソルを動かしたときの音
	int m_cursorSe;
	//決定した時の音
	int m_appSe;
	//キャンセルした時の音
	int m_cancelSe;
	//前のフレームにキーを押していたか
	bool m_isLastKey;
	//今どのセーブデータを選択しているか
	int m_selectSaveNum;
	//セーブデータのウィンドウを表示しているか
	bool m_isShowSaveWindow;
	//データ削除のウィンドウを出しているかどうか
	bool m_isShowDeleteWindow;
	//ウィンドウのどの項目を選択しているか
	int m_selectWindowNum;
	//データ削除の選択肢
	int m_selectDeleteNum;
	//ロードしたデータ(一つ目)
	int m_firstLoadData[10];
	//ロードしたデータ(二つ目)
	int m_secondLoadData[10];
	//ロードしたデータ(三つ目)
	int m_thirdLoadData[10];
	//ロードしたデータ(四つ目)
	int m_fourthLoadData[10];
	//フレーム拡縮の大きさ
	float m_frameRatioX;
	float m_frameRatioY;
	//フレームが拡大しているかどうか
	bool m_isZoom;
	//BGM
	int m_bgm;
	//シーン移動するかどうか
	bool m_isMoveScene;
	//カーソルの揺らす長さ
	float m_cursorShakeLange;
	//カーソルが右に動いているかどうか
	bool m_isCursorMoveRight;
};

