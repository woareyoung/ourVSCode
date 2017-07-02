#include "../FileSystem_Header/FileSystem.h"
///ƥ�䵱ǰ����״���ļ���
std::shared_ptr<NEXTPACE> FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	std::shared_ptr<NEXTPACE> np = nullptr;//��¼����һ����Ӧ�Է����������ͷ���
	std::shared_ptr<NEXTPACE> temp = np;
	int i;
 	SITUATION situa;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//�򿪵�ǰ����״���ļ�¼�ļ�
	situa.Line[0] = StatusQuo.Line[0];
	for (i = 1; i < 10; ++i) situa.Line[i] = StatusQuo.Line[i];
	TempFile.seekg(0);
	while (!TempFile.eof())//ѭ��ֱ���ļ�ĩβ
	{
		for (i = 1; i < 10; ++i)
		{
			TempFile >> value[i];
			if (situa.Line[i] != value[i])//ֻҪ��һ�����ݲ���ͬ����ƥ����һ����¼
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
///ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
std::shared_ptr<NEXTPACE> FileSystem::GenerMatch(SITUATION &StatusQuo, int player, int round)
{
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;
	OpenFile(name, TempFile);
	TempFile.seekg(0);
	int i, j;
	for (i = 0; i < 10; i++) value[i] = 0;
	std::shared_ptr<NEXTPACE> np = std::shared_ptr<NEXTPACE>(new NEXTPACE);//��¼δ����λ�õ������ͷ���
	std::shared_ptr<NEXTPACE> tempHead = np;//��̬ͷ���
	std::shared_ptr<NEXTPACE> tempRear = tempHead;//��̬β���
	tempHead->next = nullptr;
	np->site = 0;
	while (!TempFile.eof())
	{
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			//������ǰ����״��
			if (Compare(value[i] / 10000, StatusQuo.Line[i] / 10000, tempHead, tempRear))
			{
				for (tempRear = tempHead; ; tempRear = tempRear->next)
				{
					tempRear->site += i * 10;//��ÿһ��ֵ���ϡ��кš�
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
				//�������������������
				ClearList(np);
				np = std::shared_ptr<NEXTPACE>(new NEXTPACE);//��¼δ����λ�õ������ͷ���
				tempHead = np;//��̬ͷ���
				tempRear = tempHead;//��̬β���
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
///��ѹ������ֵ��ѹ�����������кŴӴ�С����
std::shared_ptr<NEXTPACE> FileSystem::UnPack(int value)
{
	if (value == 0) return nullptr;
	std::shared_ptr<NEXTPACE> np = nullptr;
	std::shared_ptr<NEXTPACE> temp = np;
	int tempValue = 512;//2��9�η�
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
//�Ƚ�������ֵ���鿴�ļ������Ƿ������ǰ״��
bool FileSystem::Compare(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//��ȡ��ȫ����
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//��ȡ���Ӽ���
	//�κμ��ϰ����ռ�
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
		//������ȫ����û���Ӽ��е�Ԫ�أ��򷵻�false����ʾ������
		if (temp1->site < temp2->site)
		{
			ClearList(np1);//�������
			ClearList(np2);
			ClearList(head);
			head = nullptr;
			rear = nullptr;
			return false;
		}
		//���Ӽ��е�Ԫ��Ҳ������ȫ���У��������һ��Ԫ��
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
	ClearList(np1);//�������
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