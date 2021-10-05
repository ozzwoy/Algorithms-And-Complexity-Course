#include "pch.h"
#include "OptimalBST.h"
#include "railway.h"
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

int main()
{
	vector<railway> keys{ railway{"Berlin"}, railway{"Kyiv"}, railway{"Paris"}, railway{"Tallinn"}, railway{"Warsaw"} };
	vector<double> true_probs{ 0.15, 0.10, 0.05, 0.10, 0.20 };
	vector<double> false_probs{ 0.05, 0.10, 0.05, 0.05, 0.05, 0.10 };

	OptimalBST<railway> tree(keys, true_probs, false_probs);
	tree.print();
	cout << endl << "Cost: " << tree.getCost();
}