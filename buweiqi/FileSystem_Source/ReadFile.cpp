#include "../FileSystem_Header/FileSystem.h"

//读文件到容器
void FileSystem::ReadFile(int round)
{
	bool ThreadFinish[OpenFileThreadNumber] = { false };//标记线程是否执行完毕
	bool MainFinish = false;//标记父线程是否可以离开了
	auto ReadFileToSTL = [&](std::string fileName, int ThreadNum, std::unordered_multimap<std::string, int> &stl) {
		int temp;
		std::string data;
		CountFile[ThreadNum].open(fileName, std::ios::in | std::ios::out);
		if (!CountFile) return;//打开文件失败
		//将文件读取到容器中
		while (!CountFile[ThreadNum].eof())
		{
			CountFile[ThreadNum].seekg(0);
			CountFile[ThreadNum] >> data >> temp;
			stl.insert(std::pair<std::string, int>(data, temp));
		}
		CountFile[ThreadNum].close();
		ThreadFinish[ThreadNum] = true;
	};
	int i;
	if (CountMapUseLack)
	{
		for (i = 0; i < OpenFileThreadNumber; i++)
		{
			std::thread t(ReadFileToSTL, FN.ForeName + std::to_string(round + i) + FN.TXT, i, CCountMap[i]);
			if (i == OpenFileThreadNumber - 1) t.join();//当到最后一条线程的时候，等待它执行完毕再离开
			else t.detach();//使子线程与父线程分离执行
		}
	}
	else
	{
		for (i = 0; i < OpenFileThreadNumber; i++)
		{
			std::thread t(ReadFileToSTL, FN.ForeName + std::to_string(round + i) + FN.TXT, i, CountMap[i]);
			if (i == OpenFileThreadNumber - 1) t.join();//当到最后一条线程的时候，等待它执行完毕再离开
			else t.detach();//使子线程与父线程分离执行
		}
		CountMapUseLack = true;
	}
	//等待子线程
	while (!MainFinish)
	{
		for (i = 0; i < OpenFileThreadNumber; i++)
		{
			//只要还有线程未执行完毕，则继续等待
			if (!ThreadFinish[i])
			{
				MainFinish = false;
				break;
			}
			else if (i == OpenFileThreadNumber - 1) MainFinish = true;
		}
	}
}