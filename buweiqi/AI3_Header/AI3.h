#pragma once

#include "../ChessBoard_Header/WinCheck.h"
#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
#include "../ChessBoard_Header/parameter.h"

class SimulatorGo : public AI2 {
private:
	mutable int chessScoreW[ChessEnd][ChessEnd];
protected:
	virtual int* getPatternType() override {
		if (player_to_move == Black || player_to_move == White) {
			return Type[player_to_move - 1];
		}
		else {
			return nullptr;
		}
	}
public:
	bool ifShowInfo;

	mutable int Winner;
	int player_to_move;
	SimulatorGo() :
		Winner(NoChess),
		player_to_move(Black),
		ifShowInfo(false)
	{
		initAllArray();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				chessScoreW[i][j] = getDefaultChessScore(i, j);
				chessScore[i][j] = chessScoreW[i][j];
			}
		}
	}

	SimulatorGo(
		int b[ChessEnd][ChessEnd],
		int Id) :
		player_to_move(Id),
		Winner(NoChess),
		ifShowInfo(false)
	{
		setRole(Id);
		initAllArray();
		initCSPoint();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = b[i][j];
				chessScoreW[i][j] = this->getDefaultChessScore(i, j);
				chessScore[i][j] = chessScoreW[i][j];
			}
		}
	}

	void initSimulation() /*const*/;

	std::vector<int> getAllMoves();

	// 是否还有可着子的着子点
	template<typename RandomEngine>
	bool doRandomMove(RandomEngine* engine)
	{
		initCSPoint();
		auto moves = getMoves();
		if (moves.empty()) {// 如果着子点集合为空的话，就直接返回
			Winner = getRival(player_to_move);
			return false;
		}
		else {
			if (moves.size() == 1) {
				SimulateMove(*moves.begin());
				return true;
			}
			else {
				std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
				// 开始模拟走步
				SimulateMove(moves[move_ind(*engine)]);
				return true;
			}
		}
		return false;
	}

	// 模拟着子，主要的作用是用于模拟下棋
	virtual void SimulateMove(const int& move)
	{
		// 对方的结点
		int rival = getRival(player_to_move);
		// 这里表示模拟没有结束，这时我们需要判断是否已经输赢论定了
		int line, column;
		line = getLine(move);
		column = getColumn(move);
		// 如果没有输赢未论定的话，就继续模拟
		cross[line][column] = player_to_move;

		// 显示模拟的数据，使用ifShowInfo进行控制
		showSimaluteInfo(line, column);

		// 轮到下一个玩家着子
		player_to_move = rival;
		initCSPoint();
	}

	// 从棋盘中搜集所有可行的着子点
	virtual std::vector<int> getMoves() /*const*/;

	// 用于判断输赢结果
	virtual double getResult(const int& current_player_to_move)
	{
		// 没有模拟到尽头
		if (Winner != White && Black != Black) {
			return 0.5;
		}
		return Winner == current_player_to_move ? 1.0 : 0.0;
	}

	virtual bool checkEmptyPos(
		const int& x,
		const int& y,
		const int& start,
		const int& mainColor,
		const Pos* emptyPos) override;
	// 检查棋子是否有效，并对分析的结果进行相应的加分
	virtual bool checkStone(
		const int& x,
		const int& y,
		const bool& below4) override;
	virtual void initCSPoint() override;
	virtual void ScanChessBroad() override;

	void showSimaluteInfo(const int& line, const int& column);

	virtual void changeLiveEye2DeadEye(int line, int column, int type);
	/*
	param[line]:眼的位置——行
	param[column]:眼的位置——列
	param[playerNum]:眼的拥有者
	param[CROSS]:棋盘状态
	param[NullPos]:存放眼周围的空位位置
	return:眼的气
	*/
	int GetSurroundChessNumber(
		int line, 
		int column, 
		int playerNum, 
		int CROSS[][10], 
		std::vector<std::pair<int, int>> &NullPos);
	/*
	param[line]:空位位置
	param[column]:空位位置
	*/
	int GetSurroundNumber(int line, int column)
	{
		int num = 0;
		int ID = 0;
		//上面是否有子
		if (cross[line - 1][column] != NoChess || line == 1)
		{
			if (line != 1) ID = cross[line - 1][column];
			num++;
		}
		//下面是否有子
		if (cross[line + 1][column] != NoChess || line == 9)
		{
			if (line != 9 && ID == 0) ID = cross[line + 1][column];
			else if (ID != cross[line + 1][column]) return -1;
			num++;
		}
		//左边是否有子
		if (cross[line][column - 1] != NoChess || column == 1)
		{
			if (column != 1 && ID == 0) ID = cross[line][column - 1];
			else if (ID != cross[line][column - 1]) return -1;
			num++;
		}
		//右边是否有子
		if (cross[line][column + 1] != NoChess || column == 9)
		{
			if (column != 9 && ID == 0) ID = cross[line][column + 1];
			else if (ID != cross[line][column + 1]) return -1;
			num++;
		}
		return num;
	}
};

class AI3 : public AI2
{
private:

public:

	virtual int maxandmin(const int& depth) override {
		initChessScore(true);
		int best_move = predict();
		if (best_move == 0) {
			for (int x = ChessStart; x < ChessEnd; ++x) {
				for (int y = ChessStart; y < ChessEnd; ++y) {
					if (cross[x][y] == NoChess) {
						return getMove(x, y);
					}
				}
			}
		}
		return best_move;
	}
	int predict();
};

