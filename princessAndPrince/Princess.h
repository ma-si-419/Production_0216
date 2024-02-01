#pragma once
#include"Vec2.h"
#include"CircleCol.h"
#include"Game.h"
#include"Character.h"
class Enemy;
class SceneMain;
class MagicBase;
class Particle;
class Princess : public Character
{
public:
	Princess(SceneMain* pMain);
	virtual ~Princess();

	void Init();
	void Update();
	void Draw() const;
	//ダメージを食らったときの処理
	void HitEnemy(Enemy& enemy);
	//現在のHpの割合計算する関数
	float GetHpRate() { return m_nowHp / m_hp; }
	//現在持っている血の量の割合を計算する
	float GetBloodRate() { return m_nowBlood / m_maxBlood; }
	//持っている血の量を取得する
	float GetBlood() { return m_nowBlood; }
	//魔法のハンドル設定する
	void SetMagicHandle(int handle) { m_magicHandle = handle; }
	//Playerから血を受け取るときに使う
	void ReceiveBlood(float blood);
	//体力が0かどうか判断する関数
	bool IsDeath();
	//今魔法を打っているかを取得する
	bool IsMagic() { return m_isMagic; }
	//魔法を打っている角度を取得する
	Vec2 GetMagicDir() { return m_magicVec; }
	//魔法の種類を取得する
	bool GetMagicKind() { return m_isFire; }
	//攻撃力を取得する
	float GetAtk() { return m_atk; }
	//石の状態にする
	void TransStone() {m_drawState = Game::kStone; }
	//聖剣モードを始めるときに呼ぶ
	void StartSpecialMode() { m_drawState = Game::kMagic; m_animFrame = 0; }
private:
	//メインシーンのポインタを保存する変数
	SceneMain* m_pMain;
	//パーティクルのポインタ
	Particle* m_pParticle;
	//Hpバーの位置
	Vec2 m_hpBarPos;
	//血の量バーのいち
	Vec2 m_bloodBarPos;
	//魔法のポインタを保存する変数
	MagicBase* m_pMagic;
	//現在持っている血の量
	float m_nowBlood;
	//血の量の最大値
	float m_maxBlood;
	//血の量バーの長さ
	float m_bloodBarWidth;
	//Hpバーの長さ
	float m_hpBarWidth;
	//現在のHPの量
	float m_nowHp;
	//今魔法を打っているか
	bool m_isMagic;
	//今どっちの魔法を打っているか判断する変数
	bool m_isFire;
	//前のフレーム時キーを押していたか保存する変数
	bool m_isLastKeyFlag;
	//計算結果を保存する変数
	Vec2 m_result;
	//向いている方向の角度
	float m_angle;
	//魔法を打つ間隔をカウントする変数
	int m_MagicCount;
	//魔法を撃つ線の終点を保存する変数
	Vec2 m_magicVec;
	//魔法の大きさ
	float m_scale;
	//敵にぶつかって揺れる時間をカウントする
	int m_shakeTimeCount;
	//ぶつかった時に立つフラグ
	bool m_hitFlag;
	//基本的な魔女の位置
	Vec2 m_basePos;
	//揺れるスピード
	float m_shakeSpeed;
	//エネミーとの衝突点
	Vec2 m_hitPos;
	//魔女の描画する状態を表す
	Game::WitchState m_drawState;
	//魔法のグラフィックハンドル
	int m_magicHandle;
	//風魔法の効果音
	int m_windMagicSe;
	//プレイヤーから血を受け取った時にならす効果音
	int m_passBloodSe;
	//炎魔法の効果音
	int m_fireMagicSe;
};

