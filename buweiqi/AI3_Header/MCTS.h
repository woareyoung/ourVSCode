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

namespace MCTS
{
	struct ComputeOptions// ����ѡ��
	{
		int number_of_threads;// �߳�����
		int max_iterations;// ����������
		double max_time;// ���ʱ��
		bool verbose;// �Ƿ���ʾDos��Ϣ

		ComputeOptions() :
			number_of_threads(8),// Ĭ�ϵ��߳�������8��
			max_iterations(100000),// Ĭ�ϵ����ĵ���������10000
			max_time(-1.0), // Ĭ����û��ʱ�����Ƶ�
			verbose(false)
		{ }
	};

	/**
	* �Ʋ��һ�������ӵ�
	* @param root_state ���̵�����
	* @param options �������Ĳ������ã��������̺߳����
	*/
	template<typename State>
	int computeNextMove(const State root_state,
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

#define no_move (-1)

#include <omp.h>

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
	protected:

	public:

		Node(const State& state);
		~Node();

		// �����ʾ���ǣ�
		// �Ƿ��б����չ���˵ĵ��ǲ�û�г��Թ������ӵ�
		bool hasUntriedMoves() const;
		template<typename RandomEngine>
		// �����ȡδ�������������ӷ���
		int getRandomMove(RandomEngine* engine) const;
		Node* bestChild() const;// ���ŵ����ӵ�

		bool has_children() const// �Ƿ����ӽڵ�
		{
			return !children.empty();
		}

		Node* selectChildByUCT() const;// ʹ��UTC�㷨��ѡ�����ŵ��ӽڵ�
		Node* addChild(const int& move, const State& state);// ����ӽڵ�
		void update(double result);// ���¸��ڵ����Ϣ

		const int move;// 
		Node* const parent;// ���ڵ�
		const int player_to_move;// ��ǰִ�ӵ����

		double wins;// ����
		int visits;// ���ʴ���

		std::vector<int> moves;// �����ƶ�·��
		std::vector<Node*> children; // �����ӽڵ�

		std::string to_string() const;
		std::string tree2String(int max_depth = 1000000, int indent = 0) const;

	private:
		Node(const State& state, const int& move, Node* parent);

		std::string indentString(int indent) const;// �����ַ���

		Node(const Node&);

		double UCT_score;// UCT����
	};

	template<typename State>
	Node<State>::Node(const State& state) :
		move(no_move),// ��ʼ��Ϊû���߲�
		parent(nullptr),// ��ʼ��Ϊ��
		player_to_move(state->getRole()),// ��ʼ��Ϊ��ǰ���һ���Ľ�ɫ
		wins(0),
		visits(0),
		moves(state->getMoves()),// �ɹ����ӵ����ӵ㼯
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::Node(const State& state, const int& move_, Node* parent_) :
		move(move_),
		parent(parent_),
		player_to_move(state->getRole()),
		wins(0),
		visits(0),
		moves(state->getMoves()),
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::~Node()
	{
		int temp = children.size();
		for (int i = 0; i < temp; ++i) {
			delete children[i];
		}
		children.clear();
		children.swap(vector<Node*>());
	}

	template<typename State>
	bool Node<State>::hasUntriedMoves() const// �Ƿ���û�б��������ӽڵ�
	{
		return !moves.empty();
	}

	// ��ȡδ�����Թ������ӵ�
	template<typename State>
	template<typename RandomEngine>
	int Node<State>::getRandomMove(RandomEngine* engine) const// ��û�б��������ӽڵ��л�ȡһ�������Ϊ����չ���Ľ��
	{
		// ���ȷֲ���ȡ�����
		if (moves.size() == 0) {
			return 0;
		}
		std::uniform_int_distribution<std::size_t> movesDist(0, moves.size() - 1);
		return moves[movesDist(*engine)];
	}

	// ���ŵ����ӵ�
	template<typename State>
	Node<State>* Node<State>::bestChild() const
	{
		return *std::max_element(children.begin(), children.end(),
			[](Node* a, Node* b) { return a->visits < b->visits; });;
	}

	/**
	* ʹ��UTC�㷨ѡ��UTC�������ŵ�һ���ӽڵ㲢����
	* upper confidence bounds applied to trees
	* ԭ�ľWַ��https://read01.com/NyRDaN.html
	*/
	template<typename State>
	Node<State>* Node<State>::selectChildByUCT() const
	{
		// �������UCT����
		for (auto child : children) {
			child->UCT_score = (double)(child->visits - 2 * child->wins) / double(child->visits)
				+ std::sqrt(2.0 * std::log(double(this->visits)) / child->visits);
		}

		// ѡȡӵ����������Ͻ���ӽڵ�
		return *std::max_element(children.begin(), children.end(),
			[](Node* a, Node* b) { return a->UCT_score < b->UCT_score; });
	}

	/**
	* ����ӽڵ�
	* @param move ��ǰ���ӵ�
	* @param state ��ǰ���̵�״̬�����ƣ�
	*/
	template<typename State>
	Node<State>* Node<State>::addChild(const int& move, const State& state)
	{
		// �½��µĽ�㣬���µĽ����ӵ�children�У����ж�children�����Ƿ�Ϊ��
		auto node = new Node(state, move, this);
		children.emplace_back(node);
		// �����moves�а�����move�Ļ����Ͱ�moveȥ����
		auto itr = moves.begin();
		for (; itr != moves.end() && *itr != move; ++itr);
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
	std::string Node<State>::tree2String(int max_depth, int indent) const
	{
		if (indent >= max_depth) {
			return "";
		}

		std::string s = indentString(indent) + to_string();
		for (auto child : children) {
			s += child->tree2String(max_depth, indent + 1);
		}
		return s;
	}

	template<typename State>
	std::string Node<State>::indentString(int indent) const
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
	std::unique_ptr<Node<State>> computeMSTCTree(
		const State root_state,
		const ComputeOptions options,
		std::mt19937_64::result_type initial_seed)
	{
		std::mt19937_64 random_engine(initial_seed);// ����������ӣ���������߲�

		// rootָ�����һ��Node������
		auto root = std::unique_ptr<Node<State>>(new Node<State>(root_state));

		double start_time = ::omp_get_wtime();
		double print_time = start_time;

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
			while (!node->hasUntriedMoves() && node->has_children()) {
				node = node->selectChildByUCT();
				state->SimulateMove(node->move);
			}

			// ���ѡȡһ�������
			if (node->hasUntriedMoves()) {
				auto move = node->getRandomMove(&random_engine);// ���ѡȡһ�����
				state->SimulateMove(move);// ģ����һ��
				node = node->addChild(move, state);
			}

			// Okay��ѡȡ�����㣬���ǾͿ�ʼ�ܣ�ѭ��ֱ��������
			while (state->doRandomMove(&random_engine)) {}

			// �����Ѿ����������ĸ�����ˣ����ʱ�����ǵݹ�ظ���㣬
			// Ϊ��һ��ѭ����UTC�ӷ���׼��
			while (node != nullptr) {
				node->update(state->getResult(node->player_to_move));// ����ֻ����Ӯ�ͺ���
				node = node->parent;
			}

			if (options.verbose || options.max_time >= 0) {
				double time = ::omp_get_wtime();
				std::stringstream sout;
				if (options.verbose && (time - print_time >= 1.0 || iter == options.max_iterations)) {
					sout << iter << " games played (" << double(iter) / (time - start_time) << " / second).\n";
					_cprintf("%s", sout.str().c_str());
					print_time = time;
				}

				if (time - start_time >= options.max_time) {
					break;
				}
			}

		}
		//if (options.verbose) {
		//	auto node = root.get();
		//	_cprintf("*******compute tree:*******\n %s\n", node->tree2String().c_str());
		//	// system("pause");
		//}

		return root;
	}


	/**
	* �Ʋ���Ƚ��ŵ����ӵ�
	* @param root_state ���̵�����
	* @param options �������Ĳ������ã��������̺߳����
	* @return �������ŵ����ӵ�
	*/
	template<typename State>
	int computeNextMove(const State root_state,
		const ComputeOptions options)
	{
		using namespace std;

		auto moves = root_state->getMoves();// ��ȡ���еĿ����ߵ����ӵ�
		if (options.verbose) {
			if (moves.size() == 1) {
				return moves[0];
			}
		}

		double start_time = ::omp_get_wtime();

		// �ַ����е�����ȥ����������������õ����첽�̵߳ķ�ʽ��������
		vector<future<unique_ptr<Node<State>>>> root_futures;
		ComputeOptions job_options = options;
		job_options.verbose = false;
#pragma omp parallel for private(root_futures)
		for (int t = 0; t < options.number_of_threads; ++t) {
			auto func = [t, &root_state, &job_options]() -> std::unique_ptr<Node<State>> // ָ������
			{
				return computeMSTCTree(root_state, job_options, 95279527 * t + 12580);
			};
			// ʹ���첽�߳����������ݡ����ַ�����
			root_futures.emplace_back(std::async(std::launch::async, func));
		}

		// �Ѽ����
		vector<unique_ptr<Node<State>>> roots;
		for (int t = 0; t < options.number_of_threads; ++t) {
			roots.emplace_back(std::move(root_futures[t].get()));
		}

		// �ϲ����и����ĺ��ӽ��
		map<int, int> visits;
		map<int, double> wins;

		// ��ʼ����������
		for (int move : moves) {
			visits[move] = 0;
			wins[move] = 0.0;
		}

		long long games_played = 0;
		map<int, int>::iterator visitsItr;
		map<int, double>::iterator winsItr;
		// UTC�ϲ��ӷ�
		for (int t = 0; t < options.number_of_threads; ++t) {
			auto root = roots[t].get();
			games_played += root->visits;
			for (auto child = root->children.cbegin(); child != root->children.cend(); ++child) {
				visitsItr = visits.find((*child)->move);
				if (visitsItr != visits.end()) {
					(*visitsItr).second += (*child)->visits;
					_cprintf("move and visits:[%d, %d] ", (*child)->move, (*child)->visits);
				}
				winsItr = wins.find((*child)->move);
				if (winsItr != wins.end()) {
					(*winsItr).second += (*child)->wins;
					_cprintf("move and wins:[%d, %lf] \n", (*child)->move, (*child)->wins);
				}
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
			// �������ܹ�������Ӯһ�ξͺܺ��ˡ�
			double expected_success_rate = (w + 1) / (v + 2);
			if (expected_success_rate > best_score) {
				best_move = move;
				best_score = expected_success_rate;
			}

			// ���UCT����
			if (options.verbose) {
				_cprintf("MOVE: %d  (%f %%visits) (%f %%wins) \n",
					itr.first,
					100.0 * v / double(games_played),
					100.0 * w / v + 0.5);
			}
		}

		// ������ŵ�UCT����
		if (options.verbose) {
			auto best_wins = wins[best_move];
			auto best_visits = visits[best_move];
			_cprintf("---- \n Best: %d  (%f  %%visits) (%f %%wins) \n",
				best_move,
				100.0 * best_visits / double(games_played),
				100.0 * best_wins / best_visits + 0.5);
		}

		if (options.verbose) {
			double time = ::omp_get_wtime();
			std::stringstream sout;
			sout << games_played << " games played in " << double(time - start_time) << " s. "
				<< "(" << double(games_played) / (time - start_time) << " / second, "
				<< options.number_of_threads << " parallel jobs).\n";
			_cprintf("%s", sout.str().c_str());
		}
		return best_move;
	}
}
