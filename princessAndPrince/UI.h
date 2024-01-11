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
	bool IsEnd() { return m_isClearUIEnd; };
	//ゴールドを表示する
	void ShowGold() { m_isShowGold = true; }
	//ボタンを押したらクリアを表示する
	void ShowLeaveButton() { m_isLeaveButton = true; }
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
};

