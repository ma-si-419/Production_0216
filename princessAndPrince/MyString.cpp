#include "MyString.h"

vector<string> MyString::split(string str, string separator)
{
	if (separator == "") return { str };
	vector<string> result;
	string tstr = str + separator;
	long l = tstr.length(), sl = separator.length();
	string::size_type pos = 0, prev = 0;

	for (; pos < l && (pos = tstr.find(separator, pos)) != string::npos; prev = (pos += sl)) {
		result.emplace_back(tstr, prev, pos - prev);
	}
	return result;
}