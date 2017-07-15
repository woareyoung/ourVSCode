#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/Pattern.h"

int patternAllScore[9][11] = {
	{ 60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },// 6
	{ 55, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 54, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 53, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 52, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 51, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },

	{ 45, 40, 35, 30, 30, 25, 25, 22, 22, 20, 20 },// 1

	{ 50, 60, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 40, 50, 60, 35, 35, 30, 30, 25, 25, 20, 20 }
};

void AI2::initAll() {
	register int i;
	// ��ʼ������ָ������
	void(AI2::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     ����ֻ����ӽ�����������
		&AI2::reverse,//(X,Y)
		&AI2::reverse_Y,//(X,-Y)
		&AI2::reverse_X,//(-X,-Y)
		&AI2::reverse_Y,//(-X,Y)
		&AI2::reverseXY,//(Y,-X)
		&AI2::reverse_Y,//(Y,X)
		&AI2::reverse_X,//(-Y,X)
		&AI2::reverse_Y,//(-Y,-X)
		&AI2::reverse_X_Y//(X,Y)
	};
	for (i = ChessInit; i < ChessEnd; ++i) {
		Reverse[i] = temp[i];
	}
	// ģʽ�����߶�
	int patternScore[] = { 60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 };
	// ����˵����
	// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

	// ģʽ���ж����ӵ���
	int patternCount[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
	// ���������뿴��������㷨
	DIRECTION patternBackground[] = {
		{ -1, 0 },{ 1,0 },{ 0,-1 },{ 0,1 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, 0 },{ 0, -1 },{ 0, 1 },{ 1, 0 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 }
	};
	// ������������ӵ�
	// ƥ��ģʽ�����ӷֲ�
	int patternWhite[] = {
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
		�з�����ʮ��Χɱ������Χɱ���߽�Χɱ������
		�з�����Χɱ���ǵڶ����ȼ�
		������������Χɱ��(ȱ������Χɱ��)
		**********************************************/
		Black | Edge,	Black | Edge,	NoChess,		NoChess,
		Black | Edge,	NoChess,		NoChess,		Black | Edge,
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
		NoChess | Edge, Black,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black,
		NoChess | Edge, White,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White
	};
	// ƥ��ģʽ�����ӷֲ�
	int patternBlack[] = {
		White | Edge,	White | Edge,	White | Edge,	NoChess,
		Black | Edge,	Black | Edge,	NoChess,		Black | Edge,
		Edge,			Edge,			NoChess,		NoChess,
		White | Edge,	White | Edge,	NoChess,		NoChess,
		White | Edge,	NoChess,		NoChess,		White | Edge,
		Black | Edge,	Black | Edge,	NoChess,		NoChess,
		Black | Edge,	NoChess,		NoChess,		Black | Edge,
		NoChess | Edge, White,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White,
		NoChess | Edge, Black,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black
	};
	for (i = 0; i < pattern_Total; ++i) {
		this->pattern_Score[i] = patternScore[i];
		this->pattern_Count[i] = patternCount[i];
	}
	for (i = 0; i < pattern_Sum; ++i) {
		this->pattern_Background[i] = patternBackground[i];
		this->pattern_White[i] = patternWhite[i];
		this->pattern_Black[i] = patternBlack[i];
	}
	int* pType[] = { pattern_Black,pattern_White };
	for (i = 0; i < 2; ++i) {
		Type[i] = pType[i];
	}
}

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
	for (register int i = 0; i < pattern_Sum; ++i) {
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
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
}

/**
* [AI2::reverse_X Xȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_X(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; ++i) {
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
	for (register int i = 0; i < pattern_Sum; ++i) {
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
	initCSPoint();
	int *PatternType = getPatternType();
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
void AI2::Pattern(const int *PatternType) {
	register int x_offset, y_offset;
	register int i, j, x, y;
	register int pointer, start = 0, score;
	register int mainColor = NoChess;
	register Pos emptyPos[10];
	// ÿһ�����Ӷ�Ҫ����һ��ģ��
	for (x = ChessStart; x < ChessEnd; ++x) {
		for (y = ChessStart; y < ChessEnd; ++y) {
			pointer = 0;
			for (i = 0; i < pattern_Total; ++i)
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
						if (0 == (cross[x + x_offset][y + y_offset] & PatternType[j])) goto mismatch;//����ͬ��
						else
						{
							if (cross[x + x_offset][y + y_offset] == NoChess) {
								// ���統ǰ�հ׵�ķ���ֵΪ0��ʱ�򣬾�ֱ������
								// ��Ϊ����Ϊ0��ʾ��ǰ�հ׵��λ���ǵз���ɱ�㣬û��Ҫ���
								if (CS[x + x_offset][y + y_offset] == 0) goto mismatch;
								else {
									emptyPos[start].line = x + x_offset;
									emptyPos[start++].column = y + y_offset;
								}
							}
							else mainColor = cross[x + x_offset][y + y_offset];
						}
					}
					else {
						if (0 == (Edge & PatternType[j])) goto mismatch;//���Ǳ߽�
					}
				}
				// ƥ�䵽һ��ģ��
				// ����ƥ�䵽��ģ�壬������Ҫ����ģ�廷�����ж�
				// 1���Ƿ�Χɱ��2���Ƿ�Χɱ����

				// �����ǰλ�ò�Ϊ�յĻ�����ֱ��������
				if (cross[x][y] != NoChess) goto mismatch;
				if (!checkEmptyPos(x, y, start, mainColor, emptyPos)) {
					goto mismatch;
				}
				/******************************************
				������һ���߼���Ҫ����
				����һ���㱻ƥ�䵽��εĻ�����δ���
				1�������ۼӷֵķ������д���
				2����Ҫע������ӵ��Ƿ��������
				*******************************************/
				// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
				if (!checkStone(x, y)) {
					goto mismatch;
				};
				CS[x][y] += score;// ����ƥ�䵽��һ��ģ�壬���ģ���λ�þ������
			mismatch:
				;
			}
		}
	}
}