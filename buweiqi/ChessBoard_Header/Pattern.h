#pragma once

namespace Pattern {

// ����ƥ��ģʽ
#define Black 1 // ����
#define White 2// ����
#define NoChess 4// û������
#define Edge 8 // �߽��

	typedef struct tagDIRECTION
	{
		int x_offset;
		int y_offset;
	} DIRECTION;

	struct Pos {
		int line;
		int column;
	};

	class ChessPattern {
	private:
		void(Pattern::ChessPattern::*Reverse[10])(DIRECTION*);
		/**
		* [reverse ����ת]
		* @param PatternType [ƥ��ģʽ�����ӷֲ�]
		*/
		void reverse(DIRECTION *PatternType) {
			;
		}

		/**
		* [reverseXY XY����λ��]
		* @param PatternType [ƥ��ģʽ�����ӷֲ�]
		*/
		void reverseXY(DIRECTION *PatternType) {
			register int temp;
			for (register int i = 0; i < pattern_Sum; ++i) {
				temp = (*PatternType).x_offset;
				(*PatternType).x_offset = (*PatternType).y_offset;
				(*PatternType).y_offset = temp;
				++PatternType;
			}
		}

		/**
		* [reverse_Y Yȡ��]
		* @param PatternType [ƥ��ģʽ�����ӷֲ�]
		*/
		void reverse_Y(DIRECTION *PatternType) {
			for (register int i = 0; i < pattern_Sum; ++i) {
				(*PatternType).y_offset = -(*PatternType).y_offset;
				++PatternType;
			}
		}

		/**
		* [reverse_X Xȡ��]
		* @param PatternType [ƥ��ģʽ�����ӷֲ�]
		*/
		void reverse_X(DIRECTION *PatternType) {
			for (register int i = 0; i < pattern_Sum; ++i) {
				(*PatternType).x_offset = -(*PatternType).x_offset;
				++PatternType;
			}
		}

		/**
		* [reverse_X_Y XYȡ���󽻻�]
		* @param PatternType [ƥ��ģʽ�����ӷֲ�]
		*/
		void reverse_X_Y(DIRECTION *PatternType) {
			register int temp;
			for (register int i = 0; i < pattern_Sum; ++i) {
				temp = -(*PatternType).x_offset;
				(*PatternType).x_offset = -(*PatternType).y_offset;
				(*PatternType).y_offset = temp;
				++PatternType;
			}
		}
	protected:
		////��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
		////��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
		//mutable int cross[10][10];
		//// ����
		//int chessScore[10][10];

		bool Position[4];

		// ���������б任
		virtual int* getPatternType(int turn2Who) {
			return Type[turn2Who - 1];
		}

		// ƥ��ģʽ
		static const int pattern_Total = 11;
		// ƥ��Ĵ���
		static const int pattern_Sum = 44;
		// ģʽ�����߶�
		int pattern_Score[pattern_Total];
		// ����˵����
		// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

		// ģʽ���ж����ӵ���
		int pattern_Count[pattern_Total];
		// ���������뿴��������㷨
		DIRECTION pattern_Background[pattern_Sum];
		// ������������ӵ�
		// ƥ��ģʽ�����ӷֲ�
		int pattern_White[pattern_Sum];
		// ƥ��ģʽ�����ӷֲ�
		int pattern_Black[pattern_Sum];
		int *Type[2];
	public:
		/**
		* [startPattern ģ��ƥ��]
		* @return        [��]
		*/
		void startPattern(int turn2Who, int(&cross)[10][10], int(&chessScore)[10][10]) {
			int *PatternType = getPatternType(turn2Who);
			(this->*Reverse[1])(pattern_Background);//��һ���汾 (X, Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[2])(pattern_Background);//�ڶ����汾��Y�ᷴת (X, -Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[3])(pattern_Background);//�������汾��X�ᷴת(-X, -Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[4])(pattern_Background);// ���ĸ��汾��Y�ᷴת(-X, Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[5])(pattern_Background);// ������汾��XY�ύ�� (Y, -X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[6])(pattern_Background);// �������汾��XY�ύ��֮��Y�ᷴת (Y ,X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[7])(pattern_Background);// ���߸��汾��XY�ύ��֮��X�ᷴת (-Y, X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[8])(pattern_Background);// �ڰ˸��汾��XY�ύ��֮��Y�ᷴת (-Y, -X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[9])(pattern_Background);// �ھŸ��汾��-X��-Y�ύ�� (X, Y),�������Ҫ�����ǽ�ģ�廹ԭ
		}

		/**
		* [Pattern ʹ��ģ��ƥ����������]
		* @param PatternType [ģ��]
		*/
		void Pattern(int *PatternType, int(&cross)[10][10], int(&chessScore)[10][10]) {
			register int x_offset, y_offset;
			register int i, j, x, y;
			register int pointer, start = 0, score = 0;
			register int *pattern = PatternType;
			int mainColor = NoChess;
			Pos emptyPos[3];
			auto OnChessBoard = [](int x, int y) {
				auto isOnChessBoard = [](int x) {
					return 0 < x && x < 10;
				};
				return isOnChessBoard(x) && isOnChessBoard(y);
			};
			// ÿһ�����Ӷ�Ҫ����һ��ģ��
			for (x = 1; x < 10; ++x) {
				for (y = 1; y < 10; ++y) {
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
						chessScore[x][y] += score;// ����ƥ�䵽��һ��ģ�壬���ģ���λ�þ������
						// arraySort(x, y, score);// ��������
					mismatch:
						;
					}
				}
			}
		}

		ChessPattern() {
			register int i;
			// ��ʼ������ָ������
			void(Pattern::ChessPattern::*temp[])(Pattern::DIRECTION*) = {
				nullptr,//(X,Y)     ����ֻ����ӽ�����������
				&Pattern::ChessPattern::reverse,//(X,Y)
				&Pattern::ChessPattern::reverse_Y,//(X,-Y)
				&Pattern::ChessPattern::reverse_X,//(-X,-Y)
				&Pattern::ChessPattern::reverse_Y,//(-X,Y)
				&Pattern::ChessPattern::reverseXY,//(Y,-X)
				&Pattern::ChessPattern::reverse_Y,//(Y,X)
				&Pattern::ChessPattern::reverse_X,//(-Y,X)
				&Pattern::ChessPattern::reverse_Y,//(-Y,-X)
				&Pattern::ChessPattern::reverse_X_Y//(X,Y)
			};
			for (i = 0; i < 10; ++i) {
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

		virtual void arraySort(int& x, int& y, int& score) = 0;
		virtual bool checkStone(int& x, int& y) = 0;
		virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) = 0;
	};
	
}

