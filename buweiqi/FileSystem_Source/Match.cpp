#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
std::shared_ptr<NEXTPACE> FileSystem::Match(SITUATION &StatusQuo, int &count, int round, bool isWinner)
{
	std::shared_ptr<NEXTPACE> np = nullptr;//记录“下一步”应对方法的链表的头结点
	std::shared_ptr<NEXTPACE> temp = np;
	std::shared_ptr<NEXTPACE> last = nullptr;
	int i;
	SITUATION situa;
	if (!isWinner) for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	else situa = StatusQuo;
	count = 0;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//打开当前盘面状况的记录文件
	TempFile.seekg(0);
	while (!TempFile.eof())//循环直到文件末尾
	{
		for (i = 1; i < 10; ++i)
		{
			TempFile >> value[i];
			if (situa.Line[i] != value[i])//只要有一个数据不相同，则匹配下一条记录
			{
				for (; i < 11; ++i) TempFile >> value[0];
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
					last = temp;
					temp = temp->next;
					temp->next = nullptr;
				}
				TempFile >> temp->site;
				if (temp->site < 0)
				{
					temp = nullptr;
					last->next = nullptr;
					temp = last;
				}
				TempFile >> count;
			}
		}
	}
	TempFile.close();
	return np;
}
///匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
std::shared_ptr<NEXTPACE> FileSystem::GenerMatch(SITUATION &StatusQuo, int &count, int round, bool needptr)
{
	count = 0;
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;
	OpenFile(name, TempFile);
	TempFile.seekg(0);
	int i, one, two, NoWant;
	bool HighSimilar = false, LowSimilar = false;
	bool wait = true;
	for (i = 0; i < 10; i++) value[i] = 0;
	std::shared_ptr<NEXTPACE> npHead = nullptr;//记录未下棋位置的链表的头结点
	std::shared_ptr<NEXTPACE> npRear = nullptr;
	std::shared_ptr<NEXTPACE> np = nullptr;/*子链*/
	std::shared_ptr<NEXTPACE> tempHead = nullptr;//动态头结点
	std::shared_ptr<NEXTPACE> tempH = nullptr;/*孙链*/
	std::shared_ptr<NEXTPACE> tempRear = nullptr;//动态尾结点
	while (!TempFile.eof())
	{
		HighSimilar = false;
		LowSimilar = false;
		wait = true;
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			one = value[i];
			two = StatusQuo.Line[i];
			std::async(std::launch::async, [&]()
			{
				LowSimilar = CompareLow(one % 10000, two % 10000);//低位是否包含
				wait = false;
			});
			tempH = std::shared_ptr<NEXTPACE>(new NEXTPACE);
			tempRear = tempH;
			HighSimilar = CompareHigh(value[i] / 10000, StatusQuo.Line[i] / 10000, tempH, tempRear, needptr);//判断高位是否包含
			if (tempH->site < 1)
			{
				tempH = nullptr;
				tempRear = nullptr;
			}
			if (!HighSimilar)
			{
				tempH = nullptr;
				tempRear = nullptr;
			}
			//-------------------把孙链拼接到子链（不查重）---------------------//
			//如果数据是0，则不用新建结点
			else if (value[i] != 0)
			{
				if (np == nullptr)
				{
					np = tempH;
					tempHead = tempRear;
				}
				else
				{
					tempHead->next = tempH;
					tempHead = tempH;
				}
				if(tempHead != nullptr) tempHead->next = nullptr;
			}
			//-------------------把孙链拼接到子链（不查重）---------------------//
			else
			{
				ClearList(tempH);
				tempH = nullptr;
				tempRear = nullptr;
				if(tempHead != nullptr) tempHead->next = nullptr;
			}
			while (wait) {}
			if (value[i] == 0 && i < 9) continue;
			//包含当前盘面状况
			if (HighSimilar && LowSimilar)
			{
				for (tempRear = tempHead; needptr == true && value[i] != 0 && tempRear != nullptr; tempRear = tempRear->next)
				{
					tempRear->site += i * 10;//把每一个值加上“行号”
					if (tempRear->next == nullptr) break;
				}
				if (i == 9)
				{
					TempFile >> i >> i;
					count += i;
					break;
				}
				continue;
			}
			//不包含当前状况
			else if(needptr)
			{
				//不符合条件，清空链表
				ClearList(np);
				np = nullptr;
				tempHead = nullptr;
				tempRear = nullptr;
			}
			for (; i < 11; i++) TempFile >> NoWant;
			break;
		}
		//-------------------------把子链查重并拼接到父链------------------------------//
		if (np != nullptr)
		{
			//如果父链为空，则创建一个结点
			if (npHead == nullptr)
			{
				npHead = np;
				npRear = npHead;
			}
			//遍历子链
			for (tempH = np->next; tempH != nullptr;)
			{
				np = tempH;
				//跳过不正常的数据
				if (tempH->site < 10)
				{
					tempH = tempH->next;//下一个结点
					np = nullptr;//释放内存
					continue;
				}
				wait = false;//标记有没有重复
				//遍历父链
				for (tempRear = npHead; tempRear != nullptr;)
				{
					//发现相同的数据
					if (tempRear->site == tempH->site)
					{
						tempH = tempH->next;
						np = nullptr;
						wait = true;
						break;
					}
					if (tempRear->next == nullptr) break;
					else tempRear = tempRear->next;
				}
				if (wait) continue;//若重复，遍历子链的下一个结点
				//没有重复，就将结点加入到父链末尾中去
				tempRear->next = tempH;
				tempRear = tempRear->next;
			}
			ClearList(np);
			np = nullptr;//把子链清空
			npRear->next = nullptr;
		}
		//-------------------------把子链拼接到父链------------------------------//
	}
	TempFile.close();
	if (needptr && npHead == nullptr)
		return nullptr;
	return npHead;
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
bool FileSystem::CompareHigh(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear, bool needptr)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//获取“全集”
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//获取“子集”
	//任何集合包含空集
	if (np2 == nullptr && np1 != nullptr)
	{
		if(needptr) ConnectList(rear, np1);
		ClearList(np1);
		return true;
	}
	//全集为空，但是子集不为空
	else if (np1 == nullptr && np2 != nullptr)
	{
		ClearList(np1);
		ClearList(np2);
		head = nullptr;
		rear = nullptr;
		return false;
	}
	//都是空集
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
			if (needptr)
			{
				ClearList(head);
				head = nullptr;
				rear = nullptr;
			}
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
			if (needptr)
			{
				if (rear->site < 1) rear->next = nullptr;
				else
				{
					rear->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					rear = rear->next;
					rear->next = nullptr;
				}
				rear->site = temp1->site;
			}
			temp1 = temp1->next;
		}
	}
	if (temp1 == nullptr && temp2 != nullptr)
	{
		ClearList(head);
		head = nullptr;
		rear = nullptr;
		ClearList(np1);//清空链表
		ClearList(np2);
		return false;
	}
	if(needptr) ConnectList(rear, temp1);
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
bool FileSystem::CompareLow(int FileValue, int CurrentValue)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//获取“全集”
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//获取“子集”
	//任何集合包含空集
	if (np2 == nullptr && np1 != nullptr)
	{
		ClearList(np1);
		return true;
	}
	else if (np1 == nullptr && np2 != nullptr)
	{
		ClearList(np1);
		ClearList(np2);
		return false;
	}
	else if (np1 == nullptr && np2 == nullptr) return true;
	std::shared_ptr<NEXTPACE> temp2 = np2;
	std::shared_ptr<NEXTPACE> temp1 = np1;
	while (temp1 != nullptr && temp2 != nullptr)
	{
		//若存在全集中没有子集中的元素，则返回false，表示不包含
		if (temp1->site < temp2->site)
		{
			ClearList(np1);//清空链表
			ClearList(np2);
			return false;
		}
		//若子集中的元素也存在于全集中，则遍历下一个元素
		else if (temp1->site == temp2->site)
		{
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
		else if (temp1->site > temp2->site) temp1 = temp1->next;
	}
	if (temp2 != nullptr && temp1 == nullptr)
	{
		ClearList(np1);//清空链表
		ClearList(np2);
		return false;
	}
	ClearList(np1);//清空链表
	ClearList(np2);
	return true;
}