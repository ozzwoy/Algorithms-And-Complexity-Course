#include "pch.h"
#include "railway.h"
#include "RBTreeExtended.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	RBTree<railway> tree;
	tree.insert(railway("Kyiv"));
	tree.insert(railway("Chernihiv"));
	tree.insert(railway("Warsaw"));
	tree.insert(railway("Poltava"));
	tree.insert(railway("Rivne"));
	tree.insert(railway("Kyiv"));
	tree.print();
	cout << endl;

	for (unsigned i = 0; i < 6; i++) {
		cout << i << ": " << tree[i] << endl;
	}
	cout << endl << endl;

	tree.remove(railway("Rivne"));
	tree.print();
	cout << endl;

	for (unsigned i = 0; i < 5; i++) {
		cout << i << ": " << tree[i] << endl;
	}
}