#include "pch.h"
#include "Graph.h"
#include "railway.h"
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

void bellmanFordAlgorithmTest() {
	cout << endl << endl << "-----------------------------------------" << endl << endl << endl;
	cout << "Bellman-Ford Algorithm Test" << endl << endl << endl;

	vector<railway> railways;
	railways.emplace_back("s");
	railways.emplace_back("t");
	railways.emplace_back("y");
	railways.emplace_back("x");
	railways.emplace_back("z");

	Graph<railway> graph(railways);

	graph.addEdge(0, 1, 6);
	graph.addEdge(0, 2, 7);
	graph.addEdge(1, 2, 8);
	graph.addEdge(1, 4, -4);
	graph.addEdge(1, 3, 5);
	graph.addEdge(3, 1, -2);
	graph.addEdge(2, 3, -3);
	graph.addEdge(2, 4, 9);
	graph.addEdge(4, 0, 2);
	graph.addEdge(4, 3, 7);
	graph.bellmanFordAlgorithm(0);

	for (size_t i = 0; i < graph.size(); i++) {
		cout << i << " - " << graph[i] << "   ";
	}
	cout << endl << endl << endl;

	cout << "    0  1  2  3  4" << endl << endl;
	cout << "0:";
	for (size_t i = 0; i < graph.size(); i++) {
		if (graph.getTotalCost(i) >= 0) cout << " ";
		cout << " " << graph.getTotalCost(i);
	}

	cout << endl;
}

void dijkstraAlgorithmTest() {
	cout << endl << endl << "-----------------------------------------" << endl << endl << endl;
	cout << "Dijkstra Algorithm Test" << endl << endl << endl;

	vector<railway> railways;
	railways.emplace_back("s");
	railways.emplace_back("t");
	railways.emplace_back("y");
	railways.emplace_back("x");
	railways.emplace_back("z");

	Graph<railway> graph(railways);

	graph.addEdge(0, 1, 10);
	graph.addEdge(0, 2, 5);
	graph.addEdge(1, 2, 2);
	graph.addEdge(2, 1, 3);
	graph.addEdge(1, 3, 1);
	graph.addEdge(2, 3, 9);
	graph.addEdge(2, 4, 2);
	graph.addEdge(3, 4, 4);
	graph.addEdge(4, 3, 6);
	graph.addEdge(4, 0, 7);
	graph.dijkstraAlgorithm(0);

	for (size_t i = 0; i < graph.size(); i++) {
		cout << i << " - " << graph[i] << "   ";
	}
	cout << endl << endl << endl;

	cout << "    0  1  2  3  4" << endl << endl;
	cout << "0:";
	for (size_t i = 0; i < graph.size(); i++) {
		if (graph.getTotalCost(i) >= 0) cout << " ";
		cout << " " << graph.getTotalCost(i);
	}

	cout << endl;
}

void johnsonAlgorithmTest() {
	cout << endl << endl << "-----------------------------------------" << endl << endl << endl;
	cout << "Johnson's Algorithm Test" << endl << endl << endl;

	vector<railway> railways;
	railways.emplace_back("A");
	railways.emplace_back("B");
	railways.emplace_back("C");
	railways.emplace_back("D");
	railways.emplace_back("E");

	Graph<railway> graph(railways);

	graph.addEdge(0, 1, 3);
	graph.addEdge(0, 2, 8);
	graph.addEdge(0, 4, -4);
	graph.addEdge(1, 3, 1);
	graph.addEdge(1, 4, 7);
	graph.addEdge(2, 1, 4);
	graph.addEdge(3, 0, 2);
	graph.addEdge(3, 2, -5);
	graph.addEdge(4, 3, 6);
	auto result = graph.johnsonAlgorithm();

	for (size_t i = 0; i < graph.size(); i++) {
		cout << i << " - " << graph[i] << "   ";
	}
	cout << endl << endl << endl;

	cout << "Matrix of weights" << endl << endl;
	cout << "     0   1   2   3   4" << endl << endl;
	for (size_t i = 0; i < graph.size(); i++) {
		cout << i << ":";
		for (size_t j = 0; j < graph.size(); j++) {
			if (result.first[i][j] < 0) cout << "  ";
			else cout << "   ";
			cout << result.first[i][j];
		}
		cout << endl;
	}
	cout << endl << endl;

	cout << "Matrix of ways" << endl << endl;
	cout << "    0  1  2  3  4" << endl << endl;
	for (size_t i = 0; i < graph.size(); i++) {
		cout << i << ":  ";
		for (size_t j = 0; j < graph.size(); j++) {
			cout << result.second[i][j] << "  ";
		}
		cout << endl;
	}

	cout << endl;
}

int main()
{
	bellmanFordAlgorithmTest();
	dijkstraAlgorithmTest();
	johnsonAlgorithmTest();
}