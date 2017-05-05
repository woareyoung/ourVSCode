#include "./stdafx.h"
#include "./AI2_Header/AI2.h"

/**
* �ڷ�ģ�����
* #���� O���� ~��ȷ�� !�߽� $��ǰλ�� *��ʾempty
* ÿһ��ģ������������顣
* ���ȼ���	�Է���ʮ��Χɱ
*			������ʮ��Χɱ
*			�Է�����Χɱ
*			����������Χɱ
*			�����ı߽����ӵ�
*/
int patterns_B[] = //BLACK��ģʽ
{
	/*********************************************
	�Է���ʮ��Χɱ������Χɱ���߽�Χɱ������
	��֯�Է�����Χɱ�ǵ�һ���ȼ�
	**********************************************/
	// ��ʼ��־λ,4����Ҫƥ��ĵ㣬����
	PATTERN, 4, 60,
	// ���濴�����Ŀ�����������㷨
	-1, 0,White | Edge, // 			����/��Ե 
	1, 0, White | Edge, //����/��Ե	  ��ǰ	��λ
	0,-1, White | Edge, //			����/��Ե 
	0,1, NoChess,

	/*********************************************
	�ҷ���ʮ��Χɱ������Χɱ���߽�Χɱ������
	�ҷ�����Χɱ���ǵڶ����ȼ�
	����ǲ�1����Χɱ��
	**********************************************/
	PATTERN, 4, 50,
	-1, -1,Black | Edge, //		 ����
	1, -1,Black | Edge,  //	���� ��λ ��ǰ
	0,-1, NoChess,       // 	 ����
	0,-2,Black | Edge,

	/*********************************************
	�߽����ӵ�
	**********************************************/
	PATTERN, 4, 40,
	-1, 0,Edge,		//         �߽�
	0, -1,Edge,		//  �߽� ��ǰλ��  ��λ
	0, 1,NoChess,	//        ��λ
	1, 0,NoChess,	//

	/*********************************************
	�ҷ���ʮ��Χɱ������Χɱ���߽�Χɱ������
	�ҷ�����Χɱ���ǵڶ����ȼ�
	������������Χɱ��(ȱ������Χɱ��)
	**********************************************/
	PATTERN, 4, 30,
	-1, -1,Black | Edge, //		 ����
	1, -1,Black | Edge,  //	��λ ��λ ��ǰ
	0,-1, NoChess,       // 	 ����
	0,-2,NoChess,

	PATTERN, 4, 30,
	-1, -1,Black | Edge, //		 ��λ
	1, -1,NoChess,  	//	���� ��λ ��ǰ
	0,-1, NoChess,       // 	 ����
	0,-2,Black | Edge,

	/*********************************************
	ƥ����ɱ��
	**********************************************/
	PATTERN, 4, minLimit,
	-1, -1,Black | Edge, //		 ����
	1, -1,Black | Edge,  //	���� ���� ��ǰ
	0,-1, White,       	// 	 	����
	0,-2,Black | Edge,

	/*********************************************
	ȱ��
	**********************************************/
	PATTERN, 4, 20,
	-1, -1,NoChess | Edge, 		//		 	��λ
	1, -1,Black,  				//	��λ		��λ 	��ǰ
	0,-1, NoChess | Edge,        //		 	����
	0,-2, NoChess | Edge,

	PATTERN, 4, 20,
	-1, -1,NoChess | Edge, 		//		 	��λ
	1, -1,NoChess | Edge,  		//	���� 	��λ 	��ǰ
	0,-1, NoChess | Edge,        //		 	��λ
	0,-2, Black,

	PATTEND //ģʽ����
};

int patterns_W[] =
{
	PATTERN, 4, 60,
	-1, 0, Black | Edge,
	1, 0, Black | Edge,
	0,-1, Black | Edge,
	0,1, NoChess,


	PATTERN, 4, 50,
	-1, -1,White | Edge,
	1, -1,White | Edge,
	0,-1,NoChess,
	0,-2,White | Edge,

	PATTERN, 4, 40,
	-1, 0,Edge,
	0, -1,Edge,
	0, 1,NoChess,
	1, 0,NoChess,

	PATTERN, 4, 30,
	-1, 0,White | Edge,
	1, 0, White | Edge,
	0,-1, NoChess,
	0,1, NoChess,

	PATTERN, 4, 30,
	-1, -1,White | Edge,
	1, -1,NoChess,
	0,-1, NoChess,
	0,-2,White | Edge,

	PATTERN, 4, minLimit,
	-1, -1,White | Edge,
	1, -1,White | Edge,
	0,-1, Black,
	0,-2,White | Edge,

	PATTERN, 4, 20,
	-1, -1,NoChess | Edge,
	1, -1, White,
	0,-1, NoChess | Edge,
	0,-2, NoChess | Edge,

	PATTERN, 4, 20,
	-1, -1,NoChess | Edge,
	1, -1,NoChess | Edge,
	0,-1, NoChess | Edge,
	0,-2, White,

	PATTEND
};


/**
* [AI2::startPattern ģ��ƥ��]
* @param  patAdd [ģ��]
* @return        [��]
*/
void AI2::startPattern() {
	register int t, j, *is;
	int *patAdd = nullptr;
	if (turn2Who == Black) {
		patAdd = patterns_B;
	}
	else if (turn2Who == White) {
		patAdd = patterns_W;
	}
	int *patterns = patAdd;
	/********************************************************
	��һ���汾     (X, Y)
	********************************************************/
	Pattern(patAdd, 1);

	/********************************************************
	�ڶ����汾��Y�ᷴת (X, -Y)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 2);

	/********************************************************
	�������汾��X�ᷴת (-X, -Y)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			*is = (-*is);
			is += 3;
		}
	}

	Pattern(patAdd, 3);

	/********************************************************
	���ĸ��汾��Y�ᷴת (-X, Y)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 4);

	/********************************************************
	������汾��XY�ύ�� (Y, -X)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			t = *is;
			*is = is[1];
			is[1] = t;
			is += 3;
		}
	}

	Pattern(patAdd, 5);

	/********************************************************
	�������汾��XY�ύ��֮��Y�ᷴת (Y ,X)
	********************************************************/
	/*Invert y coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 6);

	/********************************************************
	���߸��汾��XY�ύ��֮��X�ᷴת (-Y, X)
	********************************************************/
	/*Invert x coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			*is = (-*is);
			is += 3;
		}
	}

	Pattern(patAdd, 7);

	/********************************************************
	�ڰ˸��汾��XY�ύ��֮��Y�ᷴת (-Y, -X)
	********************************************************/
	/*Invert y coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 8);

	/********************************************************
	�ھŸ��汾��-X��-Y�ύ�� (X, Y),�������Ҫ�����ǽ�ģ�廹ԭ
	********************************************************/
	/*Exchange -x and -y coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			t = (-*is);
			*is = (-is[1]);
			is[1] = t;
			is += 3;
		}
	}
}

/**
* [AI2::Pattern ʹ��ģ��ƥ����������]
* @param patAdd [ģ��]
* @param times  [������ûʲô�ã�������¼�ڼ��Σ����㿴����]
*/
void AI2::Pattern(int *patAdd, int times) {
	register int x, y, j;// xy��ƥ�䵽�Ŀ�λ��Jʹ����������ģ������Ľ�����ʶ��
	register int *is, *iis;// ָ�룬����ģ��λ�õ�ָ��
	register int xs, ys;
	Pos emptyPos[3];
	int start = 0;
	int score;
	int *patterns = patAdd;
	// ������һ��ģ��
	for (is = patterns; PATTERN == *is; is += 3 + 3 * (is[1])) //is[1]=*(is+1)
	{
		for (x = 1; x < 10; ++x) {
			for (y = 1; y < 10; ++y)
			{
				if (NoChess == cross[x][y])
				{
					start = 0;
					// ����ģ��ĵ�һ������
					for (iis = is + 1, j = *iis++, score = *iis++; j; --j)//�״�ѭ�� 
					{
						xs = *iis++;
						ys = *iis++;
						// ������������
						if (onboard(x + xs, y + ys)) {
							if (0 == (cross[x + xs][y + ys] & *iis++)) {
								goto mismatch;//����ͬ��
							}
							else
							{
								if (cross[x + xs][y + ys] == NoChess) {
									// ���統ǰ�հ׵�ķ���ֵΪ0��ʱ�򣬾�ֱ������
									// ��Ϊ����Ϊ0��ʾ��ǰ�հ׵��λ���ǵз���ɱ�㣬û��Ҫ���
									if (chessScore[x + xs][y + ys] == 0) {
										goto mismatch;
									}
									else {
										emptyPos[start].line = x + xs;
										emptyPos[start].column = y + ys;
										++start;
									}
								}
							}
						}
						else {
							if (0 == (Edge & *iis++)) goto mismatch;//���Ǳ߽�
						}
					}
					//���ִ�е�����ط����ˣ����Ǿ�ƥ�䵽һ��ģ��
					// ����ƥ�䵽��ģ�壬������Ҫ����ģ�廷�����ж�
					// 1���Ƿ�Χɱ��2���Ƿ�Χɱ����

					/******************************************
					�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
					����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
					*******************************************/
					for (int i = 0; i < start; ++i) {
						// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
						cross[emptyPos[i].line][emptyPos[i].column] = Rival;
						bool flag = isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival);
						if (flag) {
							cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
							// ����ǵз�����ɱ��Ļ������������   -.-������
							chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
							goto mismatch;
						}
						// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
						cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
					}
					/******************************************
					������һ���߼���Ҫ����
					����һ���㱻ƥ�䵽��εĻ�����δ���
					1�������ۼӷֵķ������д���
					2����Ҫע������ӵ��Ƿ��������
					*******************************************/
					_cprintf("**************match up***********\n");
					// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
					// �����ǰλ�ò�Ϊ�յĻ�����ֱ��������
					if (cross[x][y] != NoChess) {
						goto mismatch;
					}
					// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
					cross[x][y] = turn2Who;
					if (isGo2Dead(x, y, turn2Who)) {
						chessScore[x][y] = minLimit;
						cross[x][y] = NoChess;
						// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
						goto mismatch;
					}
					// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
					cross[x][y] = Rival;
					if (isGo2Dead(x, y, Rival)) {
						cross[x][y] = NoChess;
						// ����ǵз�����ɱ��Ļ������������   -.-������
						chessScore[x][y] = 0;
						goto mismatch;
					}
					// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
					cross[x][y] = NoChess;
					_cprintf("add score = %d\n", score);
					chessScore[x][y] += score;// ����ƥ�䵽��һ��ģ�壬���ģ���λ�þ������
				}
			mismatch:
				;
			}
		}
	}
}