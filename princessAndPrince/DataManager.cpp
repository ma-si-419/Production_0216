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
	soundMap["attackSe"] = LoadSoundMem("data/sound/attackSe.mp3");
	//ダンスするときの効果音
	soundMap["danceSe"] = LoadSoundMem("data/sound/clearSe.mp3");
	//リザルト画面のゴールドの効果音
	soundMap["resultGoldSe"] = LoadSoundMem("data/sound/goldSe.mp3");
	//リザルト画面の経験値の効果音
	soundMap["resultExpSe"] = LoadSoundMem("data/sound/expSe.mp3");
	//フィールドのBgm
	soundMap["fieldBgm"] = LoadSoundMem("data/sound/fieldBgm.wav");
	//ボスが出てきた時のBgm
	soundMap["bossBgm"] = LoadSoundMem("data/sound/bossBgm.wav");
	//ダンスの前の敵がいなくなる時の音
	soundMap["beforeDanceSe"] = LoadSoundMem("data/sound/enemyDelete.mp3");
	//コインを拾った時の音
	soundMap["coinSe"] = LoadSoundMem("data/sound/coinSe.mp3");
	//経験値を拾った時の音
	soundMap["expSe"] = LoadSoundMem("data/sound/dropExpSe.mp3");
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
	soundMap["bloodSe"] = LoadSoundMem("data/sound/bloodSe.mp3");
	//血を渡したときの効果音
	soundMap["passBloodSe"] = LoadSoundMem("data/sound/passBloodSe.mp3");
	//ショップのBGM
	soundMap["shopBgm"] = LoadSoundMem("data/sound/shopBgm.wav");
	//セレクトシーンのBgm
	soundMap["selectBgm"] = LoadSoundMem("data/sound/selectBgm.wav");
	//タイトルシーンのBgm
	soundMap["titleBgm"] = LoadSoundMem("data/sound/titleBgm.wav");
	//炎魔法のSe
	soundMap["fireMagicSe"] = LoadSoundMem("data/sound/fireSe.mp3");
	//Princessにぶつかった時のSe
	soundMap["hitPrincessSe"] = LoadSoundMem("data/sound/hitPrincessSe.mp3");
	//メインシーンに移行するときにならす音
	soundMap["moveMainSceneSe"] = LoadSoundMem("data/sound/moveMainSceneSe.mp3");
	//プレイヤーが倒れた時に鳴らす音(二つ)
	soundMap["playerTurnSe"] = LoadSoundMem("data/sound/playerTurnSe.mp3");
	soundMap["playerDeathSe"] = LoadSoundMem("data/sound/playerDeathSe.mp3");
	//プレイヤーが立ち上がるときにならす音
	soundMap["playerStandUpSe"] = LoadSoundMem("data/sound/playerStandUpSe.mp3");
	//姫が死んだときの効果音
	soundMap["princessDeathSe"] = LoadSoundMem("data/sound/princessDeathSe.mp3");
	//セーブシーンのBGM
	soundMap["saveSceneBgm"] = LoadSoundMem("data/sound/saveSceneBgm.wav");
	//プレイヤーのグラフィック
	graphMap["playerGraph"] = LoadGraph("data/image/Monkey.png");
	//プリンセスのグラフィック
	graphMap["princessGraph"] = LoadGraph("data/image/Princess.png");
	//エネミーのグラフィック
	graphMap["enemyGraph"] = LoadGraph("data/image/Enemy.png");
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
	//背景画像まとめ
	graphMap["allFieldBgGraph"] = LoadGraph("data/image/allFieldBg.png");
	//ショップ画像
	graphMap["shopGraph"] = LoadGraph("data/image/shop.png");
	//選んでいるシーンのフレーム
	graphMap["selectSceneFrameGraph"] = LoadGraph("data/image/selectSceneFrame.png");
	//タイトルシーンの背景
	graphMap["titleSceneBgGraph"] = LoadGraph("data/image/titleBg.png");
	//ゲーム開始時に出るREADY
	graphMap["READYGraph"] = LoadGraph("data/image/READY.png");
	//ポーズ時に出るPAUSE
	graphMap["PAUSEGraph"] = LoadGraph("data/image/PAUSE.png");
	//タイトルのロゴ
	graphMap["titleGraph"] = LoadGraph("data/image/title.png");
	//ボタンのUI
	graphMap["buttonsGraph"] = LoadGraph("data/image/buttons.png");
	//セレクト画面のボタン説明
	graphMap["selectSceneButtonUiGraph"] = LoadGraph("data/image/buttonsUi.png");
	//シーンメインのUiを表示する背景
	graphMap["mainUiBgGraph"] = LoadGraph("data/image/mainUi.png");
	//魔法のUiを表示する背景のグラフィック
	graphMap["magicUiBgGraph"] = LoadGraph("data/image/witchMagicUi.png");
	//怒りゲージのUi
	graphMap["angryGaugeUiGraph"] = LoadGraph("data/image/angryGauge.png");
	//怒りモード時の王子
	graphMap["angryMonkeyGraph"] = LoadGraph("data/image/angryMonkey.png");
	//怒りモード時のエフェクト
	graphMap["angryFireGraph"] = LoadGraph("data/image/angryFire.png");
	//ビックリマーク
	graphMap["exclamationMarkGraph"] = LoadGraph("data/image/exclamationMark.png");
	//黒いボックス
	graphMap["boxGraph"] = LoadGraph("data/image/box.png");
	//買えるアイテムの右上に出す画像
	graphMap["canBuyItemMarkGraph"] = LoadGraph("data/image/canBuyItemMark.png");
	//セーブデータを選択する画面の背景
	graphMap["sceneSaveGraph"] = LoadGraph("data/image/saveDataScene.png");
	//セーブデータのフレームの画像
	graphMap["saveDataFrameGraph"] = LoadGraph("data/image/saveDataFrame.png");
	//セーブシーンのカーソル
	graphMap["saveSceneCursorGraph"] = LoadGraph("data/image/cursor.png");
	//魔法の方向を示す矢印
	graphMap["magicArrowGraph"] = LoadGraph("data/image/magicArrow.png");
	//怒りゲージがたまったときのボタン表示画像
	graphMap["angryButtonMarkGraph"] = LoadGraph("data/image/angryButtonMark.png");
	//怒りゲージが使えないときの画像
	graphMap["stoneAngryGaugeGraph"] = LoadGraph("data/image/stoneAngryGauge.png");
	//ショップに表示する戻るボタン
	graphMap["backButtonGraph"] = LoadGraph("data/image/backButton.png");
	//チュートリアル画像
	graphMap["tutorialGraph1"] = LoadGraph("data/image/tutorial1.png");
	graphMap["tutorialGraph2"] = LoadGraph("data/image/tutorial2.png");
	graphMap["tutorialGraph3"] = LoadGraph("data/image/tutorial3.png");
	graphMap["tutorialGraph4"] = LoadGraph("data/image/tutorial4.png");
	graphMap["tutorialGraph5"] = LoadGraph("data/image/tutorial5.png");
	graphMap["tutorialGraph6"] = LoadGraph("data/image/tutorial6.png");
	graphMap["tutorialGraph7"] = LoadGraph("data/image/tutorial7.png");
	//ストーリー画像
	graphMap["storyGraph1"] = LoadGraph("data/image/story1.png");
	graphMap["storyGraph2"] = LoadGraph("data/image/story2.png");
	graphMap["storyGraph3"] = LoadGraph("data/image/story3.png");
	graphMap["storyGraph4"] = LoadGraph("data/image/story4.png");
	graphMap["storyGraph5"] = LoadGraph("data/image/story5.png");
	graphMap["storyGraph6"] = LoadGraph("data/image/story6.png");
	graphMap["storyGraph7"] = LoadGraph("data/image/story7.png");
	graphMap["storyGraph8"] = LoadGraph("data/image/story8.png");

}
