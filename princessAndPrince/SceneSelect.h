#pragma once
#pragma once
#include "Scene.h"
#include "Game.h"
class Player;
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& sceneManager, DataManager& DataManager,int selectSceneNum);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();

	//シーン選択時の演出
	void MoveScene(bool up);
	//シーンを選んでいる時に出てくる三角形
	void DrawSceneSrideTriangle();
	//表示するボスを決める
	int GetShowBossKind(int num);
	//変えるアイテムがあるかどうか
	bool GetCanBuyItem();
	//セーブシーンに戻るときにセーブする
	void SetSaveData(int num);
	//ステージの名前を表示する
	void DrawStageName(int num);
	//ショップを拡大する
	void ZoomShop();
	//シーンとキャラを動かす
	void MoveCharcter();
	//ビックリマークを動かす
	void ZoomMark();
private:
	//今選択しているシーンを保存する
	int m_stageSelectNum;
	//連続でキーが押されてないように
	bool m_isKeyDown;
	//カーソルが連続で移動しないように
	bool m_isSelectKeyDown;
	//ステージを決定した後に連打しないように
	bool m_isSelectScene;
	//決定ボタンを押したときの効果音
	int m_appSe;
	//メインシーンに移動するときの音
	int m_moveMainSceneSe;
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
	int m_shopSrcY;
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
	//ボタンのUI
	int m_buttonsUiGraph;
	//ショップボタンを押したかどうか
	bool m_isShopButton;
	//ショップの表示座標(始点と終点)
	float m_shopStartPosX;
	float m_shopStartPosY;
	int m_shopEndPosX;
	int m_shopEndPosY;
	//メインシーンに移動するかどうか
	bool m_isMoveMainScene;
	//ストーリーの画像が入っている配列
	int m_storyGraph[8];
	//どのステージまでクリアしたか
	int m_clearStageNum;
	//プレイヤーとプリンセスの座標
	int m_charPosY;
	//プレイヤーとプリンセスがステージの上まで行ったかどうか
	bool m_isCharArrTopEnd;
	//プレイヤーとプリンセスがステージの下まで行ったかどうか
	bool m_isCharArrBottomEnd;
	//プレイヤーとプリンセスが下に動いているかどうか
	bool m_isCharMoveDown;
	//各ステージのボスのグラフィック
	int m_enemyGraph;
	//表示するボス
	int m_bossSrcX;
	int m_bossSrcY;
	//ボスのアニメーション
	int m_bossAnimFrame;
	//ボスを出現させるときフェードインで出現させる
	int m_bossAlpha;
	//ビックリマーク
	int m_exclamationMarkGraph;
	//黒いボックス
	int m_boxGraph;
	//黒いボックスの拡大率
	float m_boxRatio;
	//黒いボックスの角度
	float m_boxAngle;
	//プレイヤーのアイテムの値段
	int m_playerItemPriceList[21];
	//プリンセスのアイテムの値段
	int m_princessItemPriceList[21];
	//変えるアイテムがあるかどうか
	bool m_isBuy;
	//ビックリマークのポジション
	float m_exclamationMarkRatio;
	//ビックリマークを動かす速さ
	float m_exclamationMarkZoomSpeed;
	//三角形を動かす座標
	float m_shakeTrianglePosY;
	//三角形を上に動かしているかどうか
	bool m_isMoveUpTriangle;
};
