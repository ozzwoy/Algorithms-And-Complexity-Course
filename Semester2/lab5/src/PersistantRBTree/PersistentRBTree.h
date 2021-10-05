#pragma once

#include "RBTree.h"
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

//����, �� �������� �������� �������-����� ������, ������� ���� ������������� ��������
template <typename T>
class PersistentRBTree : public RBTree<T> {
private:
	//����� ������ ��� ����� ������
	vector<RBTree<T>::Node*> roots;

	//����� �������, ����� ��������� � ����
	RBTree<T>::Node* copy(RBTree<T>::Node const &source) {
		auto new_node = new typename RBTree<T>::Node(source);
		if (new_node->left) {
			new_node->left->parent = new_node;
		}
		if (new_node->right) {
			new_node->right->parent = new_node;
		}
		return new_node;
	}

	//����� ������� �� ������� �������
	RBTree<T>::Node* deepCopy(RBTree<T>::Node const &source, unsigned depth) {
		auto new_node = new typename RBTree<T>::Node(source);

		if (new_node->left) {
			new_node->left->parent = new_node;

			if (depth > 0) {
				new_node->left = deepCopy(*new_node->left, depth - 1);
			}
		}
		if (new_node->right) {
			new_node->right->parent = new_node;

			if (depth > 0) {
				new_node->right = deepCopy(*new_node->right, depth - 1);
			}
		}

		return new_node;
	}

	//���� ������� ����, ������� �� ������� �� �����
	RBTree<T>::Node* searchCopying(T const &key) {
		typename RBTree<T>::Node* current = copy(*RBTree<T>::root), *parent;

		RBTree<T>::root = current;
		//������ ���������, ���� ��������� ����� ����� �� ������ � �����
		roots.push_back(current);

		if (current->key == key) return current;

		while (current) {
			parent = current->parent;

			if (parent) {
				if (current == parent->left) {
					//������� ������� �������
					current = parent->left = copy(*current);

					if (parent->right) {
						//������ ������� ���� �����
						parent->right = deepCopy(*parent->right, 2);
					}
				}
				else {
					current = parent->right = copy(*current);

					if (parent->left) {
						parent->left = deepCopy(*parent->left, 2);
					}
				}
			}

			if (key == current->key) {
				break;
			}
			else if (key < current->key) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		return current;
	}

public:
	PersistentRBTree() {
		roots.push_back(nullptr);
	}

	//����������� �������� ������� (����� ������ �� ������ �� ���� �������, � ����� ������ �� ����� ��ﳿ ������� ������)
	void insert(T const &key) override {
		typename RBTree<T>::Node* current = RBTree<T>::root, *temp = nullptr, *parent;

		while (current) {
			parent = current->parent;

			if (parent) {
				bool left = current == parent->left;

				//������� ���� ������
				if (parent->right) {
					parent->right = copy(*parent->right);
				}
				if (parent->left) {
					parent->left = copy(*parent->left);
				}

				temp = left ? parent->left : parent->right;
			}
			//���� ������� ������� �������� - ������� ��, �������� ����� ������ ������ �� ������
			else {
				auto new_root = copy(*current);
				RBTree<T>::root = new_root;
				roots.push_back(new_root);
				temp = new_root;
			}

			if (key < current->key) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		auto new_node = new typename RBTree<T>::Node(key, RBTree<T>::Color::RED, temp, nullptr, nullptr);
		if (temp) {
			if (key < temp->key) {
				temp->left = new_node;
			}
			else {
				temp->right = new_node;
			}
			RBTree<T>::insertFixup(new_node);
			//������� ����� ������� - �������� ���� � �����
			roots[roots.size() - 1] = RBTree<T>::root;
		}
		else {
			new_node->color = RBTree<T>::Color::BLACK;
			RBTree<T>::root = new_node;
			roots.push_back(new_node);
		}
	}

	//����������� ���������� ��������� (����� ������ �� ������ �� �������, �� ���� �� ���������, � ����� ������ �� ����� ������ ��ﳿ ������� ������)
	void remove(T const &key) override {
		typename RBTree<T>::Node* current = RBTree<T>::search(key);
		if (!current) {
			return;
		}
		else {
			//���� ���� �������� � ����� - ������ ���� �������, ������ ����� ����� � �����
			current = searchCopying(key);
		}

		typename RBTree<T>::Node* replacement;
		if (!current->left || !current->right) {
			if (!current->left && !current->right) replacement = nullptr;
			else {
				replacement = current->left ? current->left = copy(*current->left) : current->right = copy(*current->right);
			}
			
			if (replacement) {
				replacement->parent = current->parent;
			}

			if (current == RBTree<T>::root) {
				RBTree<T>::root = replacement;
				roots[roots.size() - 1] = replacement;
			}
			else if (current == current->parent->left) {
				current->parent->left = replacement;
			}
			else {
				current->parent->right = replacement;
			}

			if (current->color == RBTree<T>::Color::BLACK) {
				RBTree<T>::removeFixup(replacement);
				//������� ����� ������� - �������� ���� � �����
				roots[roots.size() - 1] = RBTree<T>::root;
			}

			delete current;
		}
		else {
			replacement = current->right = copy(*current->right);
			current->left = deepCopy(*current->left, 2);

			while (replacement->left) {
				if (replacement->right) {
					//������� ��� ������ ���� �� ����� �� �����-��������
					replacement->right = deepCopy(*replacement->right, 2);
				}
				replacement = replacement->left = copy(*replacement->left);
			}

			if (replacement->right) {
				replacement->right = copy(*replacement->right);
			}

			if (replacement == current->right) {
				RBTree<T>::link(current, current->left, replacement->right);
			}
			else {
				RBTree<T>::link(replacement->parent, replacement->right, replacement->parent->right);
			}
			current->key = replacement->key;

			typename RBTree<T>::Node* temp = replacement;
			replacement = replacement->right;
			if (temp->color == RBTree<T>::Color::BLACK) {
				RBTree<T>::removeFixup(replacement);
				//������� ����� ������� - �������� ���� � �����
				roots[roots.size() - 1] = RBTree<T>::root;
			}

			delete temp;
		}
	}

	//����� ����� ������ ������� ������� ����� �����
	void printPrevious(size_t steps_back) {
		if (steps_back >= roots.size()) {
			cout << "  Impossible, the tree doesn't exist." << endl;
		}
		else if (!roots[steps_back]) {
			cout << "  The tree is empty." << endl;
		}
		else {
			RBTree<T>::print(roots[roots.size() - steps_back - 1]);
		}
	}
};
