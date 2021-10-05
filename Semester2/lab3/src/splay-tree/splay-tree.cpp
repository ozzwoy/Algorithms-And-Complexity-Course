#include "pch.h"
#include "railway.h"
#include "splay_tree.h"
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

void test() {
	splay_tree<railway> tree;
	tree.print();
	cout << endl << endl;
	tree.add(railway{ "Kyiv Railway" });
	tree.print();
	cout << endl << endl;
	tree.add(railway{ "Dnipro Railway" });
	tree.print();
	cout << endl << endl;
	tree.add(railway{ "Kharkiv Railway" });
	tree.print();
	cout << endl << endl;
	tree.add(railway{ "Odesa Railway" });
	tree.print();
	cout << endl << endl;
	if (tree.search(railway{ "Dnipro Railway" })) {
		cout << "Found." << endl << endl;
	}
	tree.print();
}

int main()
{
	test();
}