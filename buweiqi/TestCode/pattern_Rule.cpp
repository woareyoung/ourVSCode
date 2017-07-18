#include <iostream>
#include <string>
using namespace std;
#define ChessInit 0
#define ChessStart 1
#define ChessEnd 10

#define maxLimit 32767
#define minLimit -32767

// ����ƥ��ģʽ
#define Black 1 // ����
#define White 2// ����
#define NoChess 4// û������
#define Edge 8 // �߽��
// �ж������Ƿ�����������
#define isOnChessBoard(x) (0 < (x) && (x) < 10)
#define OnChessBoard(x, y) (isOnChessBoard(x) && isOnChessBoard(y))  //��������������

#define getRival(onTurn) (onTurn == White ? Black : White)

#define getLine(temp) ( (temp) <= 0 || (temp) > 990 ? 0 : (temp / 100))
#define getColumn(temp) ( (temp) <= 0 || (temp) > 990 ? 0 : (temp % 100))
#define getMove(line, column) (OnChessBoard(line, column) ? (line * 100 + column) : 0)

typedef struct tagDIRECTION
{
	int x_offset;
	int y_offset;
} DIRECTION;

struct Pos {
	int line;
	int column;
};

class pattern_Rule
{
private:
	// ƥ��ģʽ
	static const int pattern_Total = 6;
	// ƥ��Ĵ���
	static const int pattern_Sum = 42;
	// ģʽ�����߶�
	int pattern_Score[pattern_Total];
	// ����˵����
	// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

	// pattern�ӷ�λ��
	DIRECTION pattern_Score_Pos[pattern_Total];

	// ģʽ���ж����ӵ���
	int pattern_Count[pattern_Total];
	// ���������뿴��������㷨
	DIRECTION pattern_Background[pattern_Sum];
	// ������������ӵ�
	// ƥ��ģʽ�����ӷֲ�
	int pattern_White[pattern_Sum];
	// ƥ��ģʽ�����ӷֲ�
	int pattern_Black[pattern_Sum];

	void (pattern_Rule::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);

	void initAll();
	void init_cross() {
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				cross[i][j] = NoChess;
			}
		}
	}
	void Pattern(const int *PatternType, const int& line, const int& column, const int& turn2Who);
protected:
	int *Type[2];
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	mutable int cross[10][10];
public:
	pattern_Rule() {
		initAll();
		init_cross();
	}
	~pattern_Rule() {}

	void startPattern(const int& turn2Who, const int& line, const int& column);
	// -----------------------------------���̽���----------------------------------
	void showUnicode(int pos) {
		string ch[20] = { "��","��","��","��","��","��","��","��","��","��" };
		cout<<ch[pos] << "  ";
	}

	void showChessBoard() {
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (i == 0) {
					showUnicode(j);
				}
				else if (j == 0) {
					showUnicode(i);
				}
				else {
					if (cross[i][j] == NoChess) {
						cout << "  " << "  ";
					}
					else if (cross[i][j] == Black) {
						cout << "��" << "  ";
					}
					else if (cross[i][j] == White) {
						cout << "��" << "  ";
					}
				}
			}
			cout << endl << endl;
		}
	}
};

void pattern_Rule::initAll() {
	register int i;
	// ��ʼ������ָ������
	void(pattern_Rule::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     ����ֻ����ӽ�����������
		&pattern_Rule::reverse,//(X,Y)
		&pattern_Rule::reverse_Y,//(X,-Y)
		&pattern_Rule::reverse_X,//(-X,-Y)
		&pattern_Rule::reverse_Y,//(-X,Y)
		&pattern_Rule::reverseXY,//(Y,-X)
		&pattern_Rule::reverse_Y,//(Y,X)
		&pattern_Rule::reverse_X,//(-Y,X)
		&pattern_Rule::reverse_Y,//(-Y,-X)
		&pattern_Rule::reverse_X_Y//(X,Y)
	};
	for (i = ChessInit; i < ChessEnd; ++i) {
		Reverse[i] = temp[i];
	}
	// ģʽ�����߶�
	int patternScore[] = {
		60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20,// 11
		65, 65, 65, 55, 55, 55// 6
	};
	// ����˵����
	// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

	DIRECTION patternAddScorePos[] = {
		{ 0, 0 },{ 0, 0 },{ 0, 0 }/*3 �з�λ�ã�Ĭ���ڲ������ֹ*/,{ 0, -1 },{ 1, 0 },{ 1, 1 }// 3�������γ�Χɱ��Ĭ�ϲ����ⲿ��λ
	};

	// ģʽ���ж����ӵ���
	int patternCount[] = {
		7, 7, 7, 7, 7, 7// 42

	};
	// ���������뿴��������㷨
	DIRECTION patternBackground[] = {
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },// 6 42 
	};
	// ������������ӵ�
	// ƥ��ģʽ�����ӷֲ�
	int patternWhite[] = {
		/*********************************************
		����Χɱȱһ����ֹ�з�
		**********************************************/
		// { 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess,
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess
	};
	// ƥ��ģʽ�����ӷֲ�
	int patternBlack[] = {
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess,
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess// 6
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
* [pattern_Rule::reverse ����ת]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void pattern_Rule::reverse(DIRECTION *PatternType) {
	;
}

/**
* [pattern_Rule::reverseXY XY����λ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void pattern_Rule::reverseXY(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = (*PatternType).x_offset;
		(*PatternType).x_offset = (*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}

/**
* [pattern_Rule::reverse_Y Yȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void pattern_Rule::reverse_Y(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
}

/**
* [pattern_Rule::reverse_X Xȡ��]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void pattern_Rule::reverse_X(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).x_offset = -(*PatternType).x_offset;
		++PatternType;
	}
}

/**
* [pattern_Rule::reverse_X_Y XYȡ���󽻻�]
* @param PatternType [ƥ��ģʽ�����ӷֲ�]
*/
void pattern_Rule::reverse_X_Y(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = -(*PatternType).x_offset;
		(*PatternType).x_offset = -(*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}
/**
* [pattern_Rule::startPattern ģ��ƥ��]
* @return        [��]
*/
void pattern_Rule::startPattern(const int& turn2Who, const int& line, const int& column) {
	int *PatternType = Type[turn2Who - 1];
	cout << "**************��һ���汾 (X, Y)******************" << endl << endl;
	(this->*Reverse[1])(pattern_Background);//��һ���汾 (X, Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************�ڶ����汾��Y�ᷴת (X, -Y)******************" << endl << endl;
	(this->*Reverse[2])(pattern_Background);//�ڶ����汾��Y�ᷴת (X, -Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************�������汾��X�ᷴת(-X, -Y)******************" << endl << endl;
	(this->*Reverse[3])(pattern_Background);//�������汾��X�ᷴת(-X, -Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************���ĸ��汾��Y�ᷴת(-X, Y)******************" << endl << endl;
	(this->*Reverse[4])(pattern_Background);// ���ĸ��汾��Y�ᷴת(-X, Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************������汾��XY�ύ�� (Y, -X)******************" << endl << endl;
	(this->*Reverse[5])(pattern_Background);// ������汾��XY�ύ�� (Y, -X)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************�������汾��XY�ύ��֮��Y�ᷴת (Y ,X)******************" << endl << endl;
	(this->*Reverse[6])(pattern_Background);// �������汾��XY�ύ��֮��Y�ᷴת (Y ,X)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************���߸��汾��XY�ύ��֮��X�ᷴת (-Y, X)******************" << endl << endl;
	(this->*Reverse[7])(pattern_Background);// ���߸��汾��XY�ύ��֮��X�ᷴת (-Y, X)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************�ڰ˸��汾��XY�ύ��֮��Y�ᷴת (-Y, -X)******************" << endl << endl;
	(this->*Reverse[8])(pattern_Background);// �ڰ˸��汾��XY�ύ��֮��Y�ᷴת (-Y, -X)
	Pattern(PatternType, line, column, turn2Who);
	(this->*Reverse[9])(pattern_Background);// �ھŸ��汾��-X��-Y�ύ�� (X, Y),�������Ҫ�����ǽ�ģ�廹ԭ
}

void pattern_Rule::Pattern(const int *PatternType, const int& line, const int& column, const int& turn2Who) {
	int x_offset, y_offset;
	int i, j;
	int pointer = 0;
	for (i = 0; i < pattern_Total; ++i) {
		j = pointer;
		pointer += pattern_Count[i];
		for (; j < pointer; ++j) {
			x_offset = pattern_Background[j].x_offset;
			y_offset = pattern_Background[j].y_offset;
			if ((Edge & PatternType[j]) == 0) continue;
			if (PatternType[j] == NoChess) continue;
			cross[line + x_offset][column + y_offset] = PatternType[j] > Edge ? PatternType[j] - Edge : PatternType[j];
		}
	}
	showChessBoard();
	init_cross();
}

int main() {
	pattern_Rule p;
	// ˵������һ��������Χɱ�����ڶ��͵�����ԭ��λ�ã�����ѡ�м�ĵ㡣
	// pattern���ܴ�Edge
	p.startPattern(Black, 8, 1);

	return 0;
}