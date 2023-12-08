#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
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
	//取得したアイテムを消す関数
	void DeleteItem() { m_isExist = false; }

protected:
	//表示位置
	Vec2 m_pos;
	//グラフィックハンドル
	int m_handle;
	//当たり判定
	CircleCol m_circleCol;
	//アイテムの種類
	Game::ItemKinds m_kind;
	//存在しているかどうか
	bool m_isExist;
	//当たり判定の大きさ
	int m_colScale;

};

