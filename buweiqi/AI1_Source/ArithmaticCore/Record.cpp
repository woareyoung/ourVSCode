#include "../../stdafx.h"
#include "../../AI1_Header/AI1.h"
///���Է�����λ�ü�����ʷ��¼
void AI1::RecordStep(int line, int column)
{
	if (line == 0 && column == 0) return;
	SSS->ScoreRate = Score[line][column] / MaxScore;//��¼��ֵ
	STEP *S = new STEP;//�������
	SSS->next = S;
	S->Number = SSS->Number + 1;//���ý���һ���Ĳ���
	SSS = S;//��¼��̬β���
}
