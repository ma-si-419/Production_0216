#pragma once
#include <list> 
#include <memory>
#include "Fade.h"
class Pad;
class Scene;
class FileManager;

/// <summary>
/// �e�V�[���̑J�ڂƑ�����R���g���[������N���X
/// </summary>
class SceneManager
{
private:
	std::list<std::shared_ptr<Scene>>scenes_;
	std::shared_ptr<Fade> m_pFade;
	bool m_isChangeScene;
	std::shared_ptr<Scene> m_nextScene;
public:
	SceneManager();
	~SceneManager();
	/// <summary>
	/// �����Ă���V�[����Update�֐����Ăяo��
	/// </summary>
	/// <param name="input">���̓N���X</param>
	void Update(Pad& pad);

	/// <summary>
	/// �����Ă���V�[����Draw�֐����Ăяo��
	/// </summary>
	void Draw();

	void SetStartScene(std::shared_ptr<Scene> nextScene);
	/// <summary>
	/// ���ݎ��s���̃V�[���������Ŏw�肳�ꂽ�V�[���ɐ؂�ւ���
	/// </summary>
	void ChangeScene(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// ���ݐ擪�Ŏ��s���̃V�[���̏�ɃV�[�����̂�����
	/// </summary>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���݂̈�Ԍ��̃V�[�����|�b�v(�폜)����
	/// </summary>
	void popScene();
};