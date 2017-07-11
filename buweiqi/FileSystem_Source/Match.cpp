#include "../FileSystem_Header/FileSystem.h"
///ƥ�䵱ǰ����״���ļ���
std::shared_ptr<NEXTPACE> FileSystem::Match(SITUATION &StatusQuo, int &count, int round, bool isWinner)
{
	std::shared_ptr<NEXTPACE> np = nullptr;//��¼����һ����Ӧ�Է����������ͷ���
	std::shared_ptr<NEXTPACE> temp = np;
	std::shared_ptr<NEXTPACE> last = nullptr;
	int i;
	SITUATION situa;
	if (!isWinner) for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	else situa = StatusQuo;
	count = 0;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//�򿪵�ǰ����״���ļ�¼�ļ�
	TempFile.seekg(0);
	while (!TempFile.eof())//ѭ��ֱ���ļ�ĩβ
	{
		for (i = 1; i < 10; ++i)
		{
			TempFile >> value[i];
			if (situa.Line[i] != value[i])//ֻҪ��һ�����ݲ���ͬ����ƥ����һ����¼
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
///ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
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
	std::shared_ptr<NEXTPACE> npHead = nullptr;//��¼δ����λ�õ������ͷ���
	std::shared_ptr<NEXTPACE> npRear = nullptr;
	std::shared_ptr<NEXTPACE> np = nullptr;/*����*/
	std::shared_ptr<NEXTPACE> tempHead = nullptr;//��̬ͷ���
	std::shared_ptr<NEXTPACE> tempH = nullptr;/*����*/
	std::shared_ptr<NEXTPACE> tempRear = nullptr;//��̬β���
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
				LowSimilar = CompareLow(one % 10000, two % 10000);//��λ�Ƿ����
				wait = false;
			});
			tempH = std::shared_ptr<NEXTPACE>(new NEXTPACE);
			tempRear = tempH;
			HighSimilar = CompareHigh(value[i] / 10000, StatusQuo.Line[i] / 10000, tempH, tempRear, needptr);//�жϸ�λ�Ƿ����
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
			//-------------------������ƴ�ӵ������������أ�---------------------//
			//���������0�������½����
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
			//-------------------������ƴ�ӵ������������أ�---------------------//
			else
			{
				ClearList(tempH);
				tempH = nullptr;
				tempRear = nullptr;
				if(tempHead != nullptr) tempHead->next = nullptr;
			}
			while (wait) {}
			if (value[i] == 0 && i < 9) continue;
			//������ǰ����״��
			if (HighSimilar && LowSimilar)
			{
				for (tempRear = tempHead; needptr == true && value[i] != 0 && tempRear != nullptr; tempRear = tempRear->next)
				{
					tempRear->site += i * 10;//��ÿһ��ֵ���ϡ��кš�
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
			//��������ǰ״��
			else if(needptr)
			{
				//�������������������
				ClearList(np);
				np = nullptr;
				tempHead = nullptr;
				tempRear = nullptr;
			}
			for (; i < 11; i++) TempFile >> NoWant;
			break;
		}
		//-------------------------���������ز�ƴ�ӵ�����------------------------------//
		if (np != nullptr)
		{
			//�������Ϊ�գ��򴴽�һ�����
			if (npHead == nullptr)
			{
				npHead = np;
				npRear = npHead;
			}
			//��������
			for (tempH = np->next; tempH != nullptr;)
			{
				np = tempH;
				//����������������
				if (tempH->site < 10)
				{
					tempH = tempH->next;//��һ�����
					np = nullptr;//�ͷ��ڴ�
					continue;
				}
				wait = false;//�����û���ظ�
				//��������
				for (tempRear = npHead; tempRear != nullptr;)
				{
					//������ͬ������
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
				if (wait) continue;//���ظ���������������һ�����
				//û���ظ����ͽ������뵽����ĩβ��ȥ
				tempRear->next = tempH;
				tempRear = tempRear->next;
			}
			ClearList(np);
			np = nullptr;//���������
			npRear->next = nullptr;
		}
		//-------------------------������ƴ�ӵ�����------------------------------//
	}
	TempFile.close();
	if (needptr && npHead == nullptr)
		return nullptr;
	return npHead;
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
bool FileSystem::CompareHigh(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear, bool needptr)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//��ȡ��ȫ����
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//��ȡ���Ӽ���
	//�κμ��ϰ����ռ�
	if (np2 == nullptr && np1 != nullptr)
	{
		if(needptr) ConnectList(rear, np1);
		ClearList(np1);
		return true;
	}
	//ȫ��Ϊ�գ������Ӽ���Ϊ��
	else if (np1 == nullptr && np2 != nullptr)
	{
		ClearList(np1);
		ClearList(np2);
		head = nullptr;
		rear = nullptr;
		return false;
	}
	//���ǿռ�
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
			if (needptr)
			{
				ClearList(head);
				head = nullptr;
				rear = nullptr;
			}
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
		ClearList(np1);//�������
		ClearList(np2);
		return false;
	}
	if(needptr) ConnectList(rear, temp1);
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
bool FileSystem::CompareLow(int FileValue, int CurrentValue)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//��ȡ��ȫ����
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//��ȡ���Ӽ���
	//�κμ��ϰ����ռ�
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
		//������ȫ����û���Ӽ��е�Ԫ�أ��򷵻�false����ʾ������
		if (temp1->site < temp2->site)
		{
			ClearList(np1);//�������
			ClearList(np2);
			return false;
		}
		//���Ӽ��е�Ԫ��Ҳ������ȫ���У��������һ��Ԫ��
		else if (temp1->site == temp2->site)
		{
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
		else if (temp1->site > temp2->site) temp1 = temp1->next;
	}
	if (temp2 != nullptr && temp1 == nullptr)
	{
		ClearList(np1);//�������
		ClearList(np2);
		return false;
	}
	ClearList(np1);//�������
	ClearList(np2);
	return true;
}