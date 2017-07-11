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
	int j;
	for (j = 0; j < ProThreadNumber; j++)
	{
		ProThread[j] = ttt;
		ttt = ttt->next;
		if (ttt == nullptr)
		{
			j++;
			break;
		}
	}
	bool wait[ProThreadNumber - 1] = { true };//����Ƿ�һ��
	bool DOloop = true;
	for (int i = 1; i < ProThreadNumber && i < j; i++)
	{
		std::async(std::launch::async, [&]() {
			while (ProThread[i] != nullptr)
			{
				//�ж������Ƿ���ͬ
				for (int n = 1; n < 10; n++)
				{
					if (CompareHigh(ProThread[i]->Value[n] / 10000, SIT[i].Line[n] / 10000, NoWant, false) && CompareLow(ProThread[i]->Value[n] % 10000, SIT[i].Line[n] % 10000))
						break;//����в����������ݣ���������һ�����
					else if (n == 9) Count[i] += ProThread[i]->count;
				}
				ProThread[i] = ProThread[i]->JumpPtr;
			}
			wait[i - 1] = false;
		});
	}
	while (ProThread[0] != nullptr)
	{
		//�ж������Ƿ���ͬ
		for (int n = 1; n < 10; n++)
		{
			if (CompareHigh(ProThread[0]->Value[n] / 10000, SIT[0].Line[n] / 10000, NoWant, false) && CompareLow(ProThread[0]->Value[n] % 10000, SIT[0].Line[n] % 10000))
				break;//����в���ͬ�����ݣ���������һ�����
			else if (n == 9) Count[0] += ProThread[0]->count;
		}
		ProThread[0] = ProThread[0]->JumpPtr;
	}
	while (DOloop)
	{
		for (int m = 0; m < ProThreadNumber - 1 && m < j; m++)
		{
			if (wait[m]) break;
			else if (m == ProThreadNumber - 2 || m == j - 1) DOloop = false;
		}
	}
	for (int i = 0; i < ProThreadNumber && i < j; i++) SumCount += Count[i];
	return SumCount;
}
void FileSystem::ReadFileToMemory(int NowRound)
{
	std::string winfilename = FN.ForeName + std::to_string(NowRound) + FN.TXT;
	std::string losefilename = FN.ForeName + std::to_string(NowRound + 1) + FN.TXT;
	bool wait = true;
	//��ȡ���¼
	std::async(std::launch::async, [&]() {
		ReadFile(0, losefilename);
		wait = false;
	});
	//��ȡӮ��¼
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
	int i, num;
	for (num = 0; num < ProThreadNumber; num++)
	{
		temp[num] = tem;
		tem = tem->next;
		if (tem == nullptr)
		{
			num++;
			break;
		}
	}
	bool wait[ProThreadNumber - 1];
	for (i = 0; i < ProThreadNumber - 1; i++) wait[i] = true;
	for (i = 0; i < ProThreadNumber - 1 && i < num - 1; i++)
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
	std::shared_ptr<DISKSTATUS> t = temp[ProThreadNumber > num ? num - 1 : ProThreadNumber - 1];
	while (t != nullptr)
	{
		tem = t;
		t = t->JumpPtr;
		tem = nullptr;
	}
	bool www = false;
	while (true)
	{
		for (int j = 0; j < ProThreadNumber - 1 && j < num - 2; j++)
		{
			if (wait[j] == true)
			{
				www = false;
				break;
			}
			else if (j == ProThreadNumber - 2 || j == num - 3) www = true;
		}
		if (www) break;
	}
}