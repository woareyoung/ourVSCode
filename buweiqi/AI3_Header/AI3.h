#pragma once

#include "../ChessBoard_Header/WinCheck.h"
#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
#include "../ChessBoard_Header/parameter.h"

class SimulatorGo : public AI2 {
private:
	WinCheck::ChessInfo chessInfo;
	mutable int chessScoreW[ChessEnd][ChessEnd];
protected:
	virtual int* getPatternType() override {
		return Type[player_to_move - 1];
	}
public:
	bool ifShowInfo;

	mutable int Winner;
	int player_to_move;
	mutable int depth;
	SimulatorGo() :
		depth(0),
		Winner(NoChess),
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
		depth(0),
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
		auto moves = getAllMoves();/*getMoves()*/;
		if (moves.empty()) {// 如果着子点集合为空的话，就直接返回
			Winner = getRival(player_to_move);
			return false;
		}
		if (!moves.empty()) {
			return doRandomMove(engine, moves);
		}
		return false;
	}

	// 随机走步
	template<typename RandomEngine>
	bool doRandomMove(RandomEngine* engine, std::vector<int>& moves)
	{
		int move;
		// 判断是否是死棋位
		WinCheck::ChessBoardOption option;
		option.black = Black;
		option.white = White;
		option.edge = Edge;
		option.emptyChess = NoChess;
		WinCheck::ChessInfo chessInfo(option);
		while (true) {
			if (moves.size() == 0) {
				Winner = getRival(player_to_move);
				return false;
			}
			std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
			move = moves[move_ind(*engine)];
			int line = getLine(move);
			int column = getColumn(move);
			if (chessInfo.WinOrLoseCheck(line, column, player_to_move, cross)) {
				CS[line][column] = minLimit;
				auto itr = moves.begin();
				for (; itr != moves.end() && *itr != move; ++itr);
				moves.erase(itr);// 从moves数组中删除move元素
			}
			else if (moves.empty()) {
				Winner = getRival(player_to_move);
				return false;
			}
			else {
				break;
			}
		}

		// 开始模拟走步
		SimulateMove(move);
		return true;
	}

	// 模拟着子，主要的作用是用于模拟下棋
	virtual void SimulateMove(const int& move)
	{
		// 结点的深度加1
		depth++;
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
	double getResult(const int& current_player_to_move)
	{
		return Winner == current_player_to_move ? 0.0 : 1.0;
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
};

class AI3 : public AI2
{
private:

public:

	virtual int maxandmin(const int& depth) override {
		initChessScore(true);
		int maxCount = 6;
		int bestMove;
		/*do {*/
			bestMove = predict();
		/*	--maxCount;
		} while (this->isGo2Dead(getLine(bestMove), getColumn(bestMove), turn2Who) && maxCount != 0);
		if (maxCount == 0) {
			bestMove = 0;
		}*/
		return bestMove;
	}
	int predict();
};

