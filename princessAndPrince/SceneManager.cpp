#include "SceneManager.h"

#include "Scene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{

}

void SceneManager::Update(Pad& pad)
{
	//––”ö‚Ì‚ÝŽÀs
	scenes_.back()->Update(pad);
}

void SceneManager::Draw()
{
	//æ“ª‚©‚ç‡‚É•`‰æ
	//ÅŒã‚ÉÏ‚ñ‚¾‚à‚Ì‚ªÅŒã‚É•`‰æ‚³‚ê‚é
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	if (scenes_.empty())//ƒŠƒXƒg‚ª‹ó‚Á‚Û‚¾‚Á‚½‚ç“ü‚ê‘Ö‚¦‚¸‚É
	{
		scenes_.push_back(nextScene);//––”ö‚É’Ç‰Á
	}
	else
	{
		scenes_.back() = nextScene;//Šù‚Éˆê‚ÂˆÈã‚ ‚ê‚Î––”ö‚ð
	}
	scenes_.back()->Init();
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);
}

void SceneManager::popScene()
{
	scenes_.pop_back();
}

