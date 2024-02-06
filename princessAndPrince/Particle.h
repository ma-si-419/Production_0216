#pragma once
#include "Vec2.h"
class Particle
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">表示位置</param>
	/// <param name="lange">どれ位移動したら消えるか</param>
	/// <param naem="speed">移動速度</param>
	/// <param naem="speed">大きさ</param>
	/// <param name="color">色の幅 0.白1.赤2.虹色</param>
	Particle(Vec2 pos,float lange,float speed,int scale,int color);
	//デストラクタ
	~Particle();
	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//存在しているかどうかを返す
	bool GetIsExist() { return m_isExist; }

private:
	//初期位置
	Vec2 m_initialPos;
	//表示位置
	Vec2 m_pos;
	//移動量
	Vec2 m_dirVec;
	//大きさ
	int m_scale;
	//表示する半径
	float m_appearLange;
	//存在しているかどうかフラグ
	bool m_isExist;
	//色
	int m_color;
	//移動速度
	float m_spd;
	//白いパーティクルかどうか
	bool m_isWhite;
};

