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
	//末尾のみ実行
	scenes_.back()->Update(pad);
}

void SceneManager::Draw()
{
	//先頭から順に描画
	//最後に積んだものが最後に描画される
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	if (scenes_.empty())//リストが空っぽだったら入れ替えずに
	{
		scenes_.push_back(nextScene);//末尾に追加
	}
	else
	{
		scenes_.back() = nextScene;//既に一つ以上あれば末尾を
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

