#include "DataManager.h"
#include "DxLib.h"

DataManager::DataManager()
{
	//カーソルを動かす時の効果音
	soundMap["approveSe"] = LoadSoundMem("data/sound/approveSe.mp3");
	//カーソルを動かす時の効果音
	soundMap["cancelSe"] = LoadSoundMem("data/sound/cancelSe.mp3");
	//カーソルを動かす時の効果音
	soundMap["cursorSe"] = LoadSoundMem("data/sound/cursorSe.mp3");
	//敵にぶつかった時の音
	soundMap["attackSe"] = LoadSoundMem("data/sound/attack3.mp3");
	//ダンスするときの効果音
	soundMap["danceSe"] = LoadSoundMem("data/sound/clearSe.mp3");
	//リザルト画面のゴールドの効果音
	soundMap["resultGoldSe"] = LoadSoundMem("data/sound/gold1.mp3");
	//リザルト画面の経験値の効果音
	soundMap["resultExpSe"] = LoadSoundMem("data/sound/exp1.mp3");
	//フィールドのBgm
	soundMap["fieldBgm"] = LoadSoundMem("data/sound/mainBgm2.mp3");
	//ボスが出てきた時のBgm
	soundMap["bossBgm"] = LoadSoundMem("data/sound/mainBgm1.mp3");
	//ダンスの前の敵がいなくなる時の音
	soundMap["beforeDanceSe"] = LoadSoundMem("data/sound/enemyDelete.mp3");
	//プレイヤーのグラフィック
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//プリンセスのグラフィック
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//エネミーのグラフィック
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
	//背景のグラフィック
	graphMap["bgGraph"] = LoadGraph("data/image/_bg.png");

}

DataManager::~DataManager()
{
}
