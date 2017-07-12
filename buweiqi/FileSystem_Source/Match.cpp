#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
void FileSystem::Match(SITUATION &StatusQuo, std::set<int> &result, int round)
{
	std::string data = "";
	int temp;
	int count = 0;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//打开当前盘面状况的记录文件
	TempFile.seekg(0);
	while (!TempFile.eof())//循环直到文件末尾
	{
		TempFile >> data;
		TempFile >> temp;
		if(data == StatusQuo.BoardStatus) result.insert(temp);
	}
	TempFile.close();
}
///匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
void FileSystem::GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round)
{
	int count = 0;
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;//文件名
	OpenFile(name, TempFile);//打开文件
	TempFile.seekg(0);
	std::string data = "";
	std::set<int> tempSTL1, tempSTL2;
	std::set<int> one1, two1, one2, two2;
	bool com = false, wait = true;
	while (!TempFile.eof())
	{
		TempFile >> data;
		UnPack(data, one1, two1);
		UnPack(StatusQuo.BoardStatus, one2, two2);
		com = Compare(one1, one2, tempSTL1);
		if (!com)
		{
			TempFile >> count;
			continue;
		}
		com = Compare(two1, two2, tempSTL2);
		if (!com)
		{
			TempFile >> count;
			continue;
		}
		std::async(std::launch::async, [&]() {
			tempSTL2.clear();
			TempFile >> count;
			wait = false;
		});
		for (auto t : tempSTL1)
			Parent.insert(t);
		tempSTL1.clear();
		while (wait) {}
	}
	TempFile.close();
}
///将压缩的数值解压出来
void FileSystem::UnPack(std::string data, std::set<int> &OneData, std::set<int> &TwoData)
{
	int sum;
	for (int i = 0, len = data.length(); i < len; i++)
	{
		sum = i + 1;
		sum = sum / 9 * 10 + sum % 9;
		if (data[i] == 1) OneData.insert(sum);
		else if(data[i] == 2) TwoData.insert(sum);
	}
}
//比较两个数值，查看文件棋谱是否包含当前状况
bool FileSystem::Compare(std::set<int> &FileValue, std::set<int> &CurrentValue, std::set<int> &stl)
{
	if (CurrentValue.empty())
	{
		for (auto t : FileValue)
			stl.insert(t);
		return true;
	}
	else if (FileValue.empty()) return false;
	bool com = true;
	std::set<int>::iterator itend = FileValue.end();
	for (auto t : CurrentValue)
	{
		if (FileValue.find(t) == itend)
		{
			stl.clear();
			return false;
		}
	}
	return true;
}