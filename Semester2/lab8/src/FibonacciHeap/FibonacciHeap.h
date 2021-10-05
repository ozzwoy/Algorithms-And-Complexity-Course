#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

//����, �� ����� ������ Գ�������
template<typename T>
class FibonacciHeap {
private:
	//���������, �� ����� ����� ������
	struct Node {
		T key;
		//ʳ������ ����
		unsigned degree;
		//������ �������� ������
		bool mark;
		Node* left, *right, *parent, *child;


		Node(T const &_key) : key(_key), degree(0), mark(false), left(this), right(this), parent(nullptr), child(nullptr) {}

		//���� ����� ����
		void addBrother(Node* brother) {
			if (brother) {
				brother->right = this;
				brother->left = this->left;
				this->left->right = brother;
				this->left = brother;
			}
		}

		//�������� ������ ����� ������
		void addSiblings(Node* start, Node* end) {
			if (start && end) {
				start->left = this;
				end->right = this->right;
				this->right->left = end;
				this->right = start;
			}
		}

		//ϳ�'����� ���� ������, ����� �� ������
		void link(Node* new_child) {
			if (!new_child) return;
			if (child) {
				child->addBrother(new_child);
			}
			else {
				child = new_child;
				new_child->left = new_child->right = new_child;
			}
			degree++;
			new_child->mark = false;
			new_child->parent = this;
		}

		//������ ����� ����� � ������, ����� ������
		void pop() {
			left->right = right;
			right->left = left;
			if (parent) {
				if (parent->child == this) {
					parent->child = parent->degree == 1 ? nullptr : right;
				}
				parent->degree--;
			}
			parent = nullptr;
			mark = false;
		}
	};

private:
	Node* root;
	unsigned n, max_degree;

	//����� �������� ������ ���������� �������
	void consolidate() {
		if (!root) return;
		//�����, �� ������ ��� ���������� ������, �� ������� ��������� �� ��������
		vector<Node*> degree_cache(max_degree + 1, nullptr);
		Node* current = root, *x, *y, *next;
		unsigned degree;

		do {
			x = current;
			next = current->right;
			degree = x->degree;

			//������ ������� ������ ���� � �������, �� � �, - ������� �� � �
			while (degree_cache[degree]) {
				y = degree_cache[degree];
				if (x == y) break;

				//�������, ��� ������ ��������, ��� �� �������� �������������� ������
				if (x->key > y->key) {
					std::swap(x, y);
				}
				//���� ������, ��� ���������� � ������ ������, � ������� ������, �� ��������� ����� ������� ���� �����
				if (y == root) {
					root = root->right;
				}
				//���� ������, ��� ���������� � ������ ������, � ��������� ��� �������� ������, �� ��������� �������� ���������� ����� ������
				if (y == next) {
					next = next->right;
				}
				y->pop();
				x->link(y);

				degree_cache[degree] = nullptr;
				//ϳ��� ������ ������ � ������
				degree++;
				if (degree > max_degree) {
					degree_cache.push_back(nullptr);
					max_degree++;
				}
			}

			degree_cache[degree] = x;
			current = next;
		} while (current != root);

		//������ ����� ����� ������ (�����)
		for (Node* cur : degree_cache) {
			if (cur) {
				if (root) {
					if (cur->key < root->key) {
						root = cur;
					}
				}
				else {
					root = cur;
				}
			}
		}
	}

	//�������� ������ ������ ������� �� ���� �� �� ������ ������
	void cascadingCut(Node* current) {
		if (current && current->parent) {
			Node* parent = current->parent;
			//���� ������� ��� ������� (����� � �� ��� ��������� ���� �� ����), �� �������� ��, ������ �� ������ ������, �������� ������� ������
			if (current->mark) {
				current->pop();
				root->addBrother(current);
				cascadingCut(parent);
			}
			//������ ������� ��
			else {
				current->mark = true;
			}
		}
	}

	Node* searchRecursive(T const &key, Node* current) const {
		if (current->key == key) return current;

		Node* found = nullptr;
		if (current->key <= key) {
			if (current->child) {
				Node* iter = current->child;
				for (unsigned i = 0; i < current->degree && !found; i++) {
					found = searchRecursive(key, iter);
					iter = iter->right;
				}
			}
		}
		
		return found;
	}

	//���� ������� � ����� ������
	Node* search(T const &key) const {
		if (root) {
			Node* current = root, *found;
			do {
				found = searchRecursive(key, current);
				if (found) return found;
				current = current->right;
			} while (current != root);
		}
		
		return nullptr;
	}

	//������ ����
	void decreaseKey(Node* aim, T const &new_key) {
		if (!aim || aim->key < new_key) return;

		aim->key = new_key;
		Node* parent = aim->parent;
		//���� ������ ������ ������ ����, �������� ���� �������, ���������� � ������ ������, �������� ������� ������
		if (parent && new_key < parent->key) {
			aim->pop();
			root->addBrother(aim);
			cascadingCut(parent);
		}

		//���� ����� ���� ������ �� ���� ������, �� ����� ������� ��� ����� �����
		if (aim->key < root->key) {
			root = aim;
		}
	}

	void printRecursive(Node* current, unsigned level = 0, unsigned order = 0) const {
		if (current) {
			for (unsigned i = 0; i < level; i++) cout << "   ";
			cout << level << ": " << current->key << '(';
			//"m" ������, �� ���� ������� �������
			cout << current->mark ? "m" : "0";
			cout << ')' << endl;
			
			if (current->child) {
				Node* iter = current->child;
				for (unsigned i = 0; i < current->degree; i++) {
					printRecursive(iter, level + 1);
					iter = iter->right;
				}
			}
		}
	}

	void removeRecursive(Node* current) {
		if (current) {
			if (current->child) {
				Node* iter = current->child;
				for (unsigned i = 0; i < current->degree; i++) {
					removeRecursive(iter);
					iter = iter->right;
				}
			}

			delete current;
		}
	}

public:
	FibonacciHeap() : root(nullptr), n(0), max_degree(0) {}

	bool empty() const {
		return root == nullptr;
	}

	unsigned size() const {
		return n;
	}

	//��������, �� � ����� ���� � �����
	bool isIn(T const &key) const {
		return search(key) != nullptr;
	}

	void insert(T const &key) {
		Node* new_node = new Node(key);

		if (root) {
			root->addBrother(new_node);
			if (key < root->key) {
				root = new_node;
			}
		}
		else {
			root = new_node;
		}

		n++;
	}

	T getMin() const {
		if (!root) {
			throw std::invalid_argument("The heap is empty.");
		}
		return root->key;
	}

	//����� ������� ������ � �����, �������� ��
	void unite(FibonacciHeap<T> &to_unite) {
		if (to_unite.size()) {
			if (to_unite.size() == 1) {
				root->addBrother(to_unite.root);
			}
			else {
				root->addSiblings(to_unite.root, to_unite.root->left);
			}

			if (to_unite.root->key < root->key) {
				root = to_unite.root;
			}
			if (to_unite.max_degree > max_degree) {
				max_degree = to_unite.max_degree;
			}

			n += to_unite.n;
			to_unite.root = nullptr;
			to_unite.n = 0;
		}
	}

	T extractMin() {
		if (!root) {
			throw std::invalid_argument("The heap is empty.");
		}

		if (root->child) {
			root->addSiblings(root->child, root->child->left);
			Node* child = root->child, *current = child;
			do {
				current->parent = nullptr;
				current = current->right;
			} while (current != child);
		}

		Node* min = root;
		min->pop();
		root = n == 1 ? nullptr : min->right;
		consolidate();
		n--;

		T data = min->key;
		delete min;
		return data;
	}

	void decreaseKey(T const &old_one, T const &new_one) {
		if (old_one < new_one) {
			throw std::invalid_argument("The new key is greater than old one.");
		}

		Node* found = search(old_one);
		if (!found) {
			throw std::invalid_argument("The key is absent in the heap.");
		}
		decreaseKey(found, new_one);
	}

	void remove(T const &key) {
		Node* found = search(key);
		if (!found) {
			throw std::invalid_argument("The key is absent in the heap.");
		}
		decreaseKey(found, getMin());
		extractMin();
	}

	//����� ������
	void print() const {
		if (root) {
			Node* current = root;
			do {
				printRecursive(current);
				current = current->right;
			} while (current != root);
		}
		else {
			cout << "The heap is empty." << endl;
		}
	}

	~FibonacciHeap() {
		if (root) {
			Node* current = root, *next;
			root->left->right = nullptr;
			do {
				next = current->right;
				removeRecursive(current);
				current = next;
			} while (current);
		}
	}
};
