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
	/// ‘æˆêˆø”‚Åw’è‚µ‚½•¶š—ñ‚ğ‘æ“ñˆø”‚Åw’è‚µ‚½•¶š‚ğŠî€‚É•ªŠ„‚·‚é
	/// </summary>
	/// <param name="str">•ªŠ„‚µ‚½‚¢•¶š—ñ</param>
	/// <param name="separator">Šî€•¶š</param>
	/// <returns>vectorQstring‚Å•Ô‚·</returns>
	static std::vector<string> split(string str, string separator);
};

