#include "../AI3_Header/AI3.h"
#include <tuple>
#include <set>
#include <iostream>
#include <cstdlib>

// ���������Ѽ����п��е����ӵ�
std::vector<int> SimulatorGo::getMoves() /*const*/
{
	// �������Ѽ����п��ܵ����ӵ㡣
	std::vector<int> moves;

	// �����ȴ���888��Ļ��ͣ���ֱ�ӷ���moves�ˡ�
	if (depth > 888) {
		return moves;
	}

	if (Winner == Black || Winner == White) {
		if (ifShowInfo) {
			system("pause");
			_cprintf("Winner is %s", Winner == Black ? "Black" : "White");
		}
		return moves;
	}

	if (ifShowInfo) {
		// ����Pattern�Ե�ǰ������д��������п��ܵ����ӵ���뵽moves����
		_cprintf("Player is -> %s \n", player_to_move == Black ? "Black" : "White");
	}

	initSimulation();
	// const_cast<SimulatorGo*>(this)->startPattern();
	// const_cast<SimulatorGo*>(this)->ScanChessBroad();
	startPattern();
	ScanChessBroad();

	// �ж��Ƿ�������λ
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
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
					if (chessInfo.WinOrLoseCheck(i, j, player_to_move, cross)) {
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
					if (chessInfo.WinOrLoseCheck(i, j, player_to_move, cross)) {
						CS[i][j] = minLimit;
					}
					else {
						moves.emplace_back(getMove(i, j));
					}
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
	int rival = getRival(player_to_move);
	for (int i = 0; i < start; ++i) {
		if (mainColor == rival) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == 0) {
				return false;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (chessInfo.WinOrLoseCheck(emptyPos[i].line, emptyPos[i].column, rival, cross)) {
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
			if (chessInfo.WinOrLoseCheck(emptyPos[i].line, emptyPos[i].column, player_to_move, cross)) {
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
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
	// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
	int rival = getRival(player_to_move);
	if (chessInfo.WinOrLoseCheck(x, y, player_to_move, cross)) {
		CS[x][y] = minLimit;
		// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
		return false;
	}
	if (!below4) {
		return true;
	}
	// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
	if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
	if (chessInfo.WinOrLoseCheck(x, y, rival, cross)) {
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
	initSimulation();
	startPattern();
	ScanChessBroad();
	std::vector<int> allMoves;
	int rival = getRival(player_to_move);
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] != minLimit) {
				if (!chessInfo.WinOrLoseCheck(i, j, player_to_move,cross)) {
					allMoves.emplace_back(getMove(i, j));
				}
				else {
					if (CS[i][j] != 0 && chessInfo.WinOrLoseCheck(i, j, rival, cross)) {
						CS[i][j] = 0;
					}
					else if (CS[i][j] != 0){
						CS[i][j] = minLimit;
					}
				}
			}
		}
	}
	return allMoves;
}

void SimulatorGo::initSimulation() /*const*/ {
	for (int i = ChessInit; i < ChessEnd; ++i) {
		for (int j = ChessInit; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] == minLimit) continue;
			if (cross[i][j] == NoChess && CS[i][j] == 0) continue;
			CS[i][j] = /*const_cast<SimulatorGo*>(this)->*/getDefaultChessScore(i, j);
		}
	}
}

void SimulatorGo::ScanChessBroad() {
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	int rival = getRival(player_to_move);
	for (int x = ChessStart; x < ChessEnd; ++x) {
		for (int y = ChessStart; y < ChessEnd; ++y) {
			if (cross[x][y] == NoChess) {
				if (chessInfo.WinOrLoseCheck(x, y, player_to_move, cross)) {
					CS[x][y] = minLimit;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					continue;
				}
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				if (cross[x][y] == NoChess && CS[x][y] == 0) continue;
				if (chessInfo.WinOrLoseCheck(x, y, rival, cross)) {
					// ����ǵз�����ɱ��Ļ������������   -.-������
					CS[x][y] = 0;
					continue;
				}
				// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
			}
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

#include "../AI3_Header/MCTS.h"

int AI3::predict() {
	MCTS::ComputeOptions options;
	options.number_of_threads = 1;
	options.verbose = true;
	// options.max_iterations = -1;
	options.max_time = 10000;
	auto state_copy = new SimulatorGo(cross, PlayerId);
	auto best_move = MCTS::computeNextMove(state_copy, options);
	return best_move;
}
