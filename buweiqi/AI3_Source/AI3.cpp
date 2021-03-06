#include "../AI3_Header/AI3.h"
#include <tuple>
#include <set>
#include <iostream>
#include <cstdlib>

bool sort(const std::pair<int, int>& first, const std::pair<int, int>& second) {
	return first.second > second.second;
}

// 从棋盘中搜集所有可行的着子点
std::vector<int> SimulatorGo::getMoves() /*const*/
{
	// 下面是搜集所有可能的着子点。
	std::vector<int> moves;

	if (Winner == Black || Winner == White) {
		return moves;
	}

	if (ifShowInfo) {
		// 调用Pattern对当前局面进行处理，将所有可能的着子点加入到moves数组
		_cprintf("Player is -> %s \n", player_to_move == Black ? "Black" : "White");
	}

	initSimulation();
	// const_cast<SimulatorGo*>(this)->startPattern();
	// const_cast<SimulatorGo*>(this)->ScanChessBroad();
	ScanChessBroad();
	startPattern();
	ScanChessBroad();

	std::vector<std::pair<int, int>> topMoves;
	// 判断是否是死棋位
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	// 从当前棋盘中选择出由Pattern匹配出来的比较好的着子点集合
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] >= 10) {
				moves.emplace_back(getMove(i, j));
				topMoves.emplace_back(getMove(i, j), CS[i][j]);
			}
		}
	}
	if (moves.size() > 0) {
		std::sort(topMoves.begin(), topMoves.end(), sort);
		std::vector<int> tempMoves;
		int MaxScore = (*topMoves.begin()).first;
		int size = static_cast<int>(topMoves.size());
		for (int i = 0; i < size; ++i) {
			if (MaxScore == topMoves[i].first) {
				tempMoves.emplace_back(topMoves[i].first);
			}
		}
		return tempMoves;
	}
	// 如果当前Pattern匹配完之后，没有一个是好的着子点
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

	// 如果执行完上述的之后没有一个着子点可以下的话，游戏结束。
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
	判断当前匹配到的空位是否是敌方的自杀点，
	如果是的话，就把该点的分数设置为0，跳过匹配模式
	*******************************************/
	int rival = getRival(player_to_move);
	for (int i = 0; i < start; ++i) {
		if (mainColor == rival) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == 0) {
				return false;
			}
			// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
			if (chessInfo.WinOrLoseCheck(emptyPos[i].line, emptyPos[i].column, rival, cross)) {
				// 如果是敌方的自杀点的话，这里就置零   -.-！！！
				CS[emptyPos[i].line][emptyPos[i].column] = 0;
				return false;
			}
		}
		else if (mainColor == player_to_move) {
			if (CS[emptyPos[i].line][emptyPos[i].column] == minLimit) {
				return false;
			}
			// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
			if (chessInfo.WinOrLoseCheck(emptyPos[i].line, emptyPos[i].column, player_to_move, cross)) {
				CS[emptyPos[i].line][emptyPos[i].column] = minLimit;
				// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
				return false;
			}
		}
		// 这里既不是我方自杀点，也不是敌方自杀点
	}
	return true;
}

// 检查棋子是否有效，并对分析的结果进行相应的加分
bool SimulatorGo::checkStone(const int& x, const int& y, const bool& below4) {
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	// 对于当前匹配到的着子点的环境进行分析
	// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
	int rival = getRival(player_to_move);
	if (chessInfo.WinOrLoseCheck(x, y, player_to_move, cross)) {
		CS[x][y] = minLimit;
		// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
		return false;
	}
	if (!below4) {
		return true;
	}
	// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
	if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
	if (chessInfo.WinOrLoseCheck(x, y, rival, cross)) {
		// 如果是敌方的自杀点的话，这里就置零   -.-！！！
		CS[x][y] = 0;
		return false;
	}
	// 这里既不是我方自杀点，也不是敌方自杀点
	return true;
}

void SimulatorGo::initCSPoint() {
	if (player_to_move == White) {
		CS = chessScoreW;
	}
	else {
		CS = chessScore;// 这里目测不会用到的
	}
}

void SimulatorGo::changeLiveEye2DeadEye(int line, int column, int type) {
	std::vector<std::pair<int, int>> NullPos;
	int size = GetSurroundChessNumber(line, column, type, cross, NullPos);
	int score = 0;

	if (size > 0 && size <= 3) {
		for (int i = 0; i < size; ++i) {
			if (CS[NullPos[i].first][NullPos[i].second] == minLimit) {
				return;
			}
		}
	}

	if (size == 3) {
		score = pattern_Score[7];
	}
	else if (size == 2) {
		score = pattern_Score[5];
	}
	else if (size == 1) {
		score = pattern_Score[3];
	}

	if (size > 0 && size <= 3) {
		for (int i = 0; i < size; ++i) {
			CS[NullPos[i].first][NullPos[i].second] = score;
		}
	}
}

#include <omp.h>
std::vector<int> SimulatorGo::getAllMoves() {
	initSimulation();
	ScanChessBroad();
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
#pragma omp parallel for
	for (int i = ChessStart; i < ChessEnd; ++i) {
#pragma omp parallel for
		for (int j = ChessStart; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess && CS[i][j] != minLimit) {
				if (!chessInfo.WinOrLoseCheck(i, j, player_to_move, cross)) {
					allMoves.emplace_back(getMove(i, j));
				}
				else {
					if (CS[i][j] != 0 && chessInfo.WinOrLoseCheck(i, j, rival, cross)) {
						CS[i][j] = 0;
					}
					else if (CS[i][j] != 0) {
						CS[i][j] = minLimit;
					}
				}
			}
		}
	}
	return allMoves;
}

void SimulatorGo::initSimulation() /*const*/ {
#pragma omp parallel for
	for (int i = ChessInit; i < ChessEnd; ++i) {
#pragma omp parallel for
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
#pragma omp parallel for
	for (int x = ChessStart; x < ChessEnd; ++x) {
#pragma omp parallel for
		for (int y = ChessStart; y < ChessEnd; ++y) {
			if (cross[x][y] == NoChess) {
				if (GetSurroundNumber(x, y) == 4)
				{
					changeLiveEye2DeadEye(x, y, cross[x][y]);
				}
				if (CS[x][y] == minLimit) {
					continue;
				}
				if (chessInfo.WinOrLoseCheck(x, y, player_to_move, cross)) {
					CS[x][y] = minLimit;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					continue;
				}
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (CS[x][y] == 0) {
					continue;
				}
				if (chessInfo.WinOrLoseCheck(x, y, getRival(player_to_move), cross)) {
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					CS[x][y] = 0;
					continue;

				}
				// 这里既不是我方自杀点，也不是敌方自杀点
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
	options.number_of_threads = 4;
	// options.verbose = true;
	options.max_iterations = -1;
	options.max_time = 1;
	auto state_copy = new SimulatorGo(cross, PlayerId);
	return MCTS::computeNextMove(state_copy, options);
}
