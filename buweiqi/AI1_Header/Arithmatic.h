#pragma once
#ifndef ARITHMATIC_H_INCLUDED
#define ARITHMATIC_H_INCLUDED
///���������
struct SPECIALPOINTAMOUNT
{
	int Chip;//ȱ������
	int TigerMouth;//��������
	int Eye;//������
};
///��¼������ʷ����
struct STEP
{
	double ScoreRate;//��λ�õķ�ֵ�ٷֱȣ�������ֵ�ı�ֵ��
	int Number;//�ڼ���
	STEP *next;//ָ����һ��
	STEP *pre;//ָ����һ��
};
///��¼Ԥ�⵽�Ŀ���������
struct CHAIN
{
	///Ԥ�������λ��
	int line;
	int column;

	CHAIN *next;//��һ��
	CHAIN *parallel;//��ǰ�غϵ�����������
};
///������ֵ������Ԥ�⣩
enum POINTSCORE
{
	Chip = 1,
	TigerMouth = 3,
	Eye = 5
};
///�㷨֧��
class Arithmatic
{
public:
	int StepNumber;//��¼�������˶��ٲ�
	int MaxStepNumber;//���¹���MaxStepNumber���󹹽��ع鷽��
	int MaxParallel;//ÿһ��Ԥ��Ŀ����Ե����ֵ
	int MaxNext;//����������
	double a;//�ع鷽�̳�����
	double b;//�ع鷽��б��
	double _x;//xƽ��ֵ��xָ���ڼ���
	double _y;//yƽ��ֵ��yָ��ĳһ������λ�õķ�ֵ
	double AttackPerformance;//��¼���ֵĽ������ܣ���ֵΪ1��
	double BoundryRate;//������ޱ�������ֵΪ1��
	double CorrectRate;//����ϵ��
	SPECIALPOINTAMOUNT spa[3];//�±�0û�ã�1��2��Ӧ���1�����2
	POINTSCORE PoS;//0��ʾȱ�ڡ�1��ʾ���ڡ�2��ʾ��
	STEP *StepHead;//��̬��¼ͷ���
	STEP *SSS;//��̬β����¼
	CHAIN *HeadChain;//��̬ͷ���
	CHAIN *TailParallel;//ƽ������β���
};

#endif // ARITHMATIC_H_INCLUDED
