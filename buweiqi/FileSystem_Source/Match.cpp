#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
std::shared_ptr<NEXTPACE> FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	std::shared_ptr<NEXTPACE> np = NULL;//记录“下一步”应对方法的链表的头结点
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
				for (; i < 10; ++i) 
					TempFile >> value[0];
				break;
			}
			else if (i == 9)
			{
				if (np == NULL)
				{
					temp = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					np = temp;
					temp->next = NULL;
				}
				else
				{
					temp->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					temp = temp->next;
					temp->next = NULL;
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
	np->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);//用两个结点标记链表是否有效
	std::shared_ptr<NEXTPACE> tempHead = np->next;//动态头结点
	std::shared_ptr<NEXTPACE> tempRear = tempHead;//动态尾结点
	tempHead->site = 0;
	tempHead->next = NULL;
	np->site = 0;
	while (!TempFile.eof())
	{
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			//包含当前盘面状况
			if (Compare(value[i] / 10000, StatusQuo.Line[i] / 10000, tempHead, tempRear))
			{
				if (np->site == 0)
				{
					//第一个结点不要了
					// delete np;
					np = nullptr;
					np = tempHead;
				}
				for (tempRear = tempHead; ; tempRear = tempRear->next)
				{
					tempRear->site += i * 10;//把每一个值加上“行号”
					if (tempRear->next == NULL)
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
				tempHead = np->next;
				//链表只有一个结点（这种情况概率极少）
				if (tempHead == NULL)
				{
					np->site = 0;
					np->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					tempHead = np->next;
					tempHead->next = nullptr;
					tempRear = tempHead;
					tempHead->site = 0;
				}
				//链表不止一个结点且第二个结点值大于0（即为有效链表）
				else if (tempHead->site > 0)
				{
					ClearList(tempHead);//从第二个结点开始清空链表
					np->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					tempHead = np->next;
					tempHead->site = 0;
					tempRear = tempHead;
					np->site = 0;
					np->next = nullptr;
				}
				//链表依然无效
				else
				{
					tempHead = np->next;
					tempRear = np->next;
					tempHead->site = 0;
					tempHead->next = nullptr;
				}
				for (; i < 10; i++) TempFile >> value[0];
				break;
			}
		}
	}
	TempFile.close();
	if (np->site == 0)
	{
		np->next = nullptr;
		np = nullptr;
		return NULL;
	}
	return np;
}
///将压缩的数值解压出来，并将列号从大到小排列
std::shared_ptr<NEXTPACE> FileSystem::UnPack(int value)
{
	if (value == 0) return NULL;
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
			if (np == NULL) 
			{
				np = std::shared_ptr<NEXTPACE>(new NEXTPACE);
				temp = np;
				np->next = NULL;
			}
			else 
			{
				temp->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
				temp = temp->next;
				temp->next = NULL;
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
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//获取“子集”
	if (np2 == NULL) return true;//任何集合包含空集
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//获取“全集”
	if (np1 == NULL) return false;
	std::shared_ptr<NEXTPACE> temp2 = np2;
	std::shared_ptr<NEXTPACE> temp1 = np1;
	while(temp1 != NULL && temp2 != NULL)
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
	while (temp1 != NULL)
	{
		rear->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
		rear = rear->next;
		rear->next = nullptr;
		rear->site = temp1->site;
		temp1 = temp1->next;
	}
	ClearList(np1);//清空链表
	ClearList(np2);
	return true;
}
