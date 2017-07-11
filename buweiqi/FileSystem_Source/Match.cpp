#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
int FileSystem::Match(SITUATION &StatusQuo, std::set<int> &result, int round, bool isWinner)
{
	int i;
	int temp;
	SITUATION situa;
	if (!isWinner) for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	else situa = StatusQuo;
	int count = 0;
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
				TempFile >> temp;
				result.insert(temp);
				TempFile >> count;
			}
		}
	}
	TempFile.close();
	return count;
}
///匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
int FileSystem::GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round, bool needptr)
{
	int count = 0;
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;//文件名
	OpenFile(name, TempFile);//打开文件
	TempFile.seekg(0);
	int i, one, two;//one与two用于另一条线程
	int NoWant;//用于接收没用的数据
	bool HighSimilar = false, LowSimilar = false;
	bool wait = true;
	for (i = 0; i < 10; i++) value[i] = 0;
	std::set<int> Child;//子容器
	std::set<int> Temp;//临时容器
	std::set<int> AssistSTL;//辅助容器
	while (!TempFile.eof())
	{
		HighSimilar = false;
		LowSimilar = false;
		wait = true;
		Child.clear();
		AssistSTL.clear();
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
			Temp.clear();//先清空再用
			HighSimilar = CompareHigh(value[i] / 10000, StatusQuo.Line[i] / 10000, Temp, needptr);//判断高位是否包含
			//如果数据不是0，则把临时容器内容加入到子容器
			if (value[i] != 0)
				for (auto a : Temp) Child.insert(a);
			//如果数据是0，则清空子容器
			else Child.clear();
			while (wait) {}
			if (value[i] == 0 && i < 9) continue;
			//包含当前盘面状况
			if (HighSimilar && LowSimilar)
			{
				//为每一个数据加上行号，并存到辅助容器中
				for (auto t : Child)
				{
					NoWant = t + 10 * i;
					AssistSTL.insert(NoWant);
				}
				if (i == 9)
				{
					TempFile >> i >> i;
					count += i;
					break;
				}
				continue;
			}
			//不包含当前状况，清空容器
			else
			{
				Child.clear();
				AssistSTL.clear();
			}
			for (; i < 11; i++) TempFile >> NoWant;
			break;
		}
		//把辅助容器添加到父容器
		if (!Child.empty())
		{
			for (auto t : AssistSTL)
				if(t > 0) Parent.insert(t);
			AssistSTL.clear();
		}
	}
	TempFile.close();
	return count;
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
bool FileSystem::CompareHigh(int FileValue, int CurrentValue, std::set<int> &stl, bool needptr)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//获取“全集”
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//获取“子集”
	//任何集合包含空集
	if (np2 == nullptr && np1 != nullptr)
	{
		if(needptr) ConnectList(stl, np1);
		ClearList(np1);
		return true;
	}
	//全集为空，但是子集不为空
	else if (np1 == nullptr && np2 != nullptr)
	{
		ClearList(np1);
		ClearList(np2);
		return false;
	}
	//都是空集
	else if (np1 == nullptr && np2 == nullptr) return true;
	std::shared_ptr<NEXTPACE> temp2 = np2;
	std::shared_ptr<NEXTPACE> temp1 = np1;
	while(temp1 != nullptr && temp2 != nullptr)
	{
		//若存在全集中没有子集中的元素，则返回false，表示不包含
		if (temp1->site < temp2->site)
		{
			ClearList(np1);//清空链表
			ClearList(np2);
			if (needptr) stl.clear();
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
			if (needptr) stl.insert(temp1->site);
			temp1 = temp1->next;
		}
	}
	if (temp1 == nullptr && temp2 != nullptr)
	{
		stl.clear();
		ClearList(np1);//清空链表
		ClearList(np2);
		return false;
	}
	if(needptr) ConnectList(stl, temp1);
	ClearList(np1);//清空链表
	ClearList(np2);
	return true;
}
void FileSystem::ConnectList(std::set<int> &Aim, std::shared_ptr<NEXTPACE> Source)
{
	while (Source != nullptr)
	{
		Aim.insert(Source->site);
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