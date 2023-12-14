#include "SceneShop.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneSelect.h"
SceneShop::SceneShop(SceneManager& manager):
	Scene(manager)
{
}

SceneShop::~SceneShop()
{
}

void SceneShop::Init()
{
}

void SceneShop::Update(Pad& pad)
{
}

void SceneShop::Draw()
{
	DrawString(300,300,"•P‚Æ‰¤Žq", GetColor(255, 255, 255), true);
}
