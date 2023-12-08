#pragma once
#include "Game.h"
#include "CircleCol.h"
#include "Vec2.h"

class Character
{
public:
	//コンストラクタ
	Character();
	//デストラクタ
	virtual ~Character();
	//初期化処理
	virtual void Init();
	//更新処理
	virtual void Update();
	//描画処理
	virtual void Draw();
	//メンバ変数にアクセスする
	virtual void SetHandle(int handle) { m_handle = handle; }
	//現在位置を取得する
	virtual Vec2 GetPos()const { return m_pos; }
	//攻撃力を取得する
	virtual int GetAtk() { return m_atk; }
	//当たり判定を取得する
	virtual CircleCol GetColCircle() { return m_circleCol; }
protected:
	/*メンバ変数*/
	//攻撃力
	int m_atk;
	//体力
	float m_hp;
	//速度
	float m_spd;
	//防御力
	int m_def;
	//グラフィックハンドル
	int m_handle;
	//当たり判定の大きさ
	int m_radius;
	//当たり判定
	CircleCol m_circleCol;
	//位置情報
	Vec2 m_pos;
	//ノックバック
	Vec2 m_knockBack;
	//ノックバックしている時間
	int m_knockBackTime;
	//アニメーション
	int m_animFrame;
	//向いている方向を設定するときに必要になる変数
	int m_dirX;
	//向いている方向を設定するときに必要になる変数
	int m_dirY;


	//自分が向いている方向
	Game::Dir m_dir;

	/*メンバ関数*/
	void GetDir(int x, int y)
	{

		if (x == -1 && y == -1)
		{
			m_dir = Game::kDirLeftUp;
		}
		else if (x == 0 && y == -1)
		{
			m_dir = Game::kDirUp;
		}
		else if (x == 1 && y == -1)
		{
			m_dir = Game::kDirRightUp;
		}
		else if (x == 1 && y == 0)
		{
			m_dir = Game::kDirRight;
		}
		else if (x == 1 && y == 1)
		{
			m_dir = Game::kDirRightDown;
		}
		else if (x == 0 && y == 1)
		{
			m_dir = Game::kDirDown;
		}
		else if (x == -1 && y == 1)
		{
			m_dir = Game::kDirLeftDown;
		}
		else if (x == -1 && y == 0)
		{
			m_dir = Game::kDirLeft;
		}
	}
};