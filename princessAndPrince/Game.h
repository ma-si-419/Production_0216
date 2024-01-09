#pragma once

// ゲーム全体で使用する定数を定義する
namespace Game
{
	constexpr int kPlayScreenWidth = 960;	// プレイ画面の横幅
	constexpr int kScreenWidth = 1600;		// 全画面の横幅
	constexpr int kPlayScreenHeight = 960;	// 画面の縦幅

	constexpr int kColorDepth = 32;		// 16 or 32

	//基本的なキャラクターの円の大きさ
	constexpr int kRadius = 48;
	/*向いている方向を保持するenum*/
	enum Dir
	{
		kDirDown,	 // 下
		kDirLeftDown,// 左下
		kDirLeft,	 // 左
		kDirLeftUp,  // 左上
		kDirUp,		 // 上
		kDirRightUp, // 右上
		kDirRight,	 // 右
		kDirRightDown,// 右下
		kDirDeath//死んでいる状態
	};
	/*アイテムの種類を保持するenum*/
	enum ItemKinds
	{
		kEmpty,//初期化用
		kExp,//経験値
		kGold,//お金
		kBlood,//魔女を動かす血
		kPortion,//回復アイテム
	};
	/*状態を管理するenum*/
	enum State
	{
		kNormal,//基本的な状態
		kHitPlayer,//ぶつかった状態(めり込んでぶつかり、処理が複数行われないため)
		kHitMagic,//魔法にぶつかったときの状態
		kDelete,//死んでいる状態
		kNone,//何もない状態
	};
}