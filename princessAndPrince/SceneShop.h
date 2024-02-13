#pragma once
#include "Scene.h"
#include<list>
using namespace std;
struct ItemPrice
{
	int playerItemPrice;
	int princessItemPrice;
};
class SceneShop : public Scene
{
public:
	SceneShop(SceneManager& sceneManager,DataManager& DataManager,int selectSceneNum);
	virtual ~SceneShop();

	void Init();
	void Update(Pad& pad);
	void Draw();
	/// <summary>
	/// 桁数に応じて数値を右揃えにする
	/// </summary>
	/// <param name="num">右揃えにしたい数値</param>
	/// <returns>右揃えにしたい数値*フォントの大きさ</returns>
	int ArrRight(int num);
	//桁数を取得する
	int GetDigits(int num);
	//帰るアイテムに赤丸を付ける
	void DrawRedCircle();

private:
	//今選択している項目を保存する
	int m_itemSelectNum;
	//連続でキーが押されてないように
	bool m_isKeyDown;
	//カーソルが連続で移動しないように
	bool m_isSelectKeyDown;
	//今回った数を保存する
	int m_loopCount;
	//プレイヤーのアイテムの値段の配列
	int m_playerItemPriceList[21];
	//魔女のアイテムの値段の配列
	int m_princessItemPriceList[21];
	//カーソルを動かしたときの効果音
	int m_cursorSe;
	//フェード中に連打できないように
	bool m_isFade;
	//Bボタンを押したときの音
	int m_cancelSe;
	//アイテムを買った時の効果音
	int m_buySe;
	//アイテムを買えなかった時の効果音
	int m_missBuySe;
	//商人のグラフィック
	int m_traderGraph;
	//プレイヤーのグラフィック
	int m_playerGraph;
	//Princessのグラフィック
	int m_princessGraph;
	//Shopの背景
	int m_bgGraph;
	//入ってきたときに文章を表示させる
	bool m_isShowString;
	//アイテムを買えたかどうか
	bool m_isBuy;
	//カーソルを動かしたかどうか
	bool m_isMoveCursor;
	//アイテムのグラフィック
	int m_itemGraph;
	//アイテムのフレームのグラフィック
	int m_itemFrameGraph;
	//所持ゴールドを表示する時の後ろのボックス
	int m_backBoxGraph;
	//アイテムのフレームを拡大縮小する変数
	float m_itemFrameRatio;
	//アイテムフレームを拡大しているか縮小しているか
	int m_isFrameRatio;
	//Bgm
	int m_bgm;
	//セレクトシーンで選んでいたシーンを保存する
	int m_selectSceneNum;
	//選んだ商品のレベルがMAXだったら
	bool m_isMax;
	//引くゴールドの量
	int m_subGold;
	//表示するゴールドの量
	int m_showGold;
	//減らしている間所持金を揺らす
	int m_shakeGoldPosY;
	//買えるアイテムの右上につけるマーク
	int m_canBuyItemMark;
	//戻るボタンのグラフィック
	int m_backButtonGraph;
};

