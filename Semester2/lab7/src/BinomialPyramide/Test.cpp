#include "pch.h"
#include "BinomialHeap.h"
#include "railway.h"
#include <iostream>
#include <string>
using std::string;

int main()
{
	BinomialHeap<railway> heap;
	for (char i = 'A'; i < 'J'; i++) {
		heap.insert(railway{ string{ i } });
	}
	heap.print();
	cout << endl << endl;

	cout << "A: ";
	if (heap.isIn(railway{ string{ 'A' } })) {
		cout << "is in." << endl;
	}
	else {
		cout << "is not in." << endl;
	}

	cout << "Extracted min: ";
	cout << heap.extractMin() << endl;

	cout << "A: ";
	if (heap.isIn(railway{ string{ 'A' } })) {
		cout << "is in." << endl;
	}
	else {
		cout << "is not in." << endl;
	}
	cout << endl << endl;

	heap.print();
	cout << endl << endl;

	heap.remove(railway{ string{ 'D' } });
	cout << "Removed D:" << endl;
	heap.print();
	cout << endl;
	heap.remove(railway{ string{ 'E' } });
	cout << "Removed E:" << endl;
	heap.print();
}