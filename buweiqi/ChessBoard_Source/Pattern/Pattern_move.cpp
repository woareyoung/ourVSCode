#include "../../ChessBoard_Header/Pattern_Moves.h"

std::list<std::pair<int, int>> Pattern_Moves::getMoves(const bool& isFirst, const int cro[][ChessEnd]) {
	auto translate = [](int move) {
		if (move == noChess) {
			return NoChess;
		}
		return move;
	};
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			this->cross[i][j] = translate(cro[i][j]);
		}
	}
	initChessScore(isFirst);
	startPattern();
	return patternMoves;
}

void Pattern_Moves::initAll() {
	register int i;
	// ��ʼ������ָ������
	void(Pattern_Moves::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     ����ֻ����ӽ�����������
		&Pattern_Moves::reverse,//(X,Y)
		&Pattern_Moves::reverse_Y,//(X,-Y)
		&Pattern_Moves::reverse_X,//(-X,-Y)
		&Pattern_Moves::reverse_Y,//(-X,Y)
		&Pattern_Moves::reverseXY,//(Y,-X)
		&Pattern_Moves::reverse_Y,//(Y,X)
		&Pattern_Moves::reverse_X,//(-Y,X)
		&Pattern_Moves::reverse_Y,//(-Y,-X)
		&Pattern_Moves::reverse_X_Y//(X,Y)
	};
	for (i = ChessInit; i < ChessEnd; ++i) {
		Reverse[i] = temp[i];
	}
	// ģʽ�����߶�
	int patternScore[] = {
		25, 10, 5, 5, 5, 6, 6, 3, 3, 3, 3,// 11
		12, 12, 12, 13, 13, 13,// 6
		14, 14, 14, 14, 14, 14
	};
	// ����˵����
	// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

	DIRECTION patternAddScorePos[] = {
		{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },// 11
		{ 0, -1 },{ 1, 0 },{ 1, 1 }/*3 �з�λ�ã�Ĭ���ڲ������ֹ*/,{ 0, -1 },{ 1, 0 },{ 1, 1 },// 3�������γ�Χɱ��Ĭ�ϲ����ⲿ��λ
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
		����Χɱȱһ����ֹ�з�
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
* [Pattern_Moves::reverse ����ת]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void Pattern_Moves::reverse(DIRECTION *PatternType) {
	;
}

/**
* [Pattern_Moves::reverseXY XY����λ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void Pattern_Moves::reverseXY(DIRECTION *PatternType) {
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
* [Pattern_Moves::reverse_Y Yȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void Pattern_Moves::reverse_Y(DIRECTION *PatternType) {
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
* [Pattern_Moves::reverse_X Xȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void Pattern_Moves::reverse_X(DIRECTION *PatternType) {
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
* [Pattern_Moves::reverse_X_Y XYȡ���󽻻�]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void Pattern_Moves::reverse_X_Y(DIRECTION *PatternType) {
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
* [Pattern_Moves::startPattern ģ��ƥ��]
* @return        [��]
*/
void Pattern_Moves::startPattern() {
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
* [Pattern_Moves::Pattern ʹ��ģ��ƥ����������]
* @param PatternType [ģ��]
*/
void Pattern_Moves::Pattern(const int *PatternType) {
	register int x_offset, y_offset;
	register int i, j, x, y;
	register int pointer, start = 0, score;
	register int mainColor = NoChess;
	register Pos emptyPos[ChessEnd];
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
								if (chessScore[x + x_offset][y + y_offset] == minLimit) goto mismatch;
								// ���統ǰ�հ׵�ķ���ֵΪ0��ʱ�򣬾�ֱ������
								// ��Ϊ����Ϊ0��ʾ��ǰ�հ׵��λ���ǵз���ɱ�㣬û��Ҫ���
								else if (chessScore[x + x_offset][y + y_offset] == 0) goto mismatch;
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
				if (pattern_Score_Pos[i].x_offset == 0  && 
					pattern_Score_Pos[i].y_offset == 0 &&
					!checkStone(x, y, pattern_Count[i] <= 4)) {
					goto mismatch;
				};
				if (chessScore[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] != 0 && 
					chessScore[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] != minLimit) {
					patternMoves.emplace_back(getMove(x + pattern_Score_Pos[i].x_offset, y + pattern_Score_Pos[i].y_offset), score);
				}
			mismatch:
				;
			}
		}
	}
}

void Pattern_Moves::initChessScore(const bool& isFirst)
{
	for (int i = ChessStart; i < ChessEnd; ++i)
	{
		for (int j = ChessStart; j < ChessEnd; ++j)
		{
			if (cross[i][j] == NoChess && chessScore[i][j] == minLimit) continue;
			if (!isFirst && cross[i][j] == NoChess && chessScore[i][j] == 0) continue;
			chessScore[i][j] = getDefaultChessScore(i, j);
		}
	}
}

#include <stack>
#include <tuple>

void Pattern_Moves::resetGo2DeadStatus() {
	for (register int i = ChessInit; i < ChessEnd; ++i) {
		for (register int j = ChessInit; j < ChessEnd; ++j) {
			isGo2DeadStatus[i][j] = false;
		}
	}
}

/**
* [AI2::isGo2Dead �ж����ӵ��Ƿ�����ɱ����]
*/
bool Pattern_Moves::isGo2Dead(const int line, const int column, const int& type)
{
	bool simulate = false;//����Ƿ���ģ������
	int rival = getRival(type);//�Է�
	int tempLine = line, tempColumn = column;

	std::stack<std::pair<int, int>> position;
	std::stack<bool> PosNumber;//���ڼ����������
							   //��������
	auto POPdata = [&]() {
		if (position.empty()) return false;
		std::tie(tempLine, tempColumn) = position.top();
		position.pop();
		return true;
	};
	//ѹջ�������ñ���
	auto PUSHdata = [&](int l, int c) {
		position.push(std::make_pair(tempLine, tempColumn));
		tempLine = l;
		tempColumn = c;
	};
	//��ԭ����
	auto BackData = [&]() {
		tempLine = line;
		tempColumn = column;
		while (!position.empty()) position.pop();
	};
	//�ж��Ƿ���ģ������
	if (cross[tempLine][tempColumn] == NoChess)
	{
		cross[tempLine][tempColumn] = type;
		simulate = true;
	}
	//���ٱ������ж���û�б�Χ����
	//param[Player]:��� ˭ �����ӱ�Χ��
	auto VisitAll = [&](const int& Player)
	{
		resetGo2DeadStatus();
		int Rival = getRival(Player);
		while (true)
		{
			isGo2DeadStatus[tempLine][tempColumn] = true;
			//������λ������ȥ��
			if (cross[tempLine][tempColumn] == NoChess && OnChessBoard(tempLine, tempColumn))
			{
				BackData();
				return false;
			}
			//����������Լ���������δ������
			if (cross[tempLine - 1][tempColumn] == Player && !isGo2DeadStatus[tempLine - 1][tempColumn] && tempLine > 1)
			{
				PUSHdata(tempLine - 1, tempColumn);
				continue;
			}
			else if (cross[tempLine - 1][tempColumn] == NoChess && tempLine > 1)
			{
				--tempLine;
				continue;
			}
			//����������Լ���������δ������
			if (cross[tempLine + 1][tempColumn] == Player && !isGo2DeadStatus[tempLine + 1][tempColumn] && tempLine < 9)
			{
				PUSHdata(tempLine + 1, tempColumn);
				continue;
			}
			else if (cross[tempLine + 1][tempColumn] == NoChess && tempLine < 9)
			{
				++tempLine;
				continue;
			}
			//���������Լ���������δ������
			if (cross[tempLine][tempColumn - 1] == Player && !isGo2DeadStatus[tempLine][tempColumn - 1] && tempColumn > 1)
			{
				PUSHdata(tempLine, tempColumn - 1);
				continue;
			}
			else if (cross[tempLine][tempColumn - 1] == NoChess && tempColumn > 1)
			{
				--tempColumn;
				continue;
			}
			//����ұ����Լ���������δ������
			if (cross[tempLine][tempColumn + 1] == Player && !isGo2DeadStatus[tempLine][tempColumn + 1] && tempColumn < 9)
			{
				PUSHdata(tempLine, tempColumn + 1);
				continue;
			}
			else if (cross[tempLine][tempColumn + 1] == NoChess && tempColumn < 9)
			{
				++tempColumn;
				continue;
			}
			if (POPdata()) continue;
			BackData();
			return true;//�ܵ�����֤������
		}
	};
	if (cross[tempLine - 1][tempColumn] == rival && tempLine > 1)
	{
		--tempLine;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine + 1][tempColumn] == rival && tempLine < 9)
	{
		++tempLine;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine][tempColumn - 1] == rival && tempColumn > 1)
	{
		--tempColumn;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine][tempColumn + 1] == rival && tempColumn < 9)
	{
		++tempColumn;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (VisitAll(type)) {
		if (simulate) cross[line][column] = NoChess;
		return true;
	}
	if (simulate) cross[line][column] = NoChess;
	return false;
}

