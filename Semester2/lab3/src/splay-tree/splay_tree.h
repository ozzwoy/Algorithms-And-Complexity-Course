#pragma once

#include <iostream>
#include <utility>

//���������, �� ����� ����� ������
template <typename T>
struct node {
private:
	T key;

public:
	node* left_child;
	node* right_child;
	node* parent;

	node (T const &key) : key(key), left_child(nullptr), right_child(nullptr), parent(nullptr) {}

	node(T const &key, node* left_child, node* right_child, node* parent) : key(key), left_child(left_child), right_child(right_child),
		parent(parent) {
		if (left_child) {
			left_child->parent = this;
		}
		if (right_child) {
			right_child->parent = this;
		}
	}

	T get_key() const {
		return key;
	}
};

//���������, �� ����� ����������� ������
template <typename T>
class splay_tree {
private:
	node<T>* root;
	//������ �������: "� �������", "� ���� �������", "� ������ �������"
	enum class status{ ROOT, LEFT, RIGHT };

	//��������� ���� ������� �� ���� �������
	void link_sons(node<T>* parent) {
		if (parent) {
			if (parent->left_child) {
				parent->left_child->parent = parent;
			}
			if (parent->right_child) {
				parent->right_child->parent = parent;
			}
		}
	}

	//������� ��������� ������� ���� ������� ������ (zig)
	void rotate(node<T>* parent, node<T>* child) {
		node<T>* grandparent = parent->parent;
		if (grandparent) {
			if (parent == grandparent->left_child) {
				grandparent->left_child = child;
			}
			else {
				grandparent->right_child = child;
			}
		}

		if (child == parent->left_child) {
			parent->left_child = child->right_child;
			child->right_child = parent;
		}
		else {
			parent->right_child = child->left_child;
			child->left_child = parent;
		}

		child->parent = grandparent;
		link_sons(child);
		link_sons(parent);
	}

	//��������� ������� �������� (zig, zig-zig ��� zig-zag), ���� ������� ������� �� ����� �������
	node<T>* splay(node<T>* vertex) {
		if (!vertex->parent) {
			return vertex;
		}
		auto parent = vertex->parent;
		auto grandparent = parent->parent;

		//���� ����� ����, ��������� �������� ������� ���� ������� ������
		if (!grandparent) {
			rotate(parent, vertex);
			return vertex;
		}
		else {
			bool zigzig = (grandparent->left_child == parent) == (parent->left_child == vertex);
			//���� �������� ������� � ���� ����� ������ � ������ � ���� ����� �����
			//��� � ���� �������� ������� � ������ ����� ������ � ������ � ������ ����� �����
			//��� �������� �������� ������ ������� �����, � ���� ���� ������� ������ (zig-zig)
			if (zigzig) {
				rotate(grandparent, parent);
				rotate(parent, vertex);
			}
			//������ �������� �������� ���� ������� ������, � ���� ���� ������� ����� (zig-zag)
			else {
				rotate(parent, vertex);
				rotate(grandparent, vertex);
			}
		}
		//���������� ��������
		return splay(vertex);
	}

	//���� ������� � ����� ������, �������� �� � ����� 
	//���� ���� ������� � ����� ����, � ����� ���������� ������� � ���������� �� ������ ������
	node<T>* search(T const &key, node<T>* vertex) {
		//���� ������� �������, ��������� ������� ��������
		if (!vertex) {
			return nullptr;
		}
		//���� ���� ������� ������� ������ �����, �������� �� ������� � ����� � ��������� ��
		if (key == vertex->get_key()) {
			return splay(vertex);
		}
		//���� ���� ������� ������ �� ����� ����, ���������� �� ����� ��������, ���� ���� ����
		if (key < vertex->get_key() && vertex->left_child) {
			return search(key, vertex->left_child);
		}
		//���� ���� ������� ������ �� ����� ����, ���������� �� ������� ��������, ���� ���� ����
		if (key > vertex->get_key() && vertex->right_child) {
			return search(key, vertex->right_child);
		}
		//���� �� ������� ������ ������� � ����� ������, �������� ������� ���������� ������� � �����
		return splay(vertex);
	}

	//������� ������ �� ��� �������� � ��������� �� ������ �����, ������� ��������� �� ����� ��������� �����
	std::pair<node<T>*, node<T>*> split(T const &key) {
		//���� ����� �������, ��������� ���� ������� ������
		if (!root) {
			return std::pair<node<T>*, node<T>*>{ nullptr, nullptr };
		}
		//�������� ����� ������ �����
		root = search(key, root);
		node<T>* left = root->left_child;
		node<T>* right = root->right_child;

		//���� � ����� ��������� ������� � ������ ����� ������, �� ��������� ��, ��������� ��� �� ��������
		if (key == root->get_key()) {
			if (left) {
				left->parent = nullptr;
			}
			if (right) {
				right->parent = nullptr;
			}
			delete root;
			root = nullptr;
			return std::pair<node<T>*, node<T>*>{ left, right };
		}

		//���� � ����� ��������� ������� � ������ ������ �� �����, �� �������� ����� ��������
		//��������� �������� �� ����� �� ����� ��������
		if (key > root->get_key()) {
			root->right_child = nullptr;
			if (right) {
				right->parent = nullptr;
			}
			return std::pair<node<T>*, node<T>*>{ root, right };
		}
		//���� � ����� ��������� ������� � ������ ������ �� �����, �� �������� ��� ��������
		//��������� �������� �� ����� �� ��� ��������
		else {
			root->left_child = nullptr;
			if (left) {
				left->parent = nullptr;
			}
			return std::pair<node<T>*, node<T>*>{ left, root };
		}
	}

	//�������� � ������ ������� � ����� ������, �������� �� � �����
	void insert(T const &key) {
		//����������� ���� ������ �� ��� ������
		auto sons = split(key);
		//�'������ �� � ���� ������ � ������� � ���� ������
		root = new node<T>(key, sons.first, sons.second, nullptr);
	}

	//����� ��� ������, �� ��������� ������������ ������ ������
	node<T>* merge(node<T>* left, node<T>* right) {
		if (!left) {
			return right;
		}
		if (!right) {
			return left;
		}
		right = search(left->get_key(), right);
		right->left_child = left;
		left->parent = right;
		return right;
	}

	//������� ������
	void destroy(node<T>* vertex) {
		if (vertex) {
			if (vertex->left_child) {
				destroy(vertex->left_child);
			}
			if (vertex->right_child) {
				destroy(vertex->right_child);
			}
			delete vertex;
		}
	}

	//����� ������ (preorder)
	void print(node<T>* vertex, unsigned level = 0, status stat = status::ROOT) const {
		if (!vertex) {
			return;
		}
		for (unsigned i = 0; i < level; i++) {
			std::cout << "  ";
		}
		std::cout << level;

		switch (stat) {
		case status::ROOT:
			std::cout << "(o)";
			break;
		case status::LEFT:
			std::cout << "(l)";
			break;
		default:
			std::cout << "(r)";
		}

		std::cout << ": " << vertex->get_key() << std::endl;
		print(vertex->left_child, level + 1, status::LEFT);
		print(vertex->right_child, level + 1, status::RIGHT);
	}

public:
	splay_tree() : root(nullptr) {}

	void add(T const &key) {
		insert(key);
	}

	//������� ������� � ����� ������, �� ��������� ������������ ������ ������
	void remove(T const &key) {
		root = find(key, root);
		node<T>* left = root->left_child;
		node<T>* right = root->right_child;
		left->parent = right->parent = nullptr;
		delete root;
		root = merge(left, right);
	}

	bool search(T const &key) {
		return key == (root = search(key, root))->get_key();
	}

	void print() const {
		if (!root) {
			std::cout << "The tree is empty." << std::endl;
		}
		print(root);
	}

	void destroy() {
		destroy(root);
		root = nullptr;
	}

	~splay_tree() {
		if (root) {
			destroy(root);
		}
	}
};