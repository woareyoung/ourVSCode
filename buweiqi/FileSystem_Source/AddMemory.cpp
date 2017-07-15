#include "../stdafx.h"
#include "../FileSystem_Header/FileSystem.h"
#include <mutex>

#define ThreadNum 4 //读写同时进行，故实际占用线程数为 ThreadNum * 2

///将本局游戏记录到总记忆库
void FileSystem::AddMemory(std::list<SITUATION> header, int winner)
{
	std::list<SITUATION>::iterator it = header.begin();
	std::mutex g_lock;//互斥锁
	bool WriteThreadFinish[ThreadNum] = { false };//标记线程是否完成
	bool threadFinish[ThreadNum] = { false };
	auto WriteF = [&](int round, std::string Boarddata, int site) {
		std::string name = FN.ForeName + std::to_string(round) + FN.TXT;
		std::fstream file;
		OpenFile(name, file);
		bool repeat = false;
		int temp;
		int Win;
		std::string tempstr;
		//将文件中的内容读到暂存区中
		while (!file.eof())
		{
			file >> tempstr >> temp >> Win;
			if (tempstr == Boarddata && temp == site)
			{
				repeat = true;
				break;
			}
		}
		if (!repeat)
		{
			file.close();
			OpenFile(name, file);
			file.seekg(0, std::ios::end);
			file << it->BoardStatus << " " << it->ChessPosition << " " << winner << std::endl;
		}
		file.close();
	};
	for (int j = 1; it != header.end();)
	{
		for (int i = 0; i < ThreadNum; ++i)
		{
			if (threadFinish[i] == false)
			{
				if (it == header.end()) break;
				std::async(std::launch::async, [&]() {
					g_lock.lock();
					threadFinish[i] = true;
					WriteF(j, it->BoardStatus, it->ChessPosition);
					threadFinish[i] = false;
					g_lock.unlock();
				});
				++j;
				++it;
			}
			if (i == ThreadNum - 1) i = 0;
		}
//		WriteF(j, it->BoardStatus, it->ChessPosition);
	}
}
