#pragma once

#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;

//����, �� ����� ��������� ������
template <typename T>
class BinomialHeap {
private:
	//���������, �� ����� ����� ������
	struct Node {
		T key;
		unsigned degree;
		Node* parent, *next, *child;

		Node() : degree(0), parent(nullptr), next(nullptr), child(nullptr) {}

		Node(T const &_key) : Node() {
			key = _key;
		}

		//ϳ�'���� ���� �� ������ ����� ������ ����
		void link(Node* new_child) {
			new_child->parent = this;
			new_child->next = this->child;
			this->degree++;
			this->child = new_child;
		}
	};

private:
	Node* root;

	//������� ����� � ��������� ������
	Node* findMin() const {
		if (!root) return nullptr;

		//̳�������� ���� ������ ����������� � ������ ������
		T min = root->key;
		Node* min_node = root, *current = root->next;
		while (current) {
			if (current->key < min) {
				min = current->key;
				min_node = current;
			}
			current = current->next;
		}

		return min_node;
	}

	//��'���� ����� ���� ������ � �������� �������, ������������� �� � ������� ���������
	//������� ������� ������ ���������, � �������� ���������� ������ ���� 
	void merge(BinomialHeap<T> &that) {
		Node* this_ptr = this->root, *that_ptr = that.root;
		BinomialHeap<T> result;

		if (this_ptr && that_ptr) {
			Node* result_ptr;

			if (this_ptr->degree < that_ptr->degree) {
				result_ptr = this_ptr;
				this_ptr = this_ptr->next;
			}
			else {
				result_ptr = that_ptr;
				that_ptr = that_ptr->next;
			}

			//� ����� ������ ������ ���� ������ �������������� ����� �� ������ ��������
			this->root = result_ptr;

			//��������� ��'������ �� ������ ������ ������ ����� � ���� ������, �������� ������ ����� � ����, �� �����������
			//�����������, ���� ���� � ������ ����������
			while (this_ptr && that_ptr) {
				if (this_ptr->degree < that_ptr->degree) {
					result_ptr->next = this_ptr;
					this_ptr = this_ptr->next;
				}
				else {
					result_ptr->next = that_ptr;
					that_ptr = that_ptr->next;
				}
				result_ptr = result_ptr->next;
			}

			//ϳ�'������ ����� ������, �� ���������

			if (this_ptr) {
				result_ptr->next = this_ptr;
			}

			if (that_ptr) {
				result_ptr->next = that_ptr;
			}
		}
		else {
			//���� ������� ������ ������� - ���������� �������� �� ������ ������ ���� ������
			//������ ���� ������ ��� � �����������
			if (that_ptr) {
				this->root = that_ptr;
			}
		}

		that.root = nullptr;
	}

	//������ ������ ������
	Node* inverse(Node* start) {
		if (!start) return nullptr;

		Node* new_start, *end, *current;

		current = start;
		while (current->next) {
			current = current->next;
		}
		new_start = end = current;

		while (end != start) {
			current = start;
			while (current->next != end) {
				current = current->next;
			}
			end->next = current;
			end = current;
		}

		start->next = nullptr;
		return new_start;
	}

	//����� ���� ������� �������, ���� ����� ���� �����
	void decreaseKey(Node* node, T const &new_key) {
		if (!node || new_key > node->key) {
			throw std::invalid_argument("invalid operation, the key is greater or node is nullptr.");
		}

		node->key = new_key;
		Node* current = node, *parent = node->parent;
		//������ ����� �� ����� �����, ���� ����������� ���� �� ��������� ������� ��� ������ �� ���� ������
		while (parent && current->key < parent->key) {
			std::swap(current->key, parent->key);
			current = parent;
			parent = current->parent;
		}
	}

	//���� ������� �� ������
	Node* search(T const &key, Node* current) const {
		if (!current) return nullptr;
		else {
			if (current->key == key) return current;

			Node* found = nullptr;
			//�������� ������ ����� ����
			if (current->key < key) {
				found = search(key, current->child);
			}
			if (!found) {
				//���� �� �������� - ������ ����� �����
				return search(key, current->next);
			}
			else return found;
		}
	}

	void remove(Node* to_remove) {
		if (!to_remove) return;
		//���������� ������, �� �� ����������, �������� ��������
		decreaseKey(to_remove, this->getMin());
		//��������� � ������ �������� ��������
		extractMin();
	}

	void print(Node* current, unsigned level = 0) const {
		if (!current) return;

		for (unsigned i = 0; i < level; i++) {
			cout << "   ";
		}
		cout << level << ": " << current->key << endl;

		print(current->child, level + 1);
		print(current->next, level);
	}

public:
	BinomialHeap() : root(nullptr) {}

	BinomialHeap(T const &key) {
		root = new Node(key);
	}

	bool empty() const {
		return root == nullptr;
	}

	bool isIn(T const &key) const {
		return search(key, root) != nullptr;
	}
 
	//������� ��������� ����
	T getMin() const {
		if (!root) {
			throw std::invalid_argument("invalid operation, the heap is empty.");
		}

		return findMin()->key;
	}

	//����� ������� ������ � �����, ������� ������ ���������
	void unite(BinomialHeap<T> &to_unite) {
		if (to_unite.empty()) return;

		//������������ ���� ������ � ������������� ������ ������
		this->merge(to_unite);
		if (!root) return;

		Node* prev = nullptr, *current = root, *next = root->next;
		while (next) {
			//���� ����� ����� � ��������� ����������� ��������� ��� ���� ����� ����� ��� ����� ���������� �������, �������� ������ �� �������
			if (current->degree != next->degree || (next->next && next->next->degree == current->degree)) {
				prev = current;
				current = next;
			}
			//������ �������� ������� ������ ��� ���������� ������ �� ������� (��'������ ���� �� ������)
			else {
				if (current->key < next->key) {
					current->next = next->next;
					current->link(next);
				}
				else {
					if (prev) {
						prev->next = current->next;
					}
					else {
						root = next;
					}
					next->link(current);
					current = next;
				}
			}

			next = current->next;
		}
	}

	void insert(T const &key) {
		BinomialHeap<T> temp(key);
		unite(temp);
	}

	//������� ��������� ���� � ������ � ������� ����
	T extractMin() {
		if (!root) {
			throw std::invalid_argument("invalid operation, the heap is empty.");
		}

		Node* min_node = findMin(), *prev = root;
		if (min_node == root) {
			root = min_node->next;
		}
		else {
			while (prev->next != min_node) {
				prev = prev->next;
			}
			prev->next = min_node->next;
		}

		//��������� ���� ������, ������ ������ ��� � ��������� ������� ���� �������� �������
		BinomialHeap<T> temp;
		temp.root = inverse(min_node->child);
		
		//��������� ������ � ����� ������� ������ ������ ������
		Node* current = temp.root;
		while (current) {
			current->parent = nullptr;
			current = current->next;
		}

		this->unite(temp);

		T key = min_node->key;
		delete min_node;
		return key;
	}

	void remove(T const &key) {
		Node* aim = search(key, root);
		remove(aim);
	}

	void print() const {
		if (!root) {
			cout << "The heap is empty." << endl;
		}

		print(root);
	}
};