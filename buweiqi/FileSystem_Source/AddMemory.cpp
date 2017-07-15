#include "../stdafx.h"
#include "../FileSystem_Header/FileSystem.h"
#include <mutex>

#define ThreadNum 4 //��дͬʱ���У���ʵ��ռ���߳���Ϊ ThreadNum * 2

///��������Ϸ��¼���ܼ����
void FileSystem::AddMemory(std::list<SITUATION> header, int winner)
{
	std::list<SITUATION>::iterator it = header.begin();
	std::mutex g_lock;//������
	bool WriteThreadFinish[ThreadNum] = { false };//����߳��Ƿ����
	bool threadFinish[ThreadNum] = { false };
	auto WriteF = [&](int round, std::string Boarddata, int site) {
		std::string name = FN.ForeName + std::to_string(round) + FN.TXT;
		std::fstream file;
		OpenFile(name, file);
		bool repeat = false;
		int temp;
		int Win;
		std::string tempstr;
		//���ļ��е����ݶ����ݴ�����
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
