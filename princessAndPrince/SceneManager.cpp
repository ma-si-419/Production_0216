#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager() :
	m_isChangeScene(false)
{
	m_pFade = std::make_shared<Fade>();
}

SceneManager::~SceneManager()
{

}

void SceneManager::Update(Pad& pad)
{
	if (m_isChangeScene && m_pFade->GetFadeState() == kFadeIn)
	{
		if (scenes_.empty())//リストが空っぽだったら入れ替えずに
		{
			scenes_.push_back(m_nextScene);//末尾に追加
		}
		else
		{
			scenes_.back() = m_nextScene;//既に一つ以上あれば末尾を
		}

		scenes_.back()->Init();
		m_isChangeScene = false;

	}
	m_pFade->Update();
	if (m_pFade->GetFadeState() == kNormal)
	{
		//末尾のみ実行
		scenes_.back()->Update(pad);
	}
}

void SceneManager::Draw()
{
	//先頭から順に描画
	//最後に積んだものが最後に描画される
	for (auto& scene : scenes_)
	{
		scene->Draw();
		m_pFade->Draw();
	}
}

void SceneManager::SetStartScene(std::shared_ptr<Scene> nextScene)
{
	if (scenes_.empty())//リストが空っぽだったら入れ替えずに
	{
		scenes_.push_back(nextScene);//末尾に追加
	}
	else
	{
		scenes_.back() = nextScene;//既に一つ以上あれば末尾を
	}
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	m_pFade->StartFadeOut();
	m_isChangeScene = true;
	m_nextScene = nextScene;
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);
}

void SceneManager::popScene()
{
	scenes_.pop_back();
}

