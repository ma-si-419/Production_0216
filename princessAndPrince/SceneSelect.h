#pragma once
#include "Scene.h"
class SceneSelect : public Scene
{
public:
	SceneSelect(SceneManager& manager);
	virtual ~SceneSelect();

	void Init();
	void End();

	void Update(Pad& pad);
	void Draw();
private:
	//今選択しているシーンを保存する
	int m_SelectNum;
};

