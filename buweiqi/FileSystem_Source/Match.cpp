#include "../FileSystem_Header/FileSystem.h"
///ƥ�䵱ǰ����״���ļ���
std::shared_ptr<NEXTPACE> FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	std::shared_ptr<NEXTPACE> np = NULL;//��¼����һ����Ӧ�Է����������ͷ���
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
///ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
std::shared_ptr<NEXTPACE> FileSystem::GenerMatch(SITUATION &StatusQuo, int player, int round)
{
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;
	OpenFile(name, TempFile);
	TempFile.seekg(0);
	int i, j;
	for (i = 0; i < 10; i++) value[i] = 0;
	std::shared_ptr<NEXTPACE> np = std::shared_ptr<NEXTPACE>(new NEXTPACE);//��¼δ����λ�õ������ͷ���
	np->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);//����������������Ƿ���Ч
	std::shared_ptr<NEXTPACE> tempHead = np->next;//��̬ͷ���
	std::shared_ptr<NEXTPACE> tempRear = tempHead;//��̬β���
	tempHead->site = 0;
	tempHead->next = NULL;
	np->site = 0;
	while (!TempFile.eof())
	{
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			//������ǰ����״��
			if (Compare(value[i] / 10000, StatusQuo.Line[i] / 10000, tempHead, tempRear))
			{
				if (np->site == 0)
				{
					//��һ����㲻Ҫ��
					// delete np;
					np = nullptr;
					np = tempHead;
				}
				for (tempRear = tempHead; ; tempRear = tempRear->next)
				{
					tempRear->site += i * 10;//��ÿһ��ֵ���ϡ��кš�
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
				//����ֻ��һ����㣨����������ʼ��٣�
				if (tempHead == NULL)
				{
					np->site = 0;
					np->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					tempHead = np->next;
					tempHead->next = nullptr;
					tempRear = tempHead;
					tempHead->site = 0;
				}
				//����ֹһ������ҵڶ������ֵ����0����Ϊ��Ч����
				else if (tempHead->site > 0)
				{
					ClearList(tempHead);//�ӵڶ�����㿪ʼ�������
					np->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
					tempHead = np->next;
					tempHead->site = 0;
					tempRear = tempHead;
					np->site = 0;
					np->next = nullptr;
				}
				//������Ȼ��Ч
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
///��ѹ������ֵ��ѹ�����������кŴӴ�С����
std::shared_ptr<NEXTPACE> FileSystem::UnPack(int value)
{
	if (value == 0) return NULL;
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
//�Ƚ�������ֵ���鿴�ļ������Ƿ������ǰ״��
bool FileSystem::Compare(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear)
{
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//��ȡ���Ӽ���
	if (np2 == NULL) return true;//�κμ��ϰ����ռ�
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//��ȡ��ȫ����
	if (np1 == NULL) return false;
	std::shared_ptr<NEXTPACE> temp2 = np2;
	std::shared_ptr<NEXTPACE> temp1 = np1;
	while(temp1 != NULL && temp2 != NULL)
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
	while (temp1 != NULL)
	{
		rear->next = std::shared_ptr<NEXTPACE>(new NEXTPACE);
		rear = rear->next;
		rear->next = nullptr;
		rear->site = temp1->site;
		temp1 = temp1->next;
	}
	ClearList(np1);//�������
	ClearList(np2);
	return true;
}
