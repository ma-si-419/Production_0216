#pragma once
#include "Vec2.h"

/// <summary>
/// 円形を管理するクラス
/// </summary>
class CircleCol
{
public:
	CircleCol();
	virtual ~CircleCol();

	// 描画
	void Draw(int radius, unsigned int color, bool isFill) const;

	/// <summary>
	/// 中心座標と半径を保存
	/// </summary>
	/// <param name="pos">中心座標: x</param>
	/// <param name="radius">半径</param>
	void SetCenter(Vec2 pos, float radius);

	Vec2 GetCenter() const;		// 円形の中心座標

	/// <summary>
	/// 円形同士の当たり判定
	/// </summary>
	/// <param name="target">判定側</param>
	/// <returns>true : 当たった, false : 当たってない</returns>
	bool IsCollsion(const CircleCol& target);
	
	float GetRange(Vec2 pos, Vec2 targetPos);
private:
	//円形の中央の座標
	Vec2 m_pos;
	//円の半径
	int m_radius;
};
