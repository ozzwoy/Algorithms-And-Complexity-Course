#pragma once

#include <iostream>
using std::cout;

//����, �� ����� �������-����� ������
template <typename T>
class RBTree {
private:
	enum class Color { BLACK, RED };

	//���������, �� ����� ������� ������
	struct Node {
	public:
		T key;
		Color color;
		Node* parent, *left, *right;
		//ʳ������ ������� ���� �������
		unsigned num_of_descendants;

		Node(T const &key, Color color, Node* parent, Node* left, Node* right, unsigned num_of_descendants) : key(key), color(color),
			parent(parent), left(left), right(right), num_of_descendants(num_of_descendants) {}
	};

private:
	Node* root = nullptr;
	//������ �������: �����, ���� ���, ������ ���
	enum class Status { ROOT, LEFT, RIGHT };

	//�'���� ������ � ������ �� ���� �� �������
	void link(Node* parent, Node* left, Node* right) {
		if (parent) {
			parent->left = left;
			parent->right = right;
		}
		if (left) {
			left->parent = parent;
		}
		if (right) {
			right->parent = parent;
		}
	}

	//������ ������� ���� ������ ������
	void rotate(Node* parent, Node* child) {
		if (!parent || !child) return;

		Node* grandparent = parent->parent;
		child->parent = grandparent;

		if (grandparent) {
			if (parent == grandparent->left) {
				grandparent->left = child;
			}
			else {
				grandparent->right = child;
			}
		}
		else {
			root = child;
		}

		//���������� �������� ������ ���������� ������� �������
		child->num_of_descendants = parent->num_of_descendants;
		//��������� ������� ������� � ������
		parent->num_of_descendants = 0;
		//� ��������� �� ��������� ���� ������� ������ ��������� �������
		//������ �� ������� ������� ������ ������� ������� � ���� ����� ���� + 2 (���������� ��� ������� �������)
		if (child == parent->left) {
			if (child->right) {
				parent->num_of_descendants += child->right->num_of_descendants + 1;
			}
			if (parent->right) {
				parent->num_of_descendants += parent->right->num_of_descendants + 1;
			}

			link(parent, child->right, parent->right);
			link(child, child->left, parent);
		}
		else {
			if (child->left) {
				parent->num_of_descendants += child->left->num_of_descendants + 1;
			}
			if (parent->left) {
				parent->num_of_descendants += parent->left->num_of_descendants + 1;
			}

			link(parent, parent->left, child->left);
			link(child, parent, child->right);
		}
	}

	//³������� �������-���� ���������� ���� ������� �������
	void insertFixup(Node* current) {
		while (current && current->color == Color::RED) {
			Node* parent = current->parent;
			if (parent) {
				Node* grandparent = parent->parent;
				Node* uncle;

				if (grandparent) {
					if (parent == grandparent->left) {
						uncle = grandparent->right;
					}
					else {
						uncle = grandparent->left;
					}

					//������� 1 (����� ������, ������ ��������)
					if (uncle && uncle->color == Color::RED) {
						grandparent->color = Color::RED;
						parent->color = uncle->color = Color::BLACK;
						current = grandparent;
						continue;
					}
					else {
						//zigzig �������, �� ���, ������ �� �� ������ �� ���� �����
						bool zigzig = (grandparent->left == parent) == (parent->left == current);

						//������� 2 (������ ������, ������� ������� � ������ ��������)
						if (!zigzig) {
							rotate(parent, current);
							current = parent;
							continue;
						}
						//������� 3 (������ ������, ������� ������� � ���� ��������)
						else {
							parent->color = Color::BLACK;
							rotate(grandparent, parent);
						}

						grandparent->color = Color::RED;
					}

					break;
				}

				break;
			}

			break;
		}
		root->color = Color::BLACK;
	}

	//���������� ��������� �-�� ��������� ����������
	Node* select(Node* current, unsigned i) const {
		if (!current) {
			return nullptr;
		}
		unsigned curr_index = 0;
		if (current->left) {
			curr_index = current->left->num_of_descendants + 1;
		}
		if (i == curr_index) {
			return current;
		}
		else if (i < curr_index) {
			return select(current->left, i);
		}
		else {
			return select(current->right, i - curr_index - 1);
		}
	}

	//���� ������� � ������� ������
	Node* search(T const &key) const {
		Node* current = root;

		while (current) {
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

	//������ ������� ������� �� ������� � ��� ������ �� ����� �� ������ �� ������ ������� (�������)
	void decreaseUp(Node* current) {
		while (current) {
			current->num_of_descendants--;
			current = current->parent;
		}
	}

	//³������� �������-���� ���������� ���� ��������� �������
	void removeFixup(Node* current) {
		if (current == root) {
			current->color = Color::BLACK;
			return;
		}

		while (current) {
			Node* parent = current->parent;
			Node* brother = current == parent->left ? parent->right : parent->left;

			if (brother) {
				//������� 1 (���� ��������)
				if (brother->color == Color::RED) {
					rotate(parent, brother);
					parent->color = Color::RED;
					brother->color = Color::BLACK;
					continue;
				}
				
				Node* left_nephew = brother->left, *right_nephew = brother->right;
				if (!right_nephew || right_nephew->color == Color::BLACK) {
					//������� 2 (���� ������, ��������� ����)
					if (!left_nephew || left_nephew->color == Color::BLACK) {
						brother->color = Color::RED;
						current = parent;
						break;
					}
					//������� 3 (���� ������, ���� �������� ��������, ������ - ������)
					if (left_nephew && left_nephew->color == Color::RED) {
						rotate(brother, left_nephew);
						left_nephew->color = Color::BLACK;
						parent->color = Color::RED;
						continue;
					}
				}
				//������� 4 (���� ������, ������ �������� ��������)
				if (right_nephew && right_nephew->color == Color::RED) {
					rotate(parent, brother);
					brother->color = parent->color;
					parent->color = Color::BLACK;
					right_nephew->color = Color::BLACK;
					break;
				}
			}
			else {
				break;
			}
		}

		if (current) {
			current->color = Color::BLACK;
		}
	}

	//���������� �������� ����� ������
	void print(Node* current, unsigned level = 0, Status stat = Status::ROOT) const {
		if (!current) {
			return;
		}
		for (unsigned i = 0; i < level; i++) {
			cout << "  ";
		}
		cout << level;

		switch (stat) {
		case Status::ROOT:
			cout << "(o)";
			break;
		case Status::LEFT:
			cout << "(left)";
			break;
		default:
			cout << "(right)";
		}

		if (current->color == Color::BLACK) {
			cout << "(black)";
		}
		else {
			cout << "(red)";
		}

		cout << ": " << current->key << std::endl;
		print(current->left, level + 1, Status::LEFT);
		print(current->right, level + 1, Status::RIGHT);
	}

public:
	RBTree() {}

	RBTree(T const &key) {
		root = new Node(key, Color::BLACK, nullptr, nullptr, nullptr, 0);
	}

	bool empty() const {
		return root == nullptr;
	}

	//�������� ������� � ������� ������
	void insert(T const &key) {
		Node* current = root, *temp = nullptr;
		while (current) {
			temp = current;
			current->num_of_descendants++;
			if (key < current->key) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		Node* new_node = new Node(key, Color::RED, temp, nullptr, nullptr, 0);
		if (temp) {
			if (key < temp->key) {
				temp->left = new_node;
			}
			else {
				temp->right = new_node;
			}
			insertFixup(new_node);
		}
		else {
			root = new_node;
			new_node->color = Color::BLACK;
		}
	}

	//������� ������� � ������� ������
	void remove(T const &key) {
		Node* current = search(key);
		if (!current) {
			return;
		}

		Node* replacement;
		if (!current->left || !current->right) {
			decreaseUp(current->parent);

			replacement = current->left ? current->left : current->right;
			if (replacement) {
				replacement->parent = current->parent;
			}

			if (current == root) {
				root = replacement;
			}
			else if (current == current->parent->left) {
				current->parent->left = replacement;
			}
			else {
				current->parent->right = replacement;
			}

			if (current->color == Color::BLACK) {
				removeFixup(replacement);
			}

			delete current;
		}
		else {
			replacement = current->right;
			while (replacement->left) {
				replacement = replacement->left;
			}

			decreaseUp(replacement->parent);

			if (replacement == current->right) {
				link(current, current->left, replacement->right);
			}
			else {
				link(replacement->parent, replacement->right, replacement->parent->right);
			}
			current->key = replacement->key;

			Node* temp = replacement;
			replacement = replacement->right;
			if (temp->color == Color::BLACK) {
				removeFixup(replacement);
			}

			delete temp;
		}
	}

	//������� �-�� ��������� ����������
	T operator[](unsigned i) const {
		Node* found = select(root, i);
		if (found) {
			return found->key;
		}
		throw std::invalid_argument("Tree is out of range.");
	}

	//����� ������
	void print() const {
		if (root) {
			print(root);
		}
		else {
			cout << "The tree is empty." << std::endl;
		}
	}
};