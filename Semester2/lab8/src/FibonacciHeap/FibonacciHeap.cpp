#include "pch.h"
#include "FibonacciHeap.h"
#include "railway.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

int main()
{
	FibonacciHeap<railway> heap;
	heap.print();
	for (char i = 'A'; i < 'R'; i++) {
		heap.insert(railway{ string{ i } });
	}
	cout << endl << endl;
	heap.print();
	cout << endl;
	cout << "Min: " << heap.extractMin() << endl << endl;
	heap.print();
	heap.remove(string{ 'O' });
	cout << endl << endl << "O deleted:" << endl;
	heap.print();
	heap.remove(string{ 'P' });
	cout << endl << endl << "P deleted: " << endl;
	heap.print();

}