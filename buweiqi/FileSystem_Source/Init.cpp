#include "../FileSystem_Header/FileSystem.h"

FileSystem::FileSystem()
{
	AIMemoryFileName = "AIMemory.txt";//总记忆库文件（即内容只增加不减少的文件）
	TempFileName = "TempFile.txt";//临时辅助记忆库文件（记录与当前盘面状况一模一样的记忆）
	TempHouse1Name = "TempHouse1.txt";//临时记忆库文件
	TempHouse2Name = "TempHouse2.txt";//临时记忆库文件
}
///打开文件
void FileSystem::OpenFile(std::string FileName, std::fstream &f)
{
	f.open(FileName, std::ios::out | std::ios::in);//用这种打开方式时，当文件不存在时打开文件失败
	if (!f)
	{
		f.close();
		f.open(FileName, std::ios::out);//用这种打开方式时，当文件不存在时自动创建文件
		if (!f) exit(0);//若创建文件失败
		else
		{
			f.close();
			f.open(FileName, std::ios::out | std::ios::in);
			if (!f) exit(0);//若打开文件失败
		}
	}
}
///关闭文件
void FileSystem::CloseFile()
{
	AIMemory.close();
	TempFile.close();
	TempHouse1.close();
	TempHouse2.close();
	remove(TempHouse1Name.data());
	remove(TempHouse2Name.data());
	remove(TempFileName.data());
}
FileSystem::~FileSystem()
{
}