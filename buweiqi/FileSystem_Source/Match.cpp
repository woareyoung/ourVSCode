#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
std::shared_ptr<NEXTPACE> FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	std::shared_ptr<NEXTPACE> np = nullptr;//记录“下一步”应对方法的链表的头结点
	std::shared_ptr<NEXTPACE> temp = np;
	int i;
 	SITUATION situa;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//打开当前盘面状况的记录文件
	situa.Line[0] = StatusQuo.Line[0];
	for (i = 1; i < 10; ++i) situa.Line[i] = StatusQuo.Line[i];
	TempFile.seekg(0);
	while (!TempFile.eof())//循环直到文件末尾
	{
		for (i = 1; i < 10; ++i)
		{
			TempFile >> value[i];
			if (situa.Line[i] != value[i])//只要有一个数据不相同，则匹配下一条记录
			{
				for (; i < 10; ++i) TempFile >> value[0];
				break;
			}
			else if (i == 9)
			{
				if (np == nullptr)
				{
					temp = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					np = temp;
					temp->next = nullptr;
				}
				else
				{
					temp->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					temp = temp->next;
					temp->next = nullptr;
				}
				TempFile >> temp->site;
			}
		}
	}
	TempFile.close();
	return np;
}
///匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
std::shared_ptr<NEXTPACE> FileSystem::GenerMatch(SITUATION &StatusQuo, int player, int round)
{
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;
	OpenFile(name, TempFile);
	TempFile.seekg(0);
	int i, j;
	for (i = 0; i < 10; i++) value[i] = 0;
	std::shared_ptr<NEXTPACE> np = std::shared_ptr<NEXTPACE>(new NEXTPACE);//记录未下棋位置的链表的头结点
	std::shared_ptr<NEXTPACE> tempHead = np;//动态头结点
	std::shared_ptr<NEXTPACE> tempRear = tempHead;//动态尾结点
	tempHead->next = nullptr;
	np->site = 0;
	while (!TempFile.eof())
	{
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			//包含当前盘面状况
			if (Compare(value[i] / 10000, StatusQuo.Line[i] / 10000, tempHead, tempRear))
			{
				for (tempRear = tempHead; ; tempRear = tempRear->next)
				{
					tempRear->site += i * 10;//把每一个值加上“行号”
					if (tempRear->next == nullptr)
					{
						tempRear->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
						tempHead = tempRear->next;
						tempRear = tempHead;
						tempHead->site = 0;
						tempHead->next = nullptr;
						break;
					}
				}
				continue;
			}
			else
			{
				//不符合条件，清空链表
				ClearList(np);
				np = std::shared_ptr<NEXTPACE>(new NEXTPACE);//记录未下棋位置的链表的头结点
				tempHead = np;//动态头结点
				tempRear = tempHead;//动态尾结点
				tempHead->next = nullptr;
				np->site = 0;
				for (; i < 10; i++) TempFile >> value[0];
				break;
			}
		}
	}
	TempFile.close();
	if (np->site == 0)
	{
		np = nullptr;
		return nullptr;
	}
	return np;
}
///将压缩的数值解压出来，并将列号从大到小排列
std::shared_ptr<NEXTPACE> FileSystem::UnPack(int value)
{
	if (value == 0) return nullptr;
	std::shared_ptr<NEXTPACE> np = nullptr;
	std::shared_ptr<NEXTPACE> temp = np;
	int tempValue = 512;//2的9次方
	for (int i = 9; i > 0; i--)
	{
		if (value < tempValue) 
		{
			tempValue /= 2; 
			continue;
		}
		else 
		{
			if (np == nullptr)
			{
				np = std::shared_ptr<NEXTPACE>(new NEXTPACE);
				temp = np;
				np->next = nullptr;
			}
			else 
			{
				temp->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
				temp = temp->next;
				temp->next = nullptr;
			}
			temp->site = i;
			value -= tempValue;
			if (value == 0) break;
		}
		tempValue /= 2;
	}
	return np;
}
//比较两个数值，查看文件棋谱是否包含当前状况
bool FileSystem::Compare(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//获取“全集”
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//获取“子集”
	//任何集合包含空集
	if (np2 == nullptr && np1 != nullptr)
	{
		ConnectList(rear, np1);
		ClearList(np1);
		return true;
	}
	else if (np1 == nullptr && np2 != nullptr)
	{
		ClearList(np1);
		ClearList(np2);
		head = nullptr;
		rear = nullptr;
		return false;
	}
	else if (np1 == nullptr && np2 == nullptr)
	{
		head = nullptr;
		rear = nullptr;
		return true;
	}
	std::shared_ptr<NEXTPACE> temp2 = np2;
	std::shared_ptr<NEXTPACE> temp1 = np1;
	while(temp1 != nullptr && temp2 != nullptr)
	{
		//若存在全集中没有子集中的元素，则返回false，表示不包含
		if (temp1->site < temp2->site)
		{
			ClearList(np1);//清空链表
			ClearList(np2);
			ClearList(head);
			head = nullptr;
			rear = nullptr;
			return false;
		}
		//若子集中的元素也存在于全集中，则遍历下一个元素
		else if (temp1->site == temp2->site)
		{
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
		else if (temp1->site > temp2->site)
		{
			if (rear->site < 1) rear->site = temp1->site;
			else
			{
				rear->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
				rear = rear->next;
				rear->next = nullptr;
				rear->site = temp1->site;
			}
			temp1 = temp1->next;
		}
	}
	ConnectList(rear, temp1);
	ClearList(np1);//清空链表
	ClearList(np2);
	return true;
}
void FileSystem::ConnectList(std::shared_ptr<NEXTPACE> Aim, std::shared_ptr<NEXTPACE> Source)
{
	while (Source != nullptr)
	{
		if (Aim->site < 1) Aim->site = Source->site;
		else
		{
			Aim->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
			Aim = Aim->next;
			Aim->next = nullptr;
			Aim->site = Source->site;
		}
		Source = Source->next;
	}
}