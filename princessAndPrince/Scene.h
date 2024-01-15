#pragma once
#include <memory>
class Pad;
class SceneManager;
class DataManager;
/// <summary>
/// �V�[�����N���X
/// �����ȃV�[���̊�ɂȂ�N���X
/// </summary>
class Scene
{
protected:
	SceneManager& m_sceneManager;	//�V�[���Ǘ��N���X�̎Q��
	DataManager& m_dataManager;

public:
	/// <summary>
	/// ��������SceneManager�̎Q�Ƃ��󂯎���Ă���
	/// </summary>
	/// <param name="manager">SceneManager�̎Q��</param>
	Scene(SceneManager& sceneManager,DataManager& DataManager);
	virtual void Init() = 0;
	/// <summary>
	/// �V�[���̍X�V
	/// </summary>
	/// <param name="input"></param>
	virtual void Update(Pad& pad) = 0;

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	virtual void Draw() = 0;
};