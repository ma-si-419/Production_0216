#pragma once
#include <iostream>
#include <Windows.h>

struct Size
{
	int w;
	int h;
};

/// <summary>
/// アプリケーション全体を管理するクラス
/// シングルトンクラス
/// </summary>
class Application
{
private:
	Size m_isWindowSize = {};

	//シングルトンのためにprivateに
	Application() {};

	//コピー＆代入を禁止
	Application(const Application& app) = delete;	//コピーコンストラクタの禁止
	void operator=(const Application& app) = delete;	//代入演算子もついでに廃止

	/// <summary>
	/// 後処理を行う
	/// </summary>
	void Terminate();

	LPCSTR m_fontPath = 0; // 読み込むフォントファイルのパス

public:


	/// <summary>
	/// Applicationのシングルトンインスタンスの参照を返す
	/// </summary>
	/// <returns>Applicationインスタンス</returns>
	static Application& GetInstance() {
		static Application instance;	//自分自身の静的オブジェクトを作る
		return instance;
	}

	bool Init();
	void Run();

	const Size& GetWindowSize() const;

	void SetSaveData(int num);

};

