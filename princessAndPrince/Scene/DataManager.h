#pragma once
#include<string>
#include<map>
class DataManager
{
public:
	DataManager();
	~DataManager();

	void Init();

	int SearchSound(std::string str) { return soundMap[str]; };
	int SearchGraph(std::string str) { return graphMap[str]; };
	int GetFontHandle() { return m_fontHandle; }

private:
	std::map<std::string, int> soundMap;
	std::map<std::string, int> graphMap;

	int m_fontHandle;
};

