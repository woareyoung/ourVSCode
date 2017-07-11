#include "../FileSystem_Header/FileSystem.h"
///ƥ�䵱ǰ����״���ļ���
int FileSystem::Match(SITUATION &StatusQuo, std::set<int> &result, int round, bool isWinner)
{
	int i;
	int temp;
	SITUATION situa;
	if (!isWinner) for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	else situa = StatusQuo;
	int count = 0;
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
				TempFile >> temp;
				result.insert(temp);
				TempFile >> count;
			}
		}
	}
	TempFile.close();
	return count;
}
///ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
int FileSystem::GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round, bool needptr)
{
	int count = 0;
	std::string name = FN.ForeName + std::to_string(round) + FN.TXT;//�ļ���
	OpenFile(name, TempFile);//���ļ�
	TempFile.seekg(0);
	int i, one, two;//one��two������һ���߳�
	int NoWant;//���ڽ���û�õ�����
	bool HighSimilar = false, LowSimilar = false;
	bool wait = true;
	for (i = 0; i < 10; i++) value[i] = 0;
	std::set<int> Child;//������
	std::set<int> Temp;//��ʱ����
	std::set<int> AssistSTL;//��������
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
				LowSimilar = CompareLow(one % 10000, two % 10000);//��λ�Ƿ����
				wait = false;
			});
			Temp.clear();//���������
			HighSimilar = CompareHigh(value[i] / 10000, StatusQuo.Line[i] / 10000, Temp, needptr);//�жϸ�λ�Ƿ����
			//������ݲ���0�������ʱ�������ݼ��뵽������
			if (value[i] != 0)
				for (auto a : Temp) Child.insert(a);
			//���������0�������������
			else Child.clear();
			while (wait) {}
			if (value[i] == 0 && i < 9) continue;
			//������ǰ����״��
			if (HighSimilar && LowSimilar)
			{
				//Ϊÿһ�����ݼ����кţ����浽����������
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
			//��������ǰ״�����������
			else
			{
				Child.clear();
				AssistSTL.clear();
			}
			for (; i < 11; i++) TempFile >> NoWant;
			break;
		}
		//�Ѹ���������ӵ�������
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
bool FileSystem::CompareHigh(int FileValue, int CurrentValue, std::set<int> &stl, bool needptr)
{
	std::shared_ptr<NEXTPACE> np1 = UnPack(FileValue);//��ȡ��ȫ����
	std::shared_ptr<NEXTPACE> np2 = UnPack(CurrentValue);//��ȡ���Ӽ���
	//�κμ��ϰ����ռ�
	if (np2 == nullptr && np1 != nullptr)
	{
		if(needptr) ConnectList(stl, np1);
		ClearList(np1);
		return true;
	}
	//ȫ��Ϊ�գ������Ӽ���Ϊ��
	else if (np1 == nullptr && np2 != nullptr)
	{
		ClearList(np1);
		ClearList(np2);
		return false;
	}
	//���ǿռ�
	else if (np1 == nullptr && np2 == nullptr) return true;
	std::shared_ptr<NEXTPACE> temp2 = np2;
	std::shared_ptr<NEXTPACE> temp1 = np1;
	while(temp1 != nullptr && temp2 != nullptr)
	{
		//������ȫ����û���Ӽ��е�Ԫ�أ��򷵻�false����ʾ������
		if (temp1->site < temp2->site)
		{
			ClearList(np1);//�������
			ClearList(np2);
			if (needptr) stl.clear();
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
			if (needptr) stl.insert(temp1->site);
			temp1 = temp1->next;
		}
	}
	if (temp1 == nullptr && temp2 != nullptr)
	{
		stl.clear();
		ClearList(np1);//�������
		ClearList(np2);
		return false;
	}
	if(needptr) ConnectList(stl, temp1);
	ClearList(np1);//�������
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