#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
class Player;
class ItemBase
{
public:
	//コンストラクタ
	ItemBase();
	//デストラクタ
	virtual ~ItemBase();
	//初期化処理
	virtual void Init(Vec2 pos);
	//更新処理
	virtual void Update();
	//描画処理
	virtual void Draw();

	//存在しているかどうか判断する
	bool IsExist() { return m_isExist; }
	//当たり判定を取得する
	virtual CircleCol GetColCircle() { return m_circleCol; }
	//自分のアイテムの種類を取得する
	Game::ItemKinds GetKind() { return m_kind; }
	//状態を管理するenum
	Game::State m_nowState;
	//お金の量を取得する
	int GetPrice() { return m_haveGold; }
	//経験値の量を取得する
	int GetExp() { return m_haveExp; }
	//お金の量を設定する
	void SetPrice(int gold) { m_haveGold = gold; }
	//経験値の量を設定する
	void SetExp(int exp) { m_haveExp = exp; }
	//グラフィックのハンドルを設定する
	void SetHandle(int handle) { m_handle = handle; }
	//お金と経験値を取得した時は画面右上まで移動させる
	void MoveItem(Player* player);
	//ボスのお金と経験値はグラフィックを少し変える
	void SetBossItem() { m_srcX = 64; };

protected:
	//表示位置
	Vec2 m_pos;
	//動くベクトル
	Vec2 m_moveVec;
	//プレイヤーのポインタ
	Player* m_pPlayer;
	//グラフィックハンドル
	int m_handle;
	//当たり判定
	CircleCol m_circleCol;
	//アイテムの種類
	Game::ItemKinds m_kind;
	//存在しているかどうか
	bool m_isExist;
	//当たり判定の大きさ
	float m_colScale;
	//持っているお金の量
	int m_haveGold;
	//持っている経験値の量
	int m_haveExp;
	//グラフィックを切り取る座標
	int m_srcX;

};

