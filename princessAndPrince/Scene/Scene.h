#pragma once
#include <memory>
class Pad;
class SceneManager;
class DataManager;
/// <summary>
/// シーン基底クラス
/// いろんなシーンの基になるクラス
/// </summary>
class Scene
{
protected:
	SceneManager& m_sceneManager;	//シーン管理クラスの参照
	DataManager& m_dataManager;

public:
	/// <summary>
	/// 生成時にSceneManagerの参照を受け取っておく
	/// </summary>
	/// <param name="manager">SceneManagerの参照</param>
	Scene(SceneManager& sceneManager,DataManager& DataManager);
	virtual void Init() = 0;
	/// <summary>
	/// シーンの更新
	/// </summary>
	/// <param name="input"></param>
	virtual void Update(Pad& pad) = 0;

	/// <summary>
	/// シーンの描画
	/// </summary>
	virtual void Draw() = 0;
};