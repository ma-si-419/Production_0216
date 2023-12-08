#pragma once
class Player;
class UI
{
public:
	//コンストラクタ
	UI(Player* pPlayer);
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

};

