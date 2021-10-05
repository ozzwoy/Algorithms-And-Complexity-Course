#include "pch.h"
#include "BPlusTree.h"
#include "railway.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	BPlusTree<railway> tree(3);
	for (char i = 'A'; i < 'J'; i++) {
		tree.print();
		cout << endl << endl;
		tree.insert(railway{std::string(1, i)});
	}
	tree.print();
	cout << endl << endl;
	tree.remove(railway{ std::string(1, 'C') });
	tree.print();
	cout << endl << endl;
	tree.remove(railway{ std::string(1, 'B') });
	tree.print();
	cout << endl << endl;
	tree.remove(railway{ std::string(1, 'G') });
	tree.print();
	cout << endl << endl;
	tree.remove(railway{ std::string(1, 'A') });
	tree.print();
}