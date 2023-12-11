#pragma once
#include"CircleCol.h"
#include"Game.h"
#include"Vec2.h"
class SceneMain;
class TreasureBox
{
public:
	//コンストラクタ
	TreasureBox(SceneMain* sceneMain);
	//デストラクタ
	~TreasureBox();
	//初期化処理
	void Init(Vec2 pos);
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//宝箱の体力の減らす
	void SubHp() { m_hp--; }
	//宝箱の体力を減らす
	int GetHp() { return m_hp; }
	//宝箱の中身を出す
	void OpenTreasure();
	//状態を管理する
	Game::State m_nowState;
private:
	//SceneMainのポインタ
	SceneMain* m_pMain;
	//表示位置
	Vec2 m_pos;
	//グラフィックハンドル
	int m_handle;
	//当たり判定
	CircleCol m_circleCol;
	//存在しているかどうか
	bool m_isExist;
	//宝箱の体力
	int m_hp;
	//当たり判定の大きさ
	int m_colScale;
};

