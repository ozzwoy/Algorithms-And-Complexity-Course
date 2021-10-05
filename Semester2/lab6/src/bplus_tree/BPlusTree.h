#pragma once

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

template <typename T>
class BPlusTree {
private:
	struct Node {
		vector<T> keys;
		Node* parent = nullptr;
		vector<Node*> children;
		Node* left = nullptr, *right = nullptr;
		bool leaf = true;

		//Перевіряє, чи даний ключ міститься у вузлі
		bool contains(T const &key) const {
			for (T const &cur : keys) {
				if (key == cur) {
					return true;
				}
			}
			return false;
		}

		//Вставляє ключ (якщо його ще немає), не порушуючи впорядкованості
		//Повертає індекс ключа або UINT_MAX, якщо ключ уже міститься у вузлі
		unsigned insert(T const &key) {
			if (contains(key)) {
				return UINT_MAX;
			}

			unsigned i = 0;
			while (i < keys.size() && key > keys[i]) {
				i++;
			}

			if (i < keys.size()) {
				keys.insert(keys.begin() + i, key);
			}
			else {
				keys.push_back(key);
			}
			return i;
		}

		//Видаляє ключ
		bool remove(T const &key) {
			if (!contains(key)) {
				return false;
			}

			unsigned i = 0;
			while (i < keys.size() && key > keys[i]) {
				i++;
			}

			keys.erase(keys.begin() + i);
			if (!leaf) {
				children.erase(children.begin() + i);
			}
			return true;
		}
	};

private:
	Node* root;
	const unsigned MIN_DEGREE;

	//Шукає листок, у якому міститься даний ключ (або міг би міститися)
	Node* searchLeaf(T const &key) const {
		Node* current = root;
		while (!current->leaf) {
			for (unsigned i = 0; i < current->children.size(); i++) {
				if (i == current->children.size() - 1 || key < current->keys[i]) {
					current = current->children[i];
					break;
				}
			}
		}
		return current;
	}

	//Розбиває переповнений вузол на два нових
	void split(Node* node) {
		Node* new_node = new Node();

		//Новий вузол буде правим братом, вставляємо його в список листків, обмінюючи покажчики
		new_node->right = node->right;
		if (node->right) {
			node->right->left = new_node;
		}
		node->right = new_node;
		new_node->left = node;

		//Запам'ятовуємо середній ключ
		T mid_key = node->keys[MIN_DEGREE];

		//Якщо даний вузол не є листком, то розподіляємо ключі (без середнього ключа, у лівого брата дітей має бути на одного більше) та дітей
		if (!node->leaf) {
			std::copy(node->keys.begin() + MIN_DEGREE + 1, node->keys.end(), std::back_inserter(new_node->keys));
			node->keys.erase(node->keys.begin() + MIN_DEGREE, node->keys.end());

			std::copy(node->children.begin() + MIN_DEGREE + 1, node->children.end(), std::back_inserter(new_node->children));
			node->children.erase(node->children.begin() + MIN_DEGREE + 1, node->children.end());

			new_node->leaf = false;
		}
		//Інакше порівну розподіляємо ключі
		else {
			std::copy(node->keys.begin() + MIN_DEGREE, node->keys.end(), std::back_inserter(new_node->keys));
			node->keys.erase(node->keys.begin() + MIN_DEGREE, node->keys.end());
		}

		//Якщо дана вершина корінь, то створємо новий корінь, додаємо в нього середній ключ, під'єднуємо до нього старий та новий вузол
		if (root == node) {
			root = new Node;
			root->leaf = false;
			root->keys.push_back(mid_key);
			root->children.push_back(node);
			root->children.push_back(new_node);
			node->parent = new_node->parent = root;
		}
		//Інакше додаємо до батька середній ключ, під'єднуємо новий вузол та перевіряємо на переповненість
		else {
			Node* parent = node->parent;
			new_node->parent = parent;
			unsigned i = parent->insert(mid_key);
			parent->children.insert(parent->children.begin() + i + 1, new_node);
			if (parent->keys.size() == MIN_DEGREE * 2) {
				split(parent);
			}
		}
	}

	//Оновлює вершини на шляху до кореня, замінюючи входження видаленого ключа
	void update(Node* current, T const &key, T const &replacement) {
		if (current) {
			for (T &cur : current->keys) {
				if (cur == key) {
					cur = replacement;
					break;
				}
			}
			update(current->parent, key, replacement);
		}
	}

	//Видаляє ключ з вершини, налагоджуючи властивості дерева
	void deleteInNode(Node* current, T const &key) {
		if (!current->remove(key)) {
			return;
		}

		if (current->leaf) {
			//Вершина недостатньо заповнена
			if (current->keys.size() < MIN_DEGREE - 1) {
				Node* left = current->left, *right = current->right;

				//Якщо можна позичити ключ у лівого брата, позичаємо максимального
				if (left && left->keys.size() > MIN_DEGREE - 1) {
					T max_key = left->keys.back();
					left->keys.pop_back();
					current->keys.insert(current->keys.begin(), max_key);
					/*Node* child = left->children.back();
					left->children.pop_back();
					current->children.insert(current->children.begin(), child);*/
					update(left->parent, max_key, left->keys.back());
				}
				//Якщо можна позичити ключ у правого брата, позичаємо мінімального
				else if (right && right->keys.size() > MIN_DEGREE - 1) {
					T min_key = right->keys.front();
					right->keys.erase(right->keys.begin());
					current->keys.push_back(min_key);
					/*Node* child = right->children.front();
					right->children.erase(right->children.begin());
					current->children.push_back(child);*/
					update(right->parent, min_key, right->keys.front());
				}
				//Інакше зливаємо вершини
				else {
					if (left) {
						T max = left->keys.back();

						for (T cur : current->keys) {
							left->insert(cur);
						}

						//Вилучаємо вузол зі списку
						left->right = current->right;
						if (current->right) {
							current->right->left = left;
						}
						
						//Видаляємо зайвий ключ
						deleteInNode(left->parent, max);
						delete current;
					}
					else {
						T min = right->keys.front();

						for (T cur : current->keys) {
							right->insert(cur);
						}

						//Вилучаємо вузол зі списку
						right->left = current->left;
						if (current->left) {
							current->left->right = right;
						}
						
						//Видаляємо зайвий ключ
						deleteInNode(right->parent, min);
						delete current;
					}
				}

				if (root->children.size() == 1) {
					root = root->children[0];
				}

				return;
			}
		}
		
		if (key < current->keys.front()) {
			update(current->parent, key, current->keys.front());
		}
		else {
			update(current->parent, key, current->keys.back());
		}
	}

	void print(Node* current, unsigned level = 0) const {
		if (current) {
			for (unsigned i = 0; i < level; i++) {
				cout << "   ";
			}
			cout << level << ": ";
			for (T cur : current->keys) {
				cout << cur << "  ";
			}
			cout << endl;

			for (Node* cur : current->children) {
				print(cur, level + 1);
			}
		}
	}

public:
	BPlusTree(unsigned min_power) : MIN_DEGREE(min_power), root(nullptr) {}

	bool insert(T const &key) {
		if (!root) {
			root = new Node;
			root->keys.push_back(key);
		}
		else {
			Node* leaf = searchLeaf(key);
			if (leaf->insert(key) == UINT_MAX) {
				return false;
			}
			if (leaf->keys.size() == MIN_DEGREE * 2) {
				split(leaf);
			}
		}
		return true;
	}

	bool remove(T const &key) {
		Node* leaf = searchLeaf(key);
		if (!leaf->contains(key)) {
			return false;
		}
		deleteInNode(leaf, key);
		return true;
	}

	void print() const {
		if (root) {
			print(root);
		}
		else {
			cout << "The tree is empty." << endl;
		}
	}
};