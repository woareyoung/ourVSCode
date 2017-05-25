#include "../AI2_Header/AI2.h"
#include <algorithm>

// ƥ��ģʽ
int pattern_Total = 7;
// ƥ��Ĵ���
int pattern_Sum = 28;
// ģʽ�����߶�
int pattern_Score[] = { 60,50,40,30,30,20,20 };
// ģʽ���ж����ӵ���
int pattern_Count[] = { 4, 4, 4, 4, 4, 4, 4 };
// ���������뿴��������㷨
DIRECTION pattern_Background[] = {
	{ -1, 0 },	{ 1,0 },	{ 0,-1 },	{ 0,1 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, 0 },	{ 0, -1 },	{ 0, 1 },	{ 1, 0 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 }
};
// ������������ӵ�
// ƥ��ģʽ�����ӷֲ�
int pattern_White[] = {
	/*********************************************
	�Է���ʮ��Χɱ������Χɱ���߽�Χɱ������
	��֯�Է�����Χɱ�ǵ�һ���ȼ�
	**********************************************/
	Black | Edge,	Black | Edge,	Black | Edge,	NoChess,
	/*********************************************
	�ҷ���ʮ��Χɱ������Χɱ���߽�Χɱ������
	�ҷ�����Χɱ���ǵڶ����ȼ�
	����ǲ�1����Χɱ��
	**********************************************/
	White | Edge,	White | Edge,	NoChess,		White | Edge,
	/*********************************************
	�߽����ӵ�
	**********************************************/
	Edge,			Edge,			NoChess,		NoChess,
	/*********************************************
	�ҷ���ʮ��Χɱ������Χɱ���߽�Χɱ������
	�ҷ�����Χɱ���ǵڶ����ȼ�
	������������Χɱ��(ȱ������Χɱ��)
	**********************************************/
	White | Edge,	White | Edge,	NoChess,		NoChess,
	White | Edge,	NoChess,		NoChess,		White | Edge,
	/*********************************************
	ȱ��
	**********************************************/
	NoChess | Edge, White,			NoChess | Edge,	NoChess | Edge,
	NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White
};
// ƥ��ģʽ�����ӷֲ�
int pattern_Black[] = {
	White | Edge,	White | Edge,	White | Edge,	NoChess,
	Black | Edge,	Black | Edge,	NoChess,		Black | Edge,
	Edge,			Edge,			NoChess,		NoChess,
	Black | Edge,	Black | Edge,	NoChess,		NoChess,
	Black | Edge,	NoChess,		NoChess,		Black | Edge,
	NoChess | Edge, Black,			NoChess | Edge,	NoChess | Edge,
	NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black
};
int* Type[] = { pattern_Black,pattern_White };

/**
* [AI2::reverse ����ת]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse(DIRECTION *PatternType) {
	;
}

/**
* [AI2::reverseXY XY����λ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverseXY(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; i++) {
		temp = (*PatternType).x_offset;
		(*PatternType).x_offset = (*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}

/**
* [AI2::reverse_Y Yȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_Y(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; i++) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
}

/**
* [AI2::reverse_X Xȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_X(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; i++) {
		(*PatternType).x_offset = -(*PatternType).x_offset;
		++PatternType;
	}
}

/**
* [AI2::reverse_X_Y XYȡ���󽻻�]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_X_Y(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; i++) {
		temp = -(*PatternType).x_offset;
		(*PatternType).x_offset = -(*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}
/**
* [AI2::startPattern ģ��ƥ��]
* @return        [��]
*/
void AI2::startPattern() {
	int *PatternType = Type[turn2Who - 1];
	(this->*Reverse[1])(pattern_Background);//��һ���汾 (X, Y)
	Pattern(PatternType);
	(this->*Reverse[2])(pattern_Background);//�ڶ����汾��Y�ᷴת (X, -Y)
	Pattern(PatternType);
	(this->*Reverse[3])(pattern_Background);//�������汾��X�ᷴת(-X, -Y)
	Pattern(PatternType);
	(this->*Reverse[4])(pattern_Background);// ���ĸ��汾��Y�ᷴת(-X, Y)
	Pattern(PatternType);
	(this->*Reverse[5])(pattern_Background);// ������汾��XY�ύ�� (Y, -X)
	Pattern(PatternType);
	(this->*Reverse[6])(pattern_Background);// �������汾��XY�ύ��֮��Y�ᷴת (Y ,X)
	Pattern(PatternType);
	(this->*Reverse[7])(pattern_Background);// ���߸��汾��XY�ύ��֮��X�ᷴת (-Y, X)
	Pattern(PatternType);
	(this->*Reverse[8])(pattern_Background);// �ڰ˸��汾��XY�ύ��֮��Y�ᷴת (-Y, -X)
	Pattern(PatternType);
	(this->*Reverse[9])(pattern_Background);// �ھŸ��汾��-X��-Y�ύ�� (X, Y),�������Ҫ�����ǽ�ģ�廹ԭ
}

/**
* [AI2::Pattern ʹ��ģ��ƥ����������]
* @param PatternType [ģ��]
*/
void AI2::Pattern(int *PatternType) {
	register int x_offset, y_offset;
	register int i, j, x, y;
	register int pointer, start = 0, score;
	register int *pattern = PatternType;
	int mainColor = NoChess;
	Pos emptyPos[3];
	// ÿһ�����Ӷ�Ҫ����һ��ģ��
	for (x = 1; x < 10; ++x) {
		for (y = 1; y < 10; ++y)
		{
			pointer = 0;
			for (i = 0; i < pattern_Total; i++)
			{
				j = pointer;
				pointer += pattern_Count[i];
				if (NoChess != cross[x][y]) continue;// ������ǿ����ӵ����ת��һ��ѭ��
				start = 0;
				for (; j < pointer; ++j)
				{
					x_offset = pattern_Background[j].x_offset;
					y_offset = pattern_Background[j].y_offset;
					score = pattern_Score[i];
					// ������������
					if (OnChessBoard(x + x_offset, y + y_offset)) {
						if (0 == (cross[x + x_offset][y + y_offset] & pattern[j])) goto mismatch;//����ͬ��
						else
						{
							if (cross[x + x_offset][y + y_offset] == NoChess) {
								// ���統ǰ�հ׵�ķ���ֵΪ0��ʱ�򣬾�ֱ������
								// ��Ϊ����Ϊ0��ʾ��ǰ�հ׵��λ���ǵз���ɱ�㣬û��Ҫ���
								if (chessScore[x + x_offset][y + y_offset] == 0) goto mismatch;
								else {
									emptyPos[start].line = x + x_offset;
									emptyPos[start++].column = y + y_offset;
								}
							}
							else mainColor = cross[x + x_offset][y + y_offset];
						}
					}
					else {
						if (0 == (Edge & pattern[j])) goto mismatch;//���Ǳ߽�
					}
				}
				// ƥ�䵽һ��ģ��
				// ����ƥ�䵽��ģ�壬������Ҫ����ģ�廷�����ж�
				// 1���Ƿ�Χɱ��2���Ƿ�Χɱ����

				/******************************************
				�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
				����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
				*******************************************/
				// �����ǰλ�ò�Ϊ�յĻ�����ֱ��������
				if (cross[x][y] != NoChess) goto mismatch;
				for (int i = 0; i < start; ++i) {
					if (mainColor == Rival) {
						// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
						cross[emptyPos[i].line][emptyPos[i].column] = Rival;
						if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
							cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
							// ����ǵз�����ɱ��Ļ������������   -.-������
							chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
							goto mismatch;
						}
					}
					else if (mainColor == turn2Who) {
						// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
						cross[x][y] = turn2Who;
						if (isGo2Dead(x, y, turn2Who)) {
							chessScore[x][y] = minLimit;
							cross[x][y] = NoChess;
							// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
							goto mismatch;
						}
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
				// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					goto mismatch;
				}
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				if (cross[x][y] == NoChess && chessScore[x][y] == 0) goto mismatch;
				cross[x][y] = Rival;
				if (isGo2Dead(x, y, Rival)) {
					cross[x][y] = NoChess;
					// ����ǵз�����ɱ��Ļ������������   -.-������
					chessScore[x][y] = 0;
					goto mismatch;
				}
				// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
				cross[x][y] = NoChess;
				chessScore[x][y] += score;// ����ƥ�䵽��һ��ģ�壬���ģ���λ�þ������
				if (!isContaint({ x,y,score })) {
					goodMoves[MovePointer] = { x,y,score };
					// ����
					std::sort(goodMoves, goodMoves + MovePointer);
					MovePointer++;
				}
			mismatch:
				;
			}
		}
	}
}