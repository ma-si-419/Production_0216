#pragma once
#include "CircleCol.h"
#include "Game.h"
#include "Vec2.h"
class Princess;
class MagicBase
{
public:
	//コンストラクタ
	MagicBase(Princess* pPrincess,float scale);
	//デストラクタ
	virtual ~MagicBase();
	//初期化処理
	void Init(int magicNum);
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//当たり判定を取得する
	CircleCol GetCircleCol() { return m_circleCol; }
	//ハンドルを取得する
	void SetHandle(int handle) { m_handle = handle; }
	//攻撃力を取得する
	float GetAtk();
	//状態を管理する
	Game::State m_nowState;
	//魔法の種類を取得する
	bool GetMagicKind() { return m_isFire; }
private:
	//プリンセスのポインタを保存する変数
	Princess* m_pPrincess;
	//魔女のポジション
	Vec2 m_princessPos;
	//魔法のポジション
	Vec2 m_magicPos;
	//魔法の向かうベクトル
	Vec2 m_moveVec;
	//円形の当たり判定
	CircleCol m_circleCol;
	//炎魔法の攻撃力
	float m_fireAtk;
	//風魔法の攻撃力
	float m_windAtk;
	//魔法の大きさ
	float m_scale;
	//魔法の種類
	bool m_isFire;
	//魔法の速度
	float m_spd;
	//風魔法を出す時に使う変数
	float m_windAngle;
	float m_windLength;
	//プレイヤーのどちらに生成される風か判断する
	bool m_isRightWind;
	//グラフィック
	int m_handle;
	//画像を反転させるかどうか
	bool m_turnFlag;
	//画像の角度
	float m_graphAngle;
};

