#pragma once
#include "Scene.h"
class SceneSave : public Scene
{
private:
	enum dataKind
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

	//ロードしたデータによって表示する文字を変える
	void DrawSceneName(int sceneNum);
	//ロードしたデータによって表示する数値を変える
	int GetDrawData(int selectNum, dataKind kind);
	//数値を右揃えにする
	int AlignmentRight(int num);
	//セーブデータのフレームを表示する
	void DrawFrame(int selectNum);
	//セーブデータをセットする
	void SetSaveData(int selectNum);
	

private:
	
	//背景のグラフィック
	int m_bgGraph;
	//フレームのグラフィック
	int m_frameGraph;
	//前のフレームにキーを押していたか
	bool m_isLastKey;
	//今どのセーブデータを選択しているか
	int m_selectSaveNum;
	//セーブデータのウィンドウを表示しているか
	bool m_isShowSaveWindow;
	//ウィンドウのどの項目を選択しているか
	int m_selectWindowNum;
	//データ削除を選んだかどうか
	bool m_isDeleteData;
	//データ削除の選択肢
	int m_selectDeleteNum;
	//ロードしたデータ(一つ目)
	int m_firstLoadData[10];
	//ロードしたデータ(二つ目)
	int m_secondLoadData[10];
	//ロードしたデータ(三つ目)
	int m_thirdLoadData[10];
	//フレーム拡縮の大きさ
	float m_frameRatioX;
	float m_frameRatioY;
	//フレームが拡大しているかどうか
	bool m_isZoom;

};

