#include "../AI3_Header/AI3.h"
#include <tuple>
#include <set>
#include <iostream>
#include <cstdlib>


// ���������Ѽ����п��е����ӵ�
std::vector<int> SimulatorGo::getMoves() const
{
	// �������Ѽ����п��ܵ����ӵ㡣
	std::vector<int> moves;

	// �����ȴ���888��Ļ��ͣ�ֱ��������moves�ˡ�
	if (depth > 888) {
		return moves;
	}

	if (Winner == Black && Winner == White) {
		if (ifShowInfo) {
			system("pause");
			_cprintf("Winner is %s", Winner == Black ? "Black" : "White");
		}
		return moves;
	}

	if (ifShowInfo) {
		// ����Pattern�Ե�ǰ������д����������п��ܵ����ӵ���뵽moves����
		_cprintf("Player is -> %s \n", player_to_move == Black ? "Black" : "White");
	}

	const_cast<SimulatorGo*>(this)->initCSPoint();
	initSimulation();
	const_cast<SimulatorGo*>(this)->startPattern();

	// �ӵ�ǰ������ѡ�����Patternƥ������ıȽϺõ����ӵ㼯��
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] >= 20) {
				moves.emplace_back(getMove(i, j));
			}
		}
	}
	// �����ǰPatternƥ����֮��û��һ���Ǻõ����ӵ�
	if (moves.size() == 0) {
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && CS[i][j] != minLimit && CS[i][j] != 0) {
					if (const_cast<SimulatorGo*>(this)->isGo2Dead(i, j, player_to_move)) {
						CS[i][j] = minLimit;
					}
					else {
						moves.emplace_back(getMove(i, j));
					}
				}
			}
		}
	}
	if (moves.size() == 0) {
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && CS[i][j] == 0) {
					moves.emplace_back(getMove(i, j));
				}
			}
		}
	}

	// ���ִ����������֮��û��һ�����ӵ�����µĻ�����Ϸ������
	if (ifShowInfo && moves.empty()) {
		_cprintf("moves is null.\n");
	}
	return moves;
}

bool SimulatorGo::checkEmptyPos(
	const int& x,
	const int& y,
	const int& start,
	const int& mainColor,
	const Pos* emptyPos) 
{
	/******************************************
	�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
	����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
	*******************************************/
	int rival = getRival(player_to_move);
	for (int i = 0; i < start; ++i) {
		if (mainColor == rival) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == 0) {
				return false;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, rival)) {
				// ����ǵз�����ɱ��Ļ������������   -.-������
				CS[emptyPos[i].line][emptyPos[i].column] = 0;
				return false;
			}
		}
		else if (mainColor == player_to_move) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == minLimit) {
				return false;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, player_to_move)) {
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
bool SimulatorGo::checkStone(const int& x, const int& y, const bool& below4) {
	// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
	// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
	int rival = getRival(player_to_move);
	if (isGo2Dead(x, y, player_to_move)) {
		CS[x][y] = minLimit;
		// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
		return false;
	}
	if (!below4) {
		return true;
	}
	// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
	if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
	if (isGo2Dead(x, y, rival)) {
		// ����ǵз�����ɱ��Ļ������������   -.-������
		CS[x][y] = 0;
		return false;
	}
	// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
	return true;
}

void SimulatorGo::initCSPoint() {
	if (player_to_move == White) {
		CS = chessScoreW;
	}
	else {
		CS = chessScore;// ����Ŀ�ⲻ���õ���
	}
}

std::vector<int> SimulatorGo::getAllMoves() {
	startPattern();
	ScanChessBroad();
	std::vector<int> allMoves;
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] != minLimit) {
				if (!isGo2Dead(i, j, player_to_move)) {
					allMoves.emplace_back(getMove(i, j));
				}
				else {
					if (isGo2Dead(i, j, getRival(player_to_move))) {
						CS[i][j] = 0;
					}
					else {
						CS[i][j] = minLimit;
					}
				}
			}
		}
	}
	return allMoves;
}

void SimulatorGo::initSimulation() const {
	for (int i = ChessInit; i < ChessEnd; ++i) {
		for (int j = ChessInit; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] == minLimit) continue;
			if (cross[i][j] == NoChess && CS[i][j] == 0) continue;
			CS[i][j] = const_cast<SimulatorGo*>(this)->getDefaultChessScore(i, j);
		}
	}
}

void SimulatorGo::showSimaluteInfo(const int& line, const int& column) {
	if (ifShowInfo) {
		_cprintf("\n**************This is chess score*******(%d, %d)***%s********\n",
			line, column, player_to_move == Black ? "Black" : "White");
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
				if (CS[i][j] >= 20) {
					_cprintf("%d\t", CS[i][j]);
				}
				else if (CS[i][j] == minLimit) {
					_cprintf("M\t");
				}
				else if (CS[i][j] == 0) {
					_cprintf("0\t");
				}
				else {
					_cprintf(".\t");
				}
				_cprintf("\n");
		}
		_cprintf("**************This is chess cross*********(%d, %d)***%s******\n",
			line, column, player_to_move == Black ? "Black" : "White");
		showChessBoard(cross);
		system("pause");
	}
}