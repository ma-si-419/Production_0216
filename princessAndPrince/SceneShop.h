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
	//連続でAボタンが押されないように
	bool m_isAKeyDown;
};

