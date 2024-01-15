#pragma once
#include <list> 
#include <memory>
#include "Fade.h"
class Pad;
class Scene;
class FileManager;

/// <summary>
/// 各シーンの遷移と操作をコントロールするクラス
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
	/// 持っているシーンのUpdate関数を呼び出す
	/// </summary>
	/// <param name="input">入力クラス</param>
	void Update(Pad& pad);

	/// <summary>
	/// 持っているシーンのDraw関数を呼び出す
	/// </summary>
	void Draw();

	void SetStartScene(std::shared_ptr<Scene> nextScene);
	/// <summary>
	/// 現在実行中のシーンを引数で指定されたシーンに切り替える
	/// </summary>
	void ChangeScene(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// 現在先頭で実行中のシーンの上にシーンをのっける
	/// </summary>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 現在の一番後ろのシーンをポップ(削除)する
	/// </summary>
	void popScene();
};