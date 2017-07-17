#include "../stdafx.h"
#include "../AI1_Header/AIForFile.h"
#include "../ChessBoard_Header/SIP.h"
#include <future>
bool AIForFile::ForeReadFileToMemory(int round, std::unordered_multimap<std::string, std::pair<int, int>> &stl)
{
	std::string filename = FS.FN.ForeName + std::to_string(round) + FS.FN.TXT;//文件名
	std::fstream file(filename, std::ios::out | std::ios::in);//文件对象
	if (!file)
	{
		file.close();
		return false;
	}
	stl.clear();//先清空容器
	std::string tempData;
	int site, winner;
	file.seekg(0);
	while (!file.eof())
	{
		file >> tempData >> site >> winner;
		if (file.eof()) break;
		stl.emplace(tempData, std::make_pair(site, winner));
	}
	if (stl.empty()) return false;
	return true;
}