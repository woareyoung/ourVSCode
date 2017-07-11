#include "../FileSystem_Header/FileSystem.h"

int FileSystem::CountNumber(SITUATION &sit, bool win)
{
	int Count[ProThreadNumber] = { 0 };
	int SumCount = 0;
	std::shared_ptr<DISKSTATUS> ttt;
	std::set<int> NoWant;
	if (win) ttt = ProHeadWin;
	else ttt = ProHeadLose;
	SITUATION SIT[ProThreadNumber] = { sit };
	std::shared_ptr<DISKSTATUS> ProThread[ProThreadNumber];
	for (int j = 0; j < ProThreadNumber; j++)
	{
		ProThread[j] = ttt;
		ttt = ttt->next;
	}
	bool wait[ProThreadNumber - 1] = { true };//标记是否一样
	bool DOloop = true;
	for (int i = 1; i < ProThreadNumber; i++)
	{
		std::async(std::launch::async, [&]() {
			while (ProThread[i] != nullptr)
			{
				//判断盘面是否相同
				for (int n = 1; n < 10; n++)
				{
					if (CompareHigh(ProThread[i]->Value[n] / 10000, SIT[i].Line[n] / 10000, NoWant, false) && CompareLow(ProThread[i]->Value[n] % 10000, SIT[i].Line[n] % 10000))
						break;//如果有不包含的数据，则搜索下一个结点
					else if (n == 9) Count[i] += ProThread[i]->count;
				}
				ProThread[i] = ProThread[i]->JumpPtr;
			}
			wait[i - 1] = false;
		});
	}
	while (ProThread[0] != nullptr)
	{
		//判断盘面是否相同
		for (int n = 1; n < 10; n++)
		{
			if (CompareHigh(ProThread[0]->Value[n] / 10000, SIT[0].Line[n] / 10000, NoWant, false) && CompareLow(ProThread[0]->Value[n] % 10000, SIT[0].Line[n] % 10000))
				break;//如果有不相同的数据，则搜索下一个结点
			else if (n == 9) Count[0] += ProThread[0]->count;
		}
		ProThread[0] = ProThread[0]->JumpPtr;
	}
	while (DOloop)
	{
		for (int m = 0; m < ProThreadNumber - 1; m++)
		{
			if (wait[m]) break;
			else if (m == ProThreadNumber - 2) DOloop = false;
		}
	}
	for (int i = 0; i < ProThreadNumber; i++) SumCount += Count[i];
	return SumCount;
}
void FileSystem::ReadFileToMemory(int NowRound)
{
	std::string winfilename = FN.ForeName + std::to_string(NowRound) + FN.TXT;
	std::string losefilename = FN.ForeName + std::to_string(NowRound + 1) + FN.TXT;
	bool wait = true;
	//读取输记录
	std::async(std::launch::async, [&]() {
		ReadFile(0, losefilename);
		wait = false;
	});
	//读取赢记录
	ReadFile(1, winfilename);
	while (wait) {}
}
void FileSystem::ReadFile(int HeadNumber, std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios::out | std::ios::in);
	if (!file)
	{
		if(HeadNumber == 0) ProHeadLose = nullptr;
		else ProHeadWin = nullptr;
		return;
	}
	int i, num = 0;
	std::shared_ptr<DISKSTATUS> tempTail = HeadNumber == 0 ? ProHeadLose : ProHeadWin;
	std::shared_ptr<DISKSTATUS> temp;
	while (!file.eof())
	{
		if (tempTail == nullptr)
		{
			tempTail = std::shared_ptr<DISKSTATUS>(new DISKSTATUS);
			tempTail->next = nullptr;
			tempTail->prior = nullptr;
			tempTail->JumpPtr = nullptr;
			if (HeadNumber == 0) ProHeadLose = tempTail;
			else ProHeadWin = tempTail;
		}
		else
		{
			tempTail->next = std::shared_ptr<DISKSTATUS>(new DISKSTATUS);
			tempTail->next->prior = tempTail;
			tempTail = tempTail->next;
			tempTail->next = nullptr;
			tempTail->JumpPtr = nullptr;
		}
		num++;
		if (num > ProThreadNumber)
		{
			for (i = 0, temp = tempTail; i < ProThreadNumber; i++) temp = temp->prior;
			temp->JumpPtr = tempTail;
		}
		for (i = 1; i < 10; i++) file >> tempTail->Value[i];
		file >> tempTail->Value[0] >> tempTail->count;
	}
}

void FileSystem::ClearLIST(std::shared_ptr<DISKSTATUS> Head)
{
	if (Head == nullptr) return;
	std::shared_ptr<DISKSTATUS> temp[ProThreadNumber], tem = Head;
	int i;
	for (i = 0; i < ProThreadNumber; i++)
	{
		temp[i] = tem;
		tem = tem->next;
	}
	bool wait[ProThreadNumber - 1] = { true };
	for (i = 0; i < ProThreadNumber - 1; i++)
	{
		std::shared_ptr<DISKSTATUS> t = temp[i];
		std::async(std::launch::async, [&]() {
			while (t != nullptr)
			{
				tem = t;
				t = t->JumpPtr;
				tem = nullptr;
			}
			wait[i] = false;
		});
	}
	std::shared_ptr<DISKSTATUS> t = temp[ProThreadNumber - 1];
	while (t != nullptr)
	{
		tem = t;
		t = t->JumpPtr;
		tem = nullptr;
	}
	bool www = false;
	while (true)
	{
		for (int j = 0; j < ProThreadNumber - 1; j++)
		{
			if (wait[j] == true)
			{
				www = false;
				break;
			}
			else if (j == ProThreadNumber - 1) www = true;
		}
		if (www) break;
	}
}