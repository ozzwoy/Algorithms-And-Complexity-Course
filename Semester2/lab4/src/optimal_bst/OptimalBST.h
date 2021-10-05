#pragma once

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

//Клас, що описує оптимальне бінарне дерево пошуку
template <typename T>
class OptimalBST {
private:
	//Структура, що описує вузол дерева
	struct Node {
		T data;
		Node* parent, *left, *right;

		Node(T const &data, Node* parent, Node* left, Node* right) : data(data), parent(parent), left(left), right(right) {}
		Node(T const &data) : data(data), parent(nullptr), left(nullptr), right(nullptr) {}
	};

	//Можливі стани вершини: корінь, лівий потомок, правий потомок
	enum class Status { ROOT, LEFT, RIGHT };

private:
	Node* root;
	unsigned size;
	double cost;

	Node* find(T const &key) const {
		Node* current = root;

		while (current) {
			if (key == current->data) {
				break;
			}
			else if (key < current->data) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		return current;
	}

	Node* findSuccessor(Node* current) const {
		if (!current) {
			return nullptr;
		}

		Node* successor = current->right;
		if (successor) {
			while (successor->left) {
				successor = successor->left;
			}
		}
		return successor;
	}

	void print(Node* current, unsigned level = 0, Status stat = Status::ROOT) const {
		if (current) {
			for (unsigned i = 0; i < level; i++) {
				std::cout << "  ";
			}
			std::cout << level;

			switch (stat) {
			case Status::ROOT:
				std::cout << "(o)";
				break;
			case Status::LEFT:
				std::cout << "(l)";
				break;
			default:
				std::cout << "(r)";
			}

			std::cout << ": " << current->data << std::endl;
			print(current->left, level + 1, Status::LEFT);
			print(current->right, level + 1, Status::RIGHT);
		}
	}

	//Вираховує оптимальну конфігурацію дерева, тобто таку, що мат. сподівання глибини пошуку мінімальне
	//Повертає масив коренів оптимальних піддерев
	vector<vector<unsigned>> optimize(vector<double> const &true_probs, vector<double> const &false_probs) {
		unsigned n = true_probs.size();
		//Масив мат. сподівань глибини пошуку в оптимальних піддеревах та масив сум усіх вірогідностей у відповідних піддеревах
		vector<vector<double>> means(n + 2), probs(n + 2);
		vector<vector<unsigned>> roots(n);

		for (unsigned i = 0; i <= n + 1; i++) {
			means[i].resize(n + 2);
			probs[i].resize(n + 2);
		}

		for (unsigned i = 0; i < n; i++) {
			roots[i].resize(n);
		}

		//Заповнюємо початкові "листкові" значення
		for (unsigned i = 1; i <= n + 1; i++) {
			means[i][i - 1] = false_probs[i - 1];
			probs[i][i - 1] = false_probs[i - 1];
		}

		//Перебираємо всі можливі розміри піддерев
		for (unsigned k = 1; k <= n; k++) {
			//Перебираємо всі піддерева даного розміру
			for (unsigned i = 1; i <= n - k + 1; i++) {
				unsigned j = i + k - 1;
				means[i][j] = UINT_MAX;
				probs[i][j] = probs[i][j - 1] + true_probs[j - 1] + false_probs[j];

				//Перебираємо всі можливі корені даного піддерева
				for (unsigned r = i; r <= j; r++) {
					double weight = means[i][r - 1] + means[r + 1][j] + probs[i][j];
					if (weight < means[i][j]) {
						means[i][j] = weight;
						roots[i - 1][j - 1] = r - 1;
					}
				}
			}
		}

		cost = means[1][n];
		return roots;
	}

	//Рекурсивно будує оптимальне піддерево за масивом коренів
	void buildSubtree(vector<T> const &keys, vector<vector<unsigned>> const &roots, unsigned left, unsigned right) {
		if (left > right || right == UINT_MAX) {
			return;
		}
		insert(keys[roots[left][right]]);
		buildSubtree(keys, roots, left, roots[left][right] - 1);
		buildSubtree(keys, roots, roots[left][right] + 1, right);
	}

	//Будує оптимальне дерево
	void buildTree(vector<T> const &keys, vector<double> const &true_probs, vector<double> const &false_probs) {
		auto roots = optimize(true_probs, false_probs);
		buildSubtree(keys, roots, 0, roots.size() - 1);
	}

	void deleteSubtree(Node* current) {
		if (current) {
			deleteSubtree(current->left);
			deleteSubtree(current->right);
			delete current;
		}
	}

public:
	//Конструктор, що будує оптимальне дерево за заданими ключами (обов'язково відсортованими) та вірогідностями їх знаходження та не знаходження
	OptimalBST(vector<T> const &keys, vector<double> const &true_probs, vector<double> const &false_probs) : root(nullptr), size(0), cost(0) {
		if (true_probs.size() + 1 != false_probs.size()) {
			throw std::invalid_argument("Probabilities don't correspond to each other.");
		}

		double sum = 0;
		for (double p : true_probs) {
			sum += p;
		}
		for (double p : false_probs) {
			sum += p;
		}
		if (sum < 1 - 1e-8 || sum > 1 + 1e-8) {
			throw std::invalid_argument("Sum of probabilities should be equal to 1.");
		}

		buildTree(keys, true_probs, false_probs);
	}

	bool empty() const {
		return root == nullptr;
	}

	unsigned getSize() const {
		return size;
	}

	//Повертає мат. сподівання глибини пошуку
	double getCost() const {
		return cost;
	}

	bool search(T const &key) const {
		return find(key) != nullptr;
	}

	void insert(T const &key) {
		Node* current = root, *destination = nullptr;
		Status status = Status::ROOT;
		size++;

		while (current) {
			destination = current;
			if (key < current->data) {
				current = current->left;
				status = Status::LEFT;
			}
			else {
				current = current->right;
				status = Status::RIGHT;
			}
		}
		Node* new_node = new Node(key, destination, nullptr, nullptr);

		switch (status) {
		case Status::ROOT:
			root = new_node;
			break;
		case Status::LEFT:
			destination->left = new_node;
			break;
		default:
			destination->right = new_node;
		}
	}

	void remove(T const &key) {
		Node* to_remove = find(key);

		if (!to_remove) {
			return;
		}
		size--;

		if (!to_remove->right || !to_remove->left) {
			Node* child = to_remove->left ? to_remove->left : to_remove->right;

			child->parent = to_remove->parent;

			if (to_remove == root) {
				root = child;
			}
			else {
				Node* parent = to_remove->parent;

				if (to_remove == parent->left) {
					parent->left = child;
				}
				else {
					parent->right = child;
				}
			}
		}
		else {
			Node* successor = findSuccessor(to_remove);
			Node* parent = successor->parent;
			Node* child = successor->right;

			to_remove->data = successor->data;

			if (successor == parent->left) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}

			if (child) {
				child->parent = successor->parent;
			}

			delete successor;
		}
	}

	void print() const {
		if (root) {
			print(root);
		}
		else {
			cout << "The tree is empty." << endl;
		}
	}

	void deleteTree() {
		deleteSubtree(root);
	}

	~OptimalBST() {
		deleteSubtree(root);
	}
};