#pragma once
#include "../ChessBoard_Header/parameter.h"
#include "../AI3_Header/MCTS.h"
#include "../AI4_Header/AI4.h"
#include <vector>
#include <iostream>


class SimulatorGo2 : public AIPlayer {
private:
	int cross[ChessEnd][ChessEnd];
public:
	mutable int Winner;
	int player_to_move;

	SimulatorGo2() :
		Winner(NoChess)
	{
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = NoChess;
			}
		}
	}

	SimulatorGo2(
		int b[ChessEnd][ChessEnd],
		int Id) :
		player_to_move(Id),
		Winner(NoChess)
	{
		this->turn2Who = Id;
		this->PlayerId = turn2Who;
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = b[i][j];
			}
		}
	}

	// �Ƿ��п����ӵ����ӵ�
	template<typename RandomEngine>
	bool doRandomMove(RandomEngine* engine)
	{
		AI4 state;
		std::vector<int> moves;
		bool Win;
		if (state.getMoves(moves, cross, player_to_move, Win)) {
			if (moves.empty()) {
				Winner = getRival(player_to_move);
				return false;
			}
			std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
			// ��ʼģ���߲�
			SimulateMove(moves[move_ind(*engine)]);
			return true;
		}
		else {
			if (Win) {
				Winner = getRival(player_to_move);
				return false;
			}
			else {
				SimulateMove(*moves.begin());
				return true;
			}
		}
		return false;
	}

	// ���������Ѽ����п��е����ӵ�
	virtual std::vector<int> getMoves() {
		// �������Ѽ����п��ܵ����ӵ㡣
		std::vector<int> moves;

		if (Winner == Black || Winner == White) {
			return moves;
		}

		AI4 state;
		bool Win;
		// �����true���Ǿ�ֱ�ӷ���
		if (state.getMoves(moves, cross, player_to_move, Win)) {
			return moves;
		}
		else {
			if (Win) {
				// ���WinΪtrue�Ļ������Ǳ�ʾ���ˣ���ֱ���������
				moves.clear();
				moves.swap(std::vector<int>());
				return moves;
			}
			else {
				// ���WinΪfalse�Ļ������Ǳ�ʾmoves���ҽ���һ���ӣ�������һ����ȷ���ģ�Ҳֱ��������ݡ�
				return moves;
			}
		}
	}

	// �����ж���Ӯ���
	virtual double getResult(const int& current_player_to_move)
	{
		// û��ģ�⵽��ͷ
		if (Winner != White && Black != Black) {
			return 0.5;
		}
		return Winner == current_player_to_move ? 1.0 : 0.0;
	}

	// ģ�����ӣ���Ҫ������������ģ������
	virtual void SimulateMove(const int& move)
	{
		// �����ʾģ��û�н�������ʱ������Ҫ�ж��Ƿ��Ѿ���Ӯ�۶���
		// ���û����Ӯδ�۶��Ļ����ͼ���ģ��
		cross[getLine(move)][getColumn(move)] = player_to_move;
		// showSimaluteInfo(getLine(move), getColumn(move));
		// �ֵ���һ���������
		player_to_move = getRival(player_to_move);
	}

	void showSimaluteInfo(const int& line, const int& column) {
		_cprintf("**************This is chess cross*********(%d, %d)***%s******\n",
			line, column, player_to_move == Black ? "Black" : "White");
		showChessBoard(cross);
		system("pause");

	}
};


class AI5 : public AI, public AIPlayer {
private:
public:
	AI5() :
		chessCount(0)
	{
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				cross[i][j] = NoChess;
			}
		}
	}
	void GetPosition(int &line, int &column, int onTurn) {
		//���յ������Զ������
		if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
		{
			//������Ӧ�����ڶ�AI�ļ��
			if (onTurn == 0)
			{
				// ���������¿�ʼ��Ϸ���������ù���
				line++;
				column++;
				for (int i = ChessStart; i < ChessEnd; ++i) {
					for (int j = ChessStart; j < ChessEnd; ++j) {
						cross[i][j] = NoChess;
					}
				}
			}
			//����һ������������ݽ��л��ˡ���100Ϊ���1��200Ϊ���2��line��columnΪ���˵�λ�ã�
			else if (onTurn == 100 || onTurn == 200)
			{
				rollback(line, column, onTurn / 100);
			}
			return;
		}
		/******************************************
		��ӵз����ӵ㵽�ҷ�������
		*******************************************/
		this->chessCount++;
		// ע�⴫�ݽ�����onTurn�����ǶԷ��ģ������Ǽ����ġ�
		initPlayerRole(onTurn);
		cross[line][column] = Rival;

		/******************************************
		�ҷ�����
		*******************************************/
		int temp = predict();
		line = getLine(temp);
		column = getColumn(temp);
		if (!OnChessBoard(line, column)) {
			line = 0;
			column = 0;
		}
		cross[line][column] = turn2Who;

		_cprintf("**************This is chess cross*******(%d, %d)***********\n", line, column);
		showChessBoard(cross);
	}
protected:
	int cross[10][10];
	int chessCount;

	void rollback(int line, int column, int onTurn) {
		if (onTurn == turn2Who) {
			--chessCount;
		}
		if (cross[line][column] != NoChess) {
			cross[line][column] = NoChess;
		}
	}

	void initPlayerRole(int onTurn) {
		Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
		this->turn2Who = getRival(Rival);
		this->PlayerId = turn2Who;
	}

	int predict() {
		AI4 state;
		std::vector<int> moves;
		bool Win;
		if (!state.getMoves(moves, cross, PlayerId, Win)) {
			return *moves.begin();
		}
		//else {
		//	std::mt19937_64 random_engine(1008611);// ����������ӣ���������߲�
		//	std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		//	// ��ʼģ���߲�
		//	return moves[move_ind(random_engine)];
		//}
		MCTS::ComputeOptions options;
		options.number_of_threads = 2;
		options.verbose = true;
		options.max_iterations = -1;
		options.max_time = 1;
		auto state_copy = new SimulatorGo2(cross, PlayerId);
		auto best_move = MCTS::computeNextMove(state_copy, options);
		return best_move;
	}
};

