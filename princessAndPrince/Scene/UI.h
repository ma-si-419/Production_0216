#pragma once
#include <memory>
class Player;
class Princess;
class SceneMain;
class UI
{
public:
	//コンストラクタ
	UI(Player* pPlayer,Princess* pPrincess,SceneMain* pMain);
	//デストラクタ
	~UI();
	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//シーンクリア時に行うUI表示
	void SceneClearUI();
	//ゲームオーバー時に行うUI表示
	void GameOverUI();
	bool IsEnd() { return m_isClearUIEnd; };
	//ゴールドを表示する
	void ShowGold() { m_isShowGold = true; }
	//ボタンを押したらクリアを表示する
	void ShowLeaveButton() { m_isLeaveButton = true; }
	//数値を右揃えにする
	int AlignmentRight(int num);
	//statusを表示する
	void DrawStatus();
	//獲得したゴールドと経験値を表示する
	void DrawGetItem();
	//魔法のグラフィックを設定する
	void SetMagicGraph(int graph) { m_magicGraph = graph; }
	//魔法のUiを表示する背景を設定する
	void SetMagicBgGraph(int graph) { m_magicUiBgGraph = graph; }
	//怒りゲージを表示するUiのグラフィック
	void SetAngryGaugeGraph(int graph) { m_angryGaugeUiGraph = graph; }
	//怒りゲージがたまった時の画像
	void SetAngryButtonGraph(int graph) { m_angryButtonGraph = graph; }
	//怒りゲージが使えないときのグラフィック
	void SetStoneAngryGaugeGraph(int graph) { m_stoneAngryGaugeGraph = graph; }
private:
	//プレイヤーのポインタを保存する
	Player* m_pPlayer;
	//プリンセスのポインタを保存する
	Princess* m_pPrincess;
	//シーンメインのポインタを保存する
	SceneMain* m_pMain;
	//クリアのUIを表示したかどうかを表す
	bool m_isClearUIEnd;
	//表示する間の時間をカウントする
	int m_timeCount;
	//ゴールドを表示するタイミング
	bool m_isShowGold;
	//ボタンを押したらクリアを出すタイミング
	bool m_isLeaveButton;
	//ボタンのグラフィック
	int m_buttonsGraph;
	//魔法のグラフィック
	int m_magicGraph;
	//魔法のUiを表示する背景のグラフィック
	int m_magicUiBgGraph;
	//怒りゲージを表示するUIのグラフィック
	int m_angryGaugeUiGraph;
	//怒りゲージが使えないときに表示するグラフィック
	int m_stoneAngryGaugeGraph;
	//怒りゲージを揺らす時に使う
	int m_angryGaugeUiShiftPosX;
	bool m_isAngryGaugeUiShake;
	//怒りゲージがたまった時のボタン表示
	int m_angryButtonGraph;
	//怒りゲージがたまった時のボタンを拡縮する値
	double m_angryButtonRatio;
	//今ボタンが拡大しているかどうか
	bool m_isButtonZoom;
};

