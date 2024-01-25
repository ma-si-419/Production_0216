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
	SceneShop(SceneManager& sceneManager,DataManager& DataManager);
	virtual ~SceneShop();

	void Init();
	void Update(Pad& pad);
	void Draw();
	//今選択している項目を保存する
	int m_stageSelectNum;
	//連続でキーが押されてないように
	bool m_isKeyDown;
	//カーソルが連続で移動しないように
	bool m_isSelectKeyDown;
	//今回った数を保存する
	int m_loopCount;
	//プレイヤーのアイテムの値段の配列
	int m_playerItemPriceList[20];
	//魔女のアイテムの値段の配列
	int m_princessItemPriceList[20];
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
	//アイテムのグラフィック
	int m_itemGraph;
	//アイテムのフレームのグラフィック
	int m_itemFrameGraph;
};

