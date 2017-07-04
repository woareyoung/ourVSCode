#pragma once
//
// Petter Strandmark 2013
// petter.strandmark@gmail.com
//
// Ϊ������Ϸ����д���ɿ�����
// Monte Carlo Tree Search for finite games.
//
// Originally based on Python code at
// http://mcts.ai/code/python.html
//
// ʹ���˸��Ĳ��л�����
// Uses the "root parallelization" technique [1].
#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/WinCheck.h"
#include <tuple>
#include <set>
#include <assert.h>

#define pass (-2)
#define no_move (-1)

namespace MCTS
{
	class SimulatorGo : public AI2 {
	private:
		mutable int board[ChessEnd][ChessEnd];
		mutable int simulatorScore[ChessEnd][ChessEnd];
		unsigned int previous_board_hash_value;
		std::set<unsigned int> all_hash_values;
		WinCheck::ChessInfo chessInfo;

		virtual int* getPatternType() override {
			return Type[player_to_move - 1];
		}
	public:
		int player_to_move;
		mutable int depth;
		SimulatorGo() :
			depth(0),
			previous_board_hash_value(0)
		{
			initAllArray();
			all_hash_values.insert(compute_hash_value());
		}

		SimulatorGo(int b[ChessEnd][ChessEnd], int Id) :
			player_to_move(Id),
			previous_board_hash_value(0),
			depth(0)
		{
			initAllArray();
			for (int i = ChessInit; i < ChessEnd; ++i) {
				for (int j = ChessInit; j < ChessEnd; ++j) {
					board[i][j] = b[i][j];
					simulatorScore[i][j] = chessScore[i][j];
				}
			}
		}

		void initSimulation() const {
			for (int i = ChessInit; i < ChessEnd; ++i) {
				for (int j = ChessInit; j < ChessEnd; ++j) {
					simulatorScore[i][j] = chessScore[i][j];
				}
			}
		}

		static int ij_to_ind(int i, int j)
		{
			return i >= ChessStart && j >= ChessStart && i < ChessEnd && j < ChessEnd ? 0 : 100 * i + j;
		}


		static std::pair<int, int> ind_to_ij(int ind)
		{
			return ind >= 0 && ind < 910 ? std::make_pair(0, 0) : std::make_pair(ind / 100, ind % 100);
		}

		// ����hashֵ
		virtual unsigned int compute_hash_value() const
		{
			unsigned int value = 0;
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					value = 65537 * value + board[i][j];
				}
			}
			return value;
		}

		// ����߲�
		template<typename RandomEngine>
		void do_random_move(RandomEngine* engine)
		{
			auto moves = get_moves();// ��ȡ�������ӵ����ӵ㼯��
			if (moves.empty()) {// ������ӵ㼯��Ϊ�յĻ�����ֱ�ӷ���
				return;
			}
			// �����ȡһ�����ӵ�
			std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
			auto move = moves[move_ind(*engine)];
			// ��ʼ�߲�
			do_move(move);
		}

		// �ֵ���һ����������
		virtual void do_move(int move)
		{
			// ������ȼ�1
			depth++;
			// �Է��Ľ��
			int opponent = getRival(player_to_move);
			// ֱ�������߲�
			if (move == pass) {
				player_to_move = opponent;
				return;
			}

			int i, j, Win;
			std::tie(i, j) = ind_to_ij(move);// Ԫ��
											 // ���
			if (chessInfo.WinOrLose(i, j, Win, player_to_move, board)) {
				return;
			}

			board[i][j] = player_to_move;

			// ���������в���֮ǰ�������ӵĹ�ϣֵ���Է����顣
			previous_board_hash_value = compute_hash_value();
			all_hash_values.insert(previous_board_hash_value);

			// �ֵ���һ���������
			player_to_move = getRival(player_to_move);
		}

		// �Ƿ��п����ӵ����ӵ�
		virtual bool has_moves() const
		{
			return !get_moves().empty();
		}

		// ���������Ѽ����п��е����ӵ�
		virtual std::vector<int> get_moves() const
		{
			// ����Pattern�Ե�ǰ������д��������п��ܵ����ӵ���뵽moves����
			const_cast<SimulatorGo*>(this)->Revalute();
			initSimulation();
			// �������Ѽ����п��ܵ����ӵ㡣
			std::vector<int> moves;
			// �����ȴ���1000��Ļ��ͣ�ֱ��������moves�ˡ�
			if (depth > 1000) {
				return moves;
			}
			// ���������ж϶Է��Ƿ����߲������ߣ�����еĻ�����˵��δ�ﵽ��Ϸ��ֹ�������
			bool opponent_has_move = false;
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					if (is_move_possible(i, j, player_to_move)) {
						moves.push_back(ij_to_ind(i, j));
					}
					// �������û�����ӵ�����£��Ǿͱ�ʾ��ǰ���дﵽ��Ϸ�����ľ���
					if (!opponent_has_move && is_move_possible(i, j, getRival(player_to_move))) {
						opponent_has_move = true;
					}
				}
			}
			if (moves.empty() && opponent_has_move) {
				moves.push_back(pass);
			}
			return moves;
		}

		// ���ƾ�������
		// ��Ҫ��ͨ�������۵����������ӵ�����
		virtual double get_result(int current_player_to_move) const
		{
			// ��ȡ��ҵĵ�ǰ����
			// ��������������⣬���Ǹ������ӵ��������۵��������и��ֵ�
			auto get_player_score = [&](int player)
			{
				int score = 0;
				for (int i = ChessStart; i < ChessEnd; ++i) {
					for (int j = ChessStart; j < ChessEnd; ++j) {
						if (board[i][j] == player) {
							score++;
						}
					}
				}
				return score += player == player_to_move ? ourEyes : RivalEyes;
			};
			int score1 = get_player_score(1);// ��ȡ�۵�����
			int score2 = get_player_score(2);// ��ȡ�۵�����

			if (score1 == score2) {
				return 0.5;
			}

			// �����۵������ƶ���˭��Ӯ
			// ������1���۵����������2�Ķ࣬��ô���1��Ӯ
			int winner = score1 > score2 ? 1 : 2;

			// ���Ӯ���ǵ�ǰ�ƶ���
			return winner == current_player_to_move ? 0.0 : 1.0;
		}

		virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) override {
			/******************************************
			�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
			����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
			*******************************************/
			for (int i = 0; i < start; ++i) {
				if (mainColor == Rival) {
					// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
					board[emptyPos[i].line][emptyPos[i].column] = Rival;
					if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
						board[emptyPos[i].line][emptyPos[i].column] = NoChess;
						// ����ǵз�����ɱ��Ļ������������   -.-������
						simulatorScore[emptyPos[i].line][emptyPos[i].column] = 0;
						return false;
					}
				}
				else if (mainColor == turn2Who) {
					// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
					board[x][y] = turn2Who;
					if (isGo2Dead(x, y, turn2Who)) {
						simulatorScore[x][y] = minLimit;
						board[x][y] = NoChess;
						// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
						return false;
					}
				}
				// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
				board[emptyPos[i].line][emptyPos[i].column] = NoChess;
			}
			return true;
		}
		// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
		virtual bool checkStone(int& x, int& y) override {
			// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			board[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				simulatorScore[x][y] = minLimit;
				board[x][y] = NoChess;
				// ��Ϊ������Ҫͳ���۵�������������ǰ�ľ��ƣ�����������Ҫ��¼��
				++this->RivalEyes;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (board[x][y] == NoChess && simulatorScore[x][y] == 0) return false;
			board[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				board[x][y] = NoChess;
				// ����ǵз�����ɱ��Ļ������������   -.-������
				simulatorScore[x][y] = 0;
				++this->ourEyes;
				return false;
			}
			// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
			board[x][y] = NoChess;
			return true;
		}

		virtual bool is_move_possible(int i, int j) const
		{
			return is_move_possible(i, j, player_to_move);
		}

		virtual bool is_move_possible(const int i, const int j, const int player) const
		{
			const int opponent = getRival(player);// ����

												  // �ж��Ƿ���������
			if (OnChessBoard(i, j)) {
				// �����ǰ���ѡ������ӵ��������ӵĻ�����ֱ�ӷ���
				if (board[i][j] != NoChess) {
					return false;
				}
				// �жϵ�ǰ���߲��Ƿ�ᵼ�½��������������Ϸ�����Ļ���ֱ�ӷ��ء�
				int winner, ii = i, jj = j, rival = opponent;
				if (chessInfo.WinOrLose(ii, jj, rival, winner, board)) {
					return false;
				}
				// �����ʾѡ�������ӵ���û������ġ�
				board[i][j] = player;


				bool possible = false;
				if (simulatorScore[i][j] != minLimit) {
					possible = true;
				}

				if (possible) {
					if (compute_hash_value() == previous_board_hash_value) {
						possible = false;
					}
					else if (all_hash_values.find(compute_hash_value()) != all_hash_values.end()) {
						possible = false;
					}
				}

				board[i][j] = NoChess;
				return possible;
			}
			else {
				// û�п��е����ӵ�
				return false;
			}
		}
	};

	struct ComputeOptions// ����ѡ��
	{
		int number_of_threads;// �߳�����
		int max_iterations;// ����������
		double max_time;// ���ʱ��
		bool verbose;// �Ƿ�����

		ComputeOptions() :
			number_of_threads(8),// Ĭ�ϵ��߳�������8��
			max_iterations(10000),// Ĭ�ϵ����ĵ���������10000
			max_time(-1.0), // default is no time limit.Ĭ����û��ʱ�����Ƶ�
			verbose(false)
		{ }
	};

	/**
	* �Ʋ��һ�������ӵ�
	* @param root_state ���̵�����
	* @param options �������Ĳ������ã��������̺߳����
	*/
	template<typename State>
	int compute_move(const State root_state,
		const ComputeOptions options = ComputeOptions());
}
//
//
// [1] Chaslot, G. M. B., Winands, M. H., & van Den Herik, H. J. (2008).
//     Parallel monte-carlo tree search. In Computers and Games (pp.
//     60-71). Springer Berlin Heidelberg.
//

#include <algorithm>
#include <cstdlib>
#include <future>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <conio.h>


// ���ؿ�������������Monte Carlo Tree Search
namespace MCTS
{
	using std::cerr;
	using std::endl;
	using std::vector;
	using std::size_t;

	// ������������Ϣ
#define showInfoOnDOS(info) if(info != NULL){_cprintf("%s\n", info);}

	// ����������ڹ����ɿ������ģ��ɿ������ĸ�������û�������ʣ��������Ľ�㶼��ֱ����ӵġ�
	template<typename State>
	class Node
	{
	public:

		Node(const State& state);
		~Node();

		// �����ʾ���ǣ�
		// �Ƿ��б����չ���˵ĵ��ǲ�û�г��Թ������ӵ�
		bool has_untried_moves() const;
		template<typename RandomEngine>
		// �����ȡδ�������������ӷ���
		int get_untried_move(RandomEngine* engine) const;
		Node* best_child() const;// ���ŵ����ӵ�

		bool has_children() const// �Ƿ����ӽڵ�
		{
			return !children.empty();
		}

		Node* select_child_UCT() const;// ʹ��UTC�㷨��ѡ�����ŵ��ӽڵ�
		Node* add_child(const int& move, const State& state);// ����ӽڵ�
		void update(double result);// ���¸��ڵ����Ϣ

		std::string to_string() const;
		std::string tree_to_string(int max_depth = 1000000, int indent = 0) const;

		const int move;// 
		Node* const parent;// ���ڵ�
		const int player_to_move;// ��ǰִ�ӵ����

		double wins;// ����
		int visits;// ���ʴ���

		std::vector<int> moves;// �����ƶ�·��
		std::vector<Node*> children; // �����ӽڵ�

	private:
		Node(const State& state, const int& move, Node* parent);

		std::string indent_string(int indent) const;// �����ַ���

		Node(const Node&);

		double UCT_score;// UCT����
	};

	template<typename State>
	Node<State>::Node(const State& state) :
		move(no_move),// ��ʼ��Ϊû���߲�
		parent(nullptr),// ��ʼ��Ϊ��
		player_to_move(state->player_to_move),// ��ʼ��Ϊ��ǰ���һ���Ľ�ɫ
		wins(0),
		visits(0),
		moves(state->get_moves()),// �ɹ����ӵ����ӵ㼯
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::Node(const State& state, const int& move_, Node* parent_) :
		move(move_),
		parent(parent_),
		player_to_move(state->player_to_move),
		wins(0),
		visits(0),
		moves(state->get_moves()),
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::~Node()
	{
		for (auto child : children) {// ���ӽڵ�ռ�õ��ڴ�����ͷ�
			delete child;
		}
	}

	template<typename State>
	bool Node<State>::has_untried_moves() const// �Ƿ���û�б��������ӽڵ�
	{
		return !moves.empty();
	}

	// ��ȡδ�����Թ������ӵ�
	template<typename State>
	template<typename RandomEngine>
	int Node<State>::get_untried_move(RandomEngine* engine) const// ��û�б��������ӽڵ��л�ȡһ�������Ϊ����չ���Ľ��
	{
		if (moves.empty()) {
			showInfoOnDOS("error,get_untried_move -> moves vector is empty!");
			return pass;
		}
		// ����[0, move.size() - 1]��Χ��ʼ���ֲ���
		std::uniform_int_distribution<std::size_t> moves_distribution(0, moves.size() - 1);
		return moves[moves_distribution(*engine)];
	}

	// ���ŵ����ӵ�
	template<typename State>
	Node<State>* Node<State>::best_child() const
	{
		if (moves.empty()) {
			showInfoOnDOS("error, best_child -> moves vector is empty!");
		}
		if (children.empty()) {
			showInfoOnDOS("error, best_child -> node`s children is none!");
		}

		return *std::max_element(children.begin(), children.end(),
			[](Node* a, Node* b) { return a->visits < b->visits; });;
	}

	/**
	* ʹ��UTC�㷨ѡ��UTC�������ŵ�һ���ӽڵ㲢����
	* upper confidence bounds applied to trees
	* ԭ�ľWַ��https://read01.com/NyRDaN.html
	*/
	template<typename State>
	Node<State>* Node<State>::select_child_UCT() const
	{
		if (moves.empty()) {
			showInfoOnDOS("error, select_child_UCT -> moves vector is empty!");
		}
		for (auto child : children) {
			child->UCT_score = double(child->wins) / double(child->visits) +
				2.0 * std::sqrt(std::log(double(this->visits)) / child->visits);
		}

		return *std::max_element(children.begin(), children.end(),
			[](Node* a, Node* b) { return a->UCT_score < b->UCT_score; });
	}

	/**
	* ����ӽڵ�
	* @param move ��ǰ���ӵ�
	* @param state ��ǰ���̵�״̬�����ƣ�
	*/
	template<typename State>
	Node<State>* Node<State>::add_child(const int& move, const State& state)
	{
		// �½��µĽ�㣬���µĽ����ӵ�children�У����ж�children�����Ƿ�Ϊ��
		auto node = new Node(state, move, this);
		children.push_back(node);
		if (children.empty()) {
			showInfoOnDOS("error, node`s children is empty!");
		}

		// �ӵ�һ�����ӵ㿪ʼ
		auto itr = moves.begin();
		// �����޸����ӵ�����Ĵ�С
		for (; itr != moves.end() && *itr != move; ++itr);
		if (itr != moves.end()) {
			showInfoOnDOS("this move is not end!");
		}
		moves.erase(itr);// ��moves������ɾ��moveԪ��
		return node;
	}

	/**
	* ���½��ķ��ʴ����ͽ���wins
	* @param result ��ʾwin����
	*/
	template<typename State>
	void Node<State>::update(double result)
	{
		visits++;
		wins += result;
	}

	template<typename State>
	std::string Node<State>::to_string() const
	{
		std::stringstream sout;
		sout << "["
			<< "P" << 3 - player_to_move << " "
			<< "M:" << move << " "
			<< "W/V: " << wins << "/" << visits << " "
			<< "U: " << moves.size() << "]\n";
		return sout.str();
	}

	template<typename State>
	std::string Node<State>::tree_to_string(int max_depth, int indent) const
	{
		if (indent >= max_depth) {
			return "";
		}

		std::string s = indent_string(indent) + to_string();
		for (auto child : children) {
			s += child->tree_to_string(max_depth, indent + 1);
		}
		return s;
	}

	template<typename State>
	std::string Node<State>::indent_string(int indent) const
	{
		std::string s = "";
		for (int i = 1; i <= indent; ++i) {
			s += "| ";
		}
		return s;
	}

	/**
	* �������������ѡ��һ�����������չ
	* @param root_state ������������
	* @param options  ���Ĳ�������
	* @param initial_seed �������ʼ������
	* @return ���ظ����
	*/
	template<typename State>
	std::unique_ptr<Node<State>>  compute_tree(
		const State root_state,
		const ComputeOptions options,
		std::mt19937_64::result_type initial_seed)
	{
		std::mt19937_64 random_engine(initial_seed);// ����������ӣ���������߲�

		if (options.max_iterations >= 0 || options.max_time >= 0) {
			showInfoOnDOS("option is right~");
		}
		if (!(root_state->player_to_move == 1 || root_state->player_to_move == 2)) {
			showInfoOnDOS("player_to_move is error~");
			assert(root_state->player_to_move == 1 || root_state->player_to_move == 2);	
		}
		// rootָ�����һ��Node������
		auto root = std::unique_ptr<Node<State>>(new Node<State>(root_state));

		// ���������ı���������ComputeOption�����ֵ
		// �����̽���UTC�ӷִ���
		for (int iter = 1; iter <= options.max_iterations/*���ĵ�������*/ ||
			options.max_iterations < 0; ++iter) {
			auto node = root.get();
			State state = root_state;

			// �����ʾ�������п����ߵ��߲�ȫ��������Ϊ���ӽ���ˣ�
			// �����ڷǺ��ӽ����߲�move
			// node��û�г��Ե�moveΪ���ˣ�����node����Ҷ�ӽ��
			// �����ʾ���������еĺ��ӽ�㶼���ʹ��ˣ��������Ǿ���UTC���㷨�Խ����й��֡�
			while (!node->has_untried_moves() && node->has_children()) {
				node = node->select_child_UCT();
				state->do_move(node->move);
			}

			// ���ѡȡһ�������
			if (node->has_untried_moves()) {
				auto move = node->get_untried_move(&random_engine);
				state->do_move(move);
				node = node->add_child(move, state);
			}

			// Okay��ѡȡ�����㣬���ǾͿ�ʼ�ܣ�ѭ��ֱ��������
			while (state->has_moves()) {
				state->do_random_move(&random_engine);
			}

			// �����Ѿ����������ĸ�����ˣ����ʱ�����ǵݹ�ظ���㣬
			// Ϊ��һ��ѭ����UTC�ӷ���׼��
			while (node != nullptr) {
				node->update(state->get_result(node->player_to_move));
				node = node->parent;
			}

		}

		return root;
	}


	/**
	* �Ʋ���Ƚ��ŵ����ӵ�
	* @param root_state ���̵�����
	* @param options �������Ĳ������ã��������̺߳����
	* @return �������ŵ����ӵ�
	*/
	template<typename State>
	int compute_move(const State root_state,
		const ComputeOptions options)
	{
		using namespace std;

		auto moves = root_state->get_moves();// ��ȡ���еĿ����ߵ����ӵ�
		if (moves.size() <= 0) {
			showInfoOnDOS("moves.size() is zero");
			assert(moves.size() > 0);
		}
		if (moves.size() == 1) {
			return moves[0];
		}

		// �ַ����е�����ȥ����������������õ����첽�̵߳ķ�ʽ��������
		vector<future<unique_ptr<Node<State>>>> root_futures;
		ComputeOptions job_options = options;
		job_options.verbose = false;
		for (int t = 0; t < options.number_of_threads; ++t) {
			auto func = [t, &root_state, &job_options]() -> std::unique_ptr<Node<State>> // ָ������
			{
				return compute_tree(root_state, job_options, 1012411 * t + 12515);
			};
			// ʹ���첽�߳����������ݡ����ַ�����
			root_futures.push_back(std::async(std::launch::async, func));
		}

		// �Ѽ����
		vector<unique_ptr<Node<State>>> roots;
		for (int t = 0; t < options.number_of_threads; ++t) {
			// get�ȴ��첽�������������ؽ��
			roots.push_back(std::move(root_futures[t].get()));
		}

		// �ϲ����и����ĺ��ӽ��
		map<int, int> visits;
		map<int, double> wins;
		long long games_played = 0;
		// UTC�ϲ��ӷ�
		for (int t = 0; t < options.number_of_threads; ++t) {
			auto root = roots[t].get();
			games_played += root->visits;
			for (auto child = root->children.cbegin(); child != root->children.cend(); ++child) {
				visits[(*child)->move] += (*child)->visits;
				wins[(*child)->move] += (*child)->wins;
			}
		}

		// Ѱ����߷ֵĽ��
		double best_score = -1;
		int best_move = int();
		for (auto itr : visits) {
			auto move = itr.first;
			double v = itr.second;
			double w = wins[move];
			// Expected success rate assuming a uniform prior (Beta(1, 1)).
			// https://en.wikipedia.org/wiki/Beta_distribution
			double expected_success_rate = (w + 1) / (v + 2);
			if (expected_success_rate > best_score) {
				best_move = move;
				best_score = expected_success_rate;
			}

			if (options.verbose) {
				cerr << "MOVE: " << itr.first
					<< " (" << setw(2) << right << int(100.0 * v / double(games_played) + 0.5) << "% visits)"
					<< " (" << setw(2) << right << int(100.0 * w / v + 0.5) << "% wins)" << endl;
			}
		}

		if (options.verbose) {
			auto best_wins = wins[best_move];
			auto best_visits = visits[best_move];
			cerr << "----" << endl;
			cerr << "Best: " << best_move
				<< " (" << 100.0 * best_visits / double(games_played) << "% visits)"
				<< " (" << 100.0 * best_wins / best_visits << "% wins)" << endl;
		}

		return best_move;
	}

}

