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
	/// �������Ŏw�肵���������������Ŏw�肵����������ɕ�������
	/// </summary>
	/// <param name="str">����������������</param>
	/// <param name="separator">�����</param>
	/// <returns>vector�Qstring�ŕԂ�</returns>
	static std::vector<string> split(string str, string separator);
};

