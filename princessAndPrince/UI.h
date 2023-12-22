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
private:
	//プレイヤーのポインタを保存する
	Player* m_pPlayer;
	//プリンセスのポインタを保存する
	Princess* m_pPrincess;
	//シーンメインのポインタを保存する
	SceneMain* m_pMain;
};

