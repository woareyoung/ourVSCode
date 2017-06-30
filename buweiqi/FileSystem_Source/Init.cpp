#include "../FileSystem_Header/FileSystem.h"

///���ļ�
void FileSystem::OpenFile(std::string FileName, std::fstream &f)
{
	f.open(FileName, std::ios::out | std::ios::in);//�����ִ򿪷�ʽʱ�����ļ�������ʱ���ļ�ʧ��
	if (!f)
	{
		f.close();
		f.open(FileName, std::ios::out);//�����ִ򿪷�ʽʱ�����ļ�������ʱ�Զ������ļ�
		if (!f) exit(0);//�������ļ�ʧ��
		else
		{
			f.close();
			f.open(FileName, std::ios::out | std::ios::in);
			if (!f) exit(0);//�����ļ�ʧ��
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
