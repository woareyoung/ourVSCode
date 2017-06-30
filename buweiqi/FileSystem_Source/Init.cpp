#include "../FileSystem_Header/FileSystem.h"

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
void FileSystem::ClearList(std::shared_ptr<NEXTPACE> HeadNode)
{
	if (HeadNode == nullptr) return;
	std::shared_ptr<NEXTPACE> temp = HeadNode->next;
	HeadNode = nullptr;
	while(temp != nullptr)
	{
		HeadNode = temp;
		temp = temp->next;
		HeadNode = nullptr;
	}
}
