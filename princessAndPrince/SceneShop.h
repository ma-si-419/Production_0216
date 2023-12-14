#pragma once
#include "Scene.h"
class SceneShop : public Scene
{
public:
	SceneShop(SceneManager& manager);
	virtual ~SceneShop();

	void Init();
	void Update(Pad& pad);
	void Draw();
};

