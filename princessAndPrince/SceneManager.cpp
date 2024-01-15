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
		if (scenes_.empty())//���X�g������ۂ����������ւ�����
		{
			scenes_.push_back(m_nextScene);//�����ɒǉ�
		}
		else
		{
			scenes_.back() = m_nextScene;//���Ɉ�ȏ゠��Ζ�����
		}

		scenes_.back()->Init();
		m_isChangeScene = false;

	}
	m_pFade->Update();
	if (m_pFade->GetFadeState() == kNormal)
	{
		//�����̂ݎ��s
		scenes_.back()->Update(pad);
	}
}

void SceneManager::Draw()
{
	//�擪���珇�ɕ`��
	//�Ō�ɐς񂾂��̂��Ō�ɕ`�悳���
	for (auto& scene : scenes_)
	{
		scene->Draw();
		m_pFade->Draw();
	}
}

void SceneManager::SetStartScene(std::shared_ptr<Scene> nextScene)
{
	if (scenes_.empty())//���X�g������ۂ����������ւ�����
	{
		scenes_.push_back(nextScene);//�����ɒǉ�
	}
	else
	{
		scenes_.back() = nextScene;//���Ɉ�ȏ゠��Ζ�����
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

