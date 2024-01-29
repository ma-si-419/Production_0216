#include "DataManager.h"
#include "DxLib.h"

DataManager::DataManager()
{
	


}

DataManager::~DataManager()
{
}

void DataManager::Init()
{
	//カーソルを動かす時の効果音
	soundMap["approveSe"] = LoadSoundMem("data/sound/approveSe.mp3");
	//カーソルを動かす時の効果音
	soundMap["cancelSe"] = LoadSoundMem("data/sound/cancelSe.mp3");
	//カーソルを動かす時の効果音
	soundMap["cursorSe"] = LoadSoundMem("data/sound/cursorSe.mp3");
	//ショップでアイテムを買った時の効果音
	soundMap["shopSe"] = LoadSoundMem("data/sound/shopSe.mp3");
	//聖剣モードを発動した時の効果音の効果音
	soundMap["specialModeSe"] = LoadSoundMem("data/sound/specialModeSe.mp3");
	//敵にぶつかった時の音
	soundMap["attackSe"] = LoadSoundMem("data/sound/attack3.mp3");
	//ダンスするときの効果音
	soundMap["danceSe"] = LoadSoundMem("data/sound/clearSe.mp3");
	//リザルト画面のゴールドの効果音
	soundMap["resultGoldSe"] = LoadSoundMem("data/sound/gold1.mp3");
	//リザルト画面の経験値の効果音
	soundMap["resultExpSe"] = LoadSoundMem("data/sound/exp1.mp3");
	//フィールドのBgm
	soundMap["fieldBgm"] = LoadSoundMem("data/sound/fieldBgm.wav");
	//ボスが出てきた時のBgm
	soundMap["bossBgm"] = LoadSoundMem("data/sound/bossBgm.wav");
	//ダンスの前の敵がいなくなる時の音
	soundMap["beforeDanceSe"] = LoadSoundMem("data/sound/enemyDelete.mp3");
	//コインを拾った時の音
	soundMap["coinSe"] = LoadSoundMem("data/sound/coinSe.mp3");
	//経験値を拾った時の音
	soundMap["expSe"] = LoadSoundMem("data/sound/expSe.mp3");
	//ショップでアイテムを買った時の音
	soundMap["buySe"] = LoadSoundMem("data/sound/buySe.mp3");
	//ショップでアイテムを買えなかった時の音
	soundMap["missBuySe"] = LoadSoundMem("data/sound/missBuySe.mp3");
	//聖剣モードを始めるときに鳴らす効果音
	soundMap["specialModeSe"] = LoadSoundMem("data/sound/specialModeSe.mp3");
	//風魔法の効果音
	soundMap["windMagicSe"] = LoadSoundMem("data/sound/windMagic.mp3");
	//ポーションを拾った時の効果音
	soundMap["portionSe"] = LoadSoundMem("data/sound/portionSe.mp3");
	//弱点じゃないところにぶつかった時の効果音
	soundMap["hitSe"] = LoadSoundMem("data/sound/hitSe.mp3");
	//血を拾った時の効果音
	soundMap["bloodSe"] = LoadSoundMem("data/sound/bloodSe1.mp3");
	//血を渡したときの効果音
	soundMap["passBloodSe"] = LoadSoundMem("data/sound/passBloodSe.mp3");
	//ショップのBGM
	soundMap["shopBgm"] = LoadSoundMem("data/sound/shopBgm.wav");
	//セレクトシーンのBgm
	soundMap["selectBgm"] = LoadSoundMem("data/sound/selectBgm.wav");
	//タイトルシーンのBgm
	soundMap["titleBgm"] = LoadSoundMem("data/sound/titleBgm.wav");
	//プレイヤーのグラフィック
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//プリンセスのグラフィック
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//エネミーのグラフィック
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
	//背景のグラフィック
	graphMap["bgGraph"] = LoadGraph("data/image/fieldBg.png");
	//商人のグラフィック
	graphMap["traderGraph"] = LoadGraph("data/image/trader.png");
	//ショップの背景
	graphMap["shopBgGraph"] = LoadGraph("data/image/shopBg.png");
	//セレクトシーンの背景
	graphMap["selectBgGraph"] = LoadGraph("data/image/selectBg.png");
	//アイテムの画像
	graphMap["itemGraph"] = LoadGraph("data/image/item.png");
	//アイテムのフレーム
	graphMap["itemFrameGraph"] = LoadGraph("data/image/frame.png");
	//ゴールドやステージ名を出すときに後ろに出すボックス
	graphMap["backBoxGraph"] = LoadGraph("data/image/backBox.png");
	

}
