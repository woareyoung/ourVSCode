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
	typename State::Move compute_move(const State root_state,
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

#ifdef USE_OPENMP
#include <omp.h>
#endif

// ���ؿ�������������Monte Carlo Tree Search
namespace MCTS
{
	using std::cerr;
	using std::endl;
	using std::vector;
	using std::size_t;

	static void check(bool expr, const char* message);
	static void assertion_failed(const char* expr, const char* file, int line);

#define attest(expr) if (!(expr)) { ::MCTS::assertion_failed(#expr, __FILE__, __LINE__); }
#ifndef NDEBUG
#define dattest(expr) if (!(expr)) { ::MCTS::assertion_failed(#expr, __FILE__, __LINE__); }
#else
#define dattest(expr) ((void)0)
#endif

	//
	// ����������ڹ����ɿ������ģ��ɿ������ĸ�������û�������ʣ��������Ľ�㶼��ֱ����ӵġ�
	// This class is used to build the game tree. The root is created by the users and
	// the rest of the tree is created by add_node.
	//
	template<typename State>
	class Node
	{
	public:
		typedef typename State::Move Move;// �ƶ�λ�ã�������

		Node(const State& state);
		~Node();

		bool has_untried_moves() const;// �Ƿ���δ�����Թ������ӷ���
		template<typename RandomEngine>
		Move get_untried_move(RandomEngine* engine) const;// �����ȡδ�����Թ������ӷ���
		Node* best_child() const;

		bool has_children() const// �Ƿ����ӽڵ�
		{
			return !children.empty();
		}

		Node* select_child_UCT() const;
		Node* add_child(const Move& move, const State& state);
		void update(double result);

		std::string to_string() const;
		std::string tree_to_string(int max_depth = 1000000, int indent = 0) const;

		const Move move;
		Node* const parent;
		const int player_to_move;

		//std::atomic<double> wins;
		//std::atomic<int> visits;
		double wins;// ����
		int visits;// ���ʴ���

		std::vector<Move> moves;// �����ƶ�·��
		std::vector<Node*> children; // �����ӽڵ�

	private:
		Node(const State& state, const Move& move, Node* parent);

		std::string indent_string(int indent) const;// �����ַ���

		Node(const Node&);

		double UCT_score;
	};


	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////


	template<typename State>
	Node<State>::Node(const State& state) :
		move(State::no_move),
		parent(nullptr),
		player_to_move(state.player_to_move),
		wins(0),
		visits(0),
		moves(state.get_moves()),
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::Node(const State& state, const Move& move_, Node* parent_) :
		move(move_),
		parent(parent_),
		player_to_move(state.player_to_move),
		wins(0),
		visits(0),
		moves(state.get_moves()),
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::~Node()
	{
		for (auto child : children) {
			delete child;
		}
	}

	template<typename State>
	bool Node<State>::has_untried_moves() const
	{
		return !moves.empty();
	}

	// ��ȡδ�����Թ������ӵ�
	template<typename State>
	template<typename RandomEngine>
	typename State::Move Node<State>::get_untried_move(RandomEngine* engine) const
	{
		attest(!moves.empty());
		// ����[0, move.size() - 1]��Χ��ʼ���ֲ���
		std::uniform_int_distribution<std::size_t> moves_distribution(0, moves.size() - 1);
		return moves[moves_distribution(*engine)];
	}

	// ���ŵ����ӵ�
	template<typename State>
	Node<State>* Node<State>::best_child() const
	{
		attest(moves.empty());
		attest(!children.empty());

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
		attest(!children.empty());// ����ӽڵ㲻Ϊ�յĻ�����
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
	Node<State>* Node<State>::add_child(const Move& move, const State& state)
	{
		// �½��µĽ�㣬���µĽ����ӵ�children�У����ж�children�����Ƿ�Ϊ��
		auto node = new Node(state, move, this);
		children.push_back(node);
		attest(!children.empty());

		// �ӵ�һ�����ӵ㿪ʼ
		auto itr = moves.begin();
		// �����޸����ӵ�����Ĵ�С
		for (; itr != moves.end() && *itr != move; ++itr);
		attest(itr != moves.end());
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

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////

	/**
	* �������������ѡ��һ�����������չ
	* @param root_state ������������
	* @param options  ���Ĳ�������
	* @param initial_seed �������ʼ������
	* @return ���ظ����
	*/
	template<typename State>
	std::unique_ptr<Node<State>>  compute_tree(const State root_state,
		const ComputeOptions options,
		std::mt19937_64::result_type initial_seed)
	{
		std::mt19937_64 random_engine(initial_seed);// �������

		attest(options.max_iterations >= 0 || options.max_time >= 0);
		if (options.max_time >= 0) {
#ifndef USE_OPENMP
			throw std::runtime_error("ComputeOptions::max_time requires OpenMP.");
#endif
		}

		attest(root_state.player_to_move == 1 || root_state.player_to_move == 2);
		// rootָ�����һ������
		auto root = std::unique_ptr<Node<State>>(new Node<State>(root_state));

#ifdef USE_OPENMP
		double start_time = ::omp_get_wtime();
		double print_time = start_time;
#endif

		// ���� 
		for (int iter = 1; iter <= options.max_iterations/*���ĵ�������*/
			|| options.max_iterations/*�����߳���*/ < 0; ++iter) {
			auto node = root.get();
			State state = root_state;

			// ѡ��һ��ֱ��Ҷ�ӵ�·��
			// �ж��Ƿ����������߲��ҽ���Ƿ��к��ӽ�㣬����еĻ��ͼ���ѭ����ֱ�����
			// --------------------------����������-------------------------
			while (!node->has_untried_moves() && node->has_children()) {
				node = node->select_child_UCT();
				state.do_move(node->move);
			}

			// ������ǻ�û�д������״̬����ô��һ���½ڵ�չ�������ƶ������ 
			if (node->has_untried_moves()) {
				auto move = node->get_untried_move(&random_engine);
				state.do_move(move);
				node = node->add_child(move, state);
			}

			// ѭ��ֱ��������
			while (state.has_moves()) {
				state.do_random_move(&random_engine);
			}

			// �����Ѿ����������ĸ�����ˣ����ʱ�����ǵݹ�ظ����
			while (node != nullptr) {
				node->update(state.get_result(node->player_to_move));
				node = node->parent;
			}

#ifdef USE_OPENMP
			if (options.verbose || options.max_time >= 0) {
				double time = ::omp_get_wtime();
				if (options.verbose && (time - print_time >= 1.0 || iter == options.max_iterations)) {
					std::cerr << iter << " games played (" << double(iter) / (time - start_time) << " / second)." << endl;
					print_time = time;
				}

				if (time - start_time >= options.max_time) {
					break;
				}
			}
#endif
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
	typename State::Move compute_move(const State root_state,
		const ComputeOptions options)
	{
		using namespace std;

		attest(root_state.player_to_move == 1 || root_state.player_to_move == 2);

		auto moves = root_state.get_moves();
		attest(moves.size() > 0);
		if (moves.size() == 1) {
			return moves[0];
		}

#ifdef USE_OPENMP
		double start_time = ::omp_get_wtime();
#endif

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
		map<typename State::Move, int> visits;
		map<typename State::Move, double> wins;
		long long games_played = 0;
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
		typename State::Move best_move = typename State::Move();
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
				cerr << "Move: " << itr.first
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

#ifdef USE_OPENMP
		if (options.verbose) {
			double time = ::omp_get_wtime();
			std::cerr << games_played << " games played in " << double(time - start_time) << " s. "
				<< "(" << double(games_played) / (time - start_time) << " / second, "
				<< options.number_of_threads << " parallel jobs)." << endl;
		}
#endif

		return best_move;
	}

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////


	static void check(bool expr, const char* message)
	{
		if (!expr) {
			throw std::invalid_argument(message);
		}
	}


	// ����ʧ��
	static void assertion_failed(const char* expr, const char* file_cstr, int line)
	{
		using namespace std;

		// Extract the file name only.
		string file(file_cstr);
		auto pos = file.find_last_of("/\\");
		if (pos == string::npos) {
			pos = 0;
		}
		file = file.substr(pos + 1);  // Returns empty string if pos + 1 == length.

		stringstream sout;
		sout << "Assertion failed: " << expr << " in " << file << ":" << line << ".";
		throw runtime_error(sout.str().c_str());
	}

}

