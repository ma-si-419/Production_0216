#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


class MyString
{
public:
	/// <summary>
	/// 第一引数で指定した文字列を第二引数で指定した文字を基準に分割する
	/// </summary>
	/// <param name="str">分割したい文字列</param>
	/// <param name="separator">基準文字</param>
	/// <returns>vector＿stringで返す</returns>
	static std::vector<string> split(string str, string separator);
};

