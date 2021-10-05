#pragma once

#include <iostream>
#include <utility>

//Структура, що описує вузол дерева
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

//Структура, що описує розширюване дерево
template <typename T>
class splay_tree {
private:
	node<T>* root;
	//Статус вершини: "є коренем", "є лівою дитиною", "є правою дитиною"
	enum class status{ ROOT, LEFT, RIGHT };

	//Прижднати синів вершини до цієї вершини
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

	//Здійснює звичайний поворот сина навколо батька (zig)
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

	//Послідовно здійснює повороти (zig, zig-zig або zig-zag), поки вказана вершина не стане коренем
	node<T>* splay(node<T>* vertex) {
		if (!vertex->parent) {
			return vertex;
		}
		auto parent = vertex->parent;
		auto grandparent = parent->parent;

		//Якщо дідуся немає, здійснюємо звичаний поворот сина навколо батька
		if (!grandparent) {
			rotate(parent, vertex);
			return vertex;
		}
		else {
			bool zigzig = (grandparent->left_child == parent) == (parent->left_child == vertex);
			//Якщо синівська вершина є лівим сином батька і батько є лівим сином дідуся
			//або ж якщо синівська вершина є правим сином батька і батько є правим сином дідуся
			//тоді спочатку обертаємо батька навколо дідуся, а потім сина навколо батька (zig-zig)
			if (zigzig) {
				rotate(grandparent, parent);
				rotate(parent, vertex);
			}
			//Інакше обертаємо спочатку сина навколо батька, а потім сина навколо дідуся (zig-zag)
			else {
				rotate(parent, vertex);
				rotate(grandparent, vertex);
			}
		}
		//Повторюємо операцію
		return splay(vertex);
	}

	//Шукає вершину з даним ключем, виносячи її в корінь 
	//Якщо такої вершини в дереві немає, в корінь виноситься вершина з найближчим до даного ключем
	node<T>* search(T const &key, node<T>* vertex) {
		//Якщо вершина порожня, повертаємо порожній покажчик
		if (!vertex) {
			return nullptr;
		}
		//Якщо ключ вершини дорівнює даному ключу, виносимо цю вершину в корінь і повертаємо її
		if (key == vertex->get_key()) {
			return splay(vertex);
		}
		//Якщо ключ вершини більший за даний ключ, переходимо до лівого піддерева, якщо воно існує
		if (key < vertex->get_key() && vertex->left_child) {
			return search(key, vertex->left_child);
		}
		//Якщо ключ вершини менший за даний ключ, переходимо до правого піддерева, якщо воно існує
		if (key > vertex->get_key() && vertex->right_child) {
			return search(key, vertex->right_child);
		}
		//Якщо не вдалося знайти вершину з даним ключем, виносимо останню розглянуту вершину в корінь
		return splay(vertex);
	}

	//Розділяє дерево на два піддерева в залежності від даного ключа, повертає покажчики на корені отриманих дерев
	std::pair<node<T>*, node<T>*> split(T const &key) {
		//Якщо корінь порожній, повертаємо пару порожніх вершин
		if (!root) {
			return std::pair<node<T>*, node<T>*>{ nullptr, nullptr };
		}
		//Виконуємо пошук даного ключа
		root = search(key, root);
		node<T>* left = root->left_child;
		node<T>* right = root->right_child;

		//Якщо в корінь потрапила вершина з ключем рівним даному, то видаляємо її, повертаємо два її піддерева
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

		//Якщо в корінь потрапила вершина з ключем меншим за даний, то відділяємо праве піддерево
		//Повертаємо покажчик на корінь та праве піддерево
		if (key > root->get_key()) {
			root->right_child = nullptr;
			if (right) {
				right->parent = nullptr;
			}
			return std::pair<node<T>*, node<T>*>{ root, right };
		}
		//Якщо в корінь потрапила вершина з ключем більшим за даний, то відділяємо ліве піддерево
		//Повертаємо покажчик на корінь та ліве піддерево
		else {
			root->left_child = nullptr;
			if (left) {
				left->parent = nullptr;
			}
			return std::pair<node<T>*, node<T>*>{ left, root };
		}
	}

	//Вставляє в дерево вершину з даним ключем, виносячи її в корінь
	void insert(T const &key) {
		//Розщеплюємо дане дерево на два дерева
		auto sons = split(key);
		//З'єднуємо їх в одне дерево з коренем у новій вершині
		root = new node<T>(key, sons.first, sons.second, nullptr);
	}

	//Зливає два дерева, не порушуючи властивостей дерева пошуку
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

	//Видаляє дерево
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

	//Друкує дерево (preorder)
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

	//Видаляє вершину з даним ключем, не порушуючи властивостей дерева пошуку
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