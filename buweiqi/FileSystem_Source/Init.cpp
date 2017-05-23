#include "../FileSystem_Header/FileSystem.h"

FileSystem::FileSystem()
{
	OpenFile(AIMemoryFileName, AIMemory);
	OpenFile(TempFileName, TempFile);
	OpenFile(TempHouse1Name, TempHouse1);
	OpenFile(TempHouse2Name, TempHouse2);
	NowTempHouse = &AIMemory;
}
FileSystem::FileSystem(char *FileName)
{
	OpenFile(FileName, MatchFile);
}
///打开文件
void FileSystem::OpenFile(char *FileName, std::fstream &f)
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
	remove(TempHouse1Name);
	remove(TempHouse2Name);
	remove(TempFileName);
}
FileSystem::~FileSystem()
{
	MatchFile.close();
}