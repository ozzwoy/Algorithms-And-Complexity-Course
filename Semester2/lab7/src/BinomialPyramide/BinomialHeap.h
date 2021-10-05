#pragma once

#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;

//Клас, що описує біноміальну піраміду
template <typename T>
class BinomialHeap {
private:
	//Структура, що описує вузол піраміди
	struct Node {
		T key;
		unsigned degree;
		Node* parent, *next, *child;

		Node() : degree(0), parent(nullptr), next(nullptr), child(nullptr) {}

		Node(T const &_key) : Node() {
			key = _key;
		}

		//Під'єднує зліва до даного вузла нового сина
		void link(Node* new_child) {
			new_child->parent = this;
			new_child->next = this->child;
			this->degree++;
			this->child = new_child;
		}
	};

private:
	Node* root;

	//Повертає вузол з мінімальним ключем
	Node* findMin() const {
		if (!root) return nullptr;

		//Мінімальний ключ завжди знаходиться в списку коренів
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

	//Об'єднує корені даної піраміди з коренями вказаної, впорядковуючи їх в порядку зростання
	//причому вказана піраміда очищується, а утворена записується замість даної 
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

			//У якості нового кореня даної піраміди встановлюється корінь із меншим степенем
			this->root = result_ptr;

			//Поступово під'єднуємо до нового списку коренів корені з двох списків, обираючи менший корінь з двох, що порівнюються
			//Зупиняємося, коли один зі списків закінчиться
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

			//Під'єднуємо решту списку, що залишився

			if (this_ptr) {
				result_ptr->next = this_ptr;
			}

			if (that_ptr) {
				result_ptr->next = that_ptr;
			}
		}
		else {
			//Якщо вказана піраміда порожня - присвоюємо значення її кореня кореню даної піраміди
			//Інакше дана піраміда вже є результатом
			if (that_ptr) {
				this->root = that_ptr;
			}
		}

		that.root = nullptr;
	}

	//Обертає список вершин
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

	//Змінює ключ вказаної вершини, якщо новий ключ менше
	void decreaseKey(Node* node, T const &new_key) {
		if (!node || new_key > node->key) {
			throw std::invalid_argument("invalid operation, the key is greater or node is nullptr.");
		}

		node->key = new_key;
		Node* current = node, *parent = node->parent;
		//Обмінює ключі на шляху вгору, доки батьківський ключ не виявиться меншним або батька не буде взагалі
		while (parent && current->key < parent->key) {
			std::swap(current->key, parent->key);
			current = parent;
			parent = current->parent;
		}
	}

	//Шукає вершину за ключем
	Node* search(T const &key, Node* current) const {
		if (!current) return nullptr;
		else {
			if (current->key == key) return current;

			Node* found = nullptr;
			//Спочатку шукаємо серед дітей
			if (current->key < key) {
				found = search(key, current->child);
			}
			if (!found) {
				//Якщо не знайдено - шукаємо серед братів
				return search(key, current->next);
			}
			else return found;
		}
	}

	void remove(Node* to_remove) {
		if (!to_remove) return;
		//Присвоюємо вершині, що має видалитися, мінімальне значення
		decreaseKey(to_remove, this->getMin());
		//Видаляємо з піраміди мінімальне значення
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
 
	//Повертає мінімальний ключ
	T getMin() const {
		if (!root) {
			throw std::invalid_argument("invalid operation, the heap is empty.");
		}

		return findMin()->key;
	}

	//Зливає вказану піраміду з даною, вказана піраміда очищується
	void unite(BinomialHeap<T> &to_unite) {
		if (to_unite.empty()) return;

		//Перетворюємо дану піраміду у впорядкований список коренів
		this->merge(to_unite);
		if (!root) return;

		Node* prev = nullptr, *current = root, *next = root->next;
		while (next) {
			//Якщо даний корінь і наступний відрізняються степенями або якщо підряд ідуть три корені однакового степеня, зміщуємося вправо на одиницю
			if (current->degree != next->degree || (next->next && next->next->degree == current->degree)) {
				prev = current;
				current = next;
			}
			//Інакше збільшуємо порядок даного або наступного кореня на одиницю (під'єднуємо один із коренів)
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

	//Видаляє мінімальний ключ з піраміди й повертає його
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

		//Створюємо нову піраміду, список коренів якої є оберненим списком дітей мінімальної вершини
		BinomialHeap<T> temp;
		temp.root = inverse(min_node->child);
		
		//Прибираємо батька в кожної вершини нового списку коренів
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