#include "pch.h"
#include "PersistentRBTree.h"
#include "railway.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

int main()
{
	PersistentRBTree<railway> tree;
	for (char i = 'E'; i < 'I'; i++) {
		tree.insert(string{ i });
	}

	for (char i = 'A'; i < 'E'; i++) {
		tree.insert(string{ i });
	}

	for (char i = 'E'; i < 'I'; i++) {
		tree.remove(string{ i });
	}

	for (char i = 'A'; i < 'E'; i++) {
		tree.remove(string{ i });
	}

	cout << endl << endl;
	for (size_t i = 0; i < 18; i++) {
		cout << "  " << i << " steps back:" << endl << endl;
		tree.printPrevious(i);
		cout << endl << endl;
	}
}