#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/Pattern.h"

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
	int patternScore[] = {
		60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20,// 11
		//55, 55, 55, 45, 45, 45,// 6
		//55, 55, 55, 45, 45, 45// 6
		60, 60, 60, 50, 50, 50,// 6
		60, 60, 60, 50, 50, 50// 6
	};
	// ����˵����
	// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

	DIRECTION patternAddScorePos[] = {
		{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },// 11
		{ 0, -1 },{ 1, 0 },{ 1, 1 },{ 0, -1 },{ 1, 0 },{ 1, 1 },// 6���γ�Χɱ��Ĭ�ϲ����ⲿ��λ
		{ 0, 1 },{ 1, 1 },{ 2, -1 },{ 0, 1 },{ 1, 1 },{ 2, -1 }// 6
	};

	// ģʽ���ж����ӵ���
	int patternCount[] = {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,// 44
		7, 7, 7, 7, 7, 7,// 42
		11,11,11,11,11,11// 66
	};
	// ���������뿴��������㷨
	DIRECTION patternBackground[] = {
		{ -1, 0 },{ 1,0 },{ 0,-1 },{ 0,1 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, 0 },{ 0, -1 },{ 0, 1 },{ 1, 0 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },// 2

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },// 2

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },// 4 44

		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },// 6 42 

		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 }// 6 66
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
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White,

		/*********************************************
		����Χɱȱһ
		**********************************************/
		// { 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess,
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess,
		/*********************************************
		��Χɱȱһ
		**********************************************/
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,NoChess,Black | Edge,
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,Black | Edge,NoChess,
		Black | Edge,Black | Edge,Black | Edge,	White,White,NoChess,Black | Edge,White,Black | Edge,Black | Edge,Black | Edge,
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,NoChess,White | Edge,
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,White | Edge,NoChess,
		White | Edge,White | Edge,White | Edge,	Black,Black,NoChess,White | Edge,Black,White | Edge,White | Edge,White | Edge
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
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black,// 11
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess,
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess,// 6
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,NoChess,White | Edge,
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,White | Edge,NoChess,
		White | Edge,White | Edge,White | Edge,	Black,Black,NoChess,White | Edge,Black,White | Edge,White | Edge,White | Edge,
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,NoChess,Black | Edge,
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,Black | Edge,NoChess,
		Black | Edge,Black | Edge,Black | Edge,	White,White,NoChess,Black | Edge,White,Black | Edge,Black | Edge,Black | Edge
	};
	for (i = 0; i < pattern_Total; ++i) {
		this->pattern_Score_Pos[i] = patternAddScorePos[i];
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
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = (*PatternType).x_offset;
		(*PatternType).x_offset = (*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		temp = (*ScorePos).x_offset;
		(*ScorePos).x_offset = (*ScorePos).y_offset;
		(*ScorePos).y_offset = temp;
		++ScorePos;
	}
}

/**
* [AI2::reverse_Y Yȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_Y(DIRECTION *PatternType) {
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		(*ScorePos).y_offset = -(*ScorePos).y_offset;
		++ScorePos;
	}
}

/**
* [AI2::reverse_X Xȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_X(DIRECTION *PatternType) {
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).x_offset = -(*PatternType).x_offset;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		(*ScorePos).x_offset = -(*ScorePos).x_offset;
		++ScorePos;
	}
}

/**
* [AI2::reverse_X_Y XYȡ���󽻻�]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void AI2::reverse_X_Y(DIRECTION *PatternType) {
	register int temp;
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = -(*PatternType).x_offset;
		(*PatternType).x_offset = -(*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		temp = -(*ScorePos).x_offset;
		(*ScorePos).x_offset = -(*ScorePos).y_offset;
		(*ScorePos).y_offset = temp;
		++ScorePos;
	}
}
/**
* [AI2::startPattern ģ��ƥ��]
* @return        [��]
*/
void AI2::startPattern() {
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
				score = pattern_Score[i];
				for (; j < pointer; ++j)
				{
					x_offset = pattern_Background[j].x_offset;
					y_offset = pattern_Background[j].y_offset;
					// ������������
					if (OnChessBoard(x + x_offset, y + y_offset)) {
						if (0 == (cross[x + x_offset][y + y_offset] & PatternType[j])) goto mismatch;//����ͬ��
						else
						{
							if (cross[x + x_offset][y + y_offset] == NoChess) {
								// �����ǰ�հ׵�������λ�Ļ�����ֱ������ƥ����
								if (CS[x + x_offset][y + y_offset] == minLimit) goto mismatch;
								// ���統ǰ�հ׵�ķ���ֵΪ0��ʱ�򣬾�ֱ������
								// ��Ϊ����Ϊ0��ʾ��ǰ�հ׵��λ���ǵз���ɱ�㣬û��Ҫ���
								else if (CS[x + x_offset][y + y_offset] == 0) goto mismatch;
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
				if (pattern_Score_Pos[i].x_offset == 0 &&
					pattern_Score_Pos[i].y_offset == 0 &&
					!checkStone(x, y, pattern_Count[i] <= 4)) {
					goto mismatch;
				};
				if (CS[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] != 0 &&
					CS[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] != minLimit) {
					CS[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] += score;// ����ƥ�䵽��һ��ģ�壬���ģ���λ�þ������
				}
			mismatch:
				;
			}
		}
	}
}

#include "../ChessBoard_Header/WinCheck.h"

bool AI2::checkEmptyPos(const int& x, const int& y, const int& start, const int& mainColor, const Pos* emptyPos) {
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	/******************************************
	�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
	����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
	*******************************************/
	for (int i = 0; i < start; ++i) {
		if (mainColor == Rival) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == 0) {
				return false;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (chessInfo.WinOrLoseCheck(emptyPos[i].line, emptyPos[i].column, Rival, cross)) {
				// ����ǵз�����ɱ��Ļ������������   -.-������
				CS[emptyPos[i].line][emptyPos[i].column] = 0;
				return false;
			}
		}
		else if (mainColor == turn2Who) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == minLimit) {
				return false;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			if (chessInfo.WinOrLoseCheck(emptyPos[i].line, emptyPos[i].column, turn2Who, cross)) {
				CS[emptyPos[i].line][emptyPos[i].column] = minLimit;
				// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
				return false;
			}
		}
		// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
	}
	return true;
}
// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
bool AI2::checkStone(const int& x, const int& y, const bool& below4) {
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
	// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
	if (chessInfo.WinOrLoseCheck(x, y, turn2Who, cross)) {
		CS[x][y] = minLimit;
		// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
		return false;
	}
	// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
	if (!below4) {
		return true;
	}
	if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
	if (chessInfo.WinOrLoseCheck(x, y, Rival, cross)) {
		// ����ǵз�����ɱ��Ļ������������   -.-������
		CS[x][y] = 0;
		return false;
	}
	// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
	return true;
}

void AI2::ScanChessBroad() {
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	for (int x = ChessStart; x < ChessEnd; ++x) {
		for (int y = ChessStart; y < ChessEnd; ++y) {
			if (cross[x][y] == NoChess) {
				if (CS[x][y] == minLimit) continue;
				if (chessInfo.WinOrLoseCheck(x, y, turn2Who, cross)) {
					CS[x][y] = minLimit;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					continue;
				}
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				if (CS[x][y] == 0) continue;
				if (chessInfo.WinOrLoseCheck(x, y, Rival, cross)) {
					// ����ǵз�����ɱ��Ļ������������   -.-������
					CS[x][y] = 0;
					continue;
				}
				// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
			}
		}
	}
}