#pragma once

#include "FibonacciHeap.h"
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>
using std::endl;
using std::list;
using std::vector;

//����, �� ����� ���������� ����
template <typename T>
class Graph {
private:
	//���������, �� ����� ������� �����
	struct Node {
		size_t id;
		T data;
		double total_cost;
		Node* parent;

		Node()  {}

		Node(size_t _id, T const &_data) : id(_id), data(_data) {}

		bool operator<(Node rhs) const {
			return this->total_cost < rhs.total_cost;
		}

		bool operator<=(Node rhs) const {
			return this->total_cost <= rhs.total_cost;
		}

		bool operator>(Node rhs) const {
			return this->total_cost > rhs.total_cost;
		}

		bool operator==(Node const &rhs) const {
			return id == rhs.id;
		}

		friend std::ostream& operator<<(std::ostream& os, Node const &node) {
			return os;
		}
	};

	//���������, �� ����� ����� �����
	struct Edge {
		size_t to;
		double weight;

		Edge(size_t _to, double _weight) : to(_to), weight(_weight) {}
	};

private:
	//����� ������
	vector<Node*> vertices;
	//������ ��������
	vector<list<Edge>> adjacency_lists;
	//���������, �� ������ �������� "������������"
	static const double MAX_COST;

	//����� ��� ������ �� ���� ������, ������� ���� ��������� �������
	void initSignleSource(size_t main_vertex) {
		for (Node* v : vertices) {
			v->total_cost = MAX_COST;
			v->parent = nullptr;
		}
		vertices[main_vertex]->total_cost = 0;
	}

	//�������� �������
	bool relax(size_t from, size_t to, double weight) {
		if (vertices[from]->total_cost == MAX_COST) return false;

		if (vertices[to]->total_cost > vertices[from]->total_cost + weight) {
			vertices[to]->total_cost = vertices[from]->total_cost + weight;
			vertices[to]->parent = vertices[from];
			return true;
		}
		return false;
	}

	//�������� �� �������, ����� � �����
	void relaxAdjacent(size_t from) {
		for (Edge edge : adjacency_lists[from]) {
			relax(from, edge.to, edge.weight);
		}
	}

public:
	Graph() {}

	Graph(vector<T> const &data) {
		for (T const &item : data) {
			addVertex(item);
		}
	}

	Graph(Graph<T> const *source) {
		for (Node* v : source->vertices) {
			this->addVertex(v->data);
		}

		for (size_t i = 0; i < source->adjacency_lists.size(); i++) {
			for (Edge edge : source->adjacency_lists[i]) {
				this->addEdge(i, edge.to, edge.weight);
			}
		}
	}

	bool empty() const {
		return vertices.empty();
	}

	size_t size() const {
		return vertices.size();
	}

	size_t num_of_edges() const {
		size_t sum = 0;
		for (auto list : adjacency_lists) {
			sum += list.size();
		}
		return sum;
	}

	T operator[](size_t n) const {
		if (n >= vertices.size()) {
			throw std::out_of_range("graph out of range.");
		}

		return vertices[n]->data;
	}

	//���� �������
	void addVertex(T const &data) {
		vertices.push_back(new Node(vertices.size(), data));
		adjacency_lists.emplace_back();
	}

	//���� �����
	void addEdge(size_t from, size_t to, double weight) {
		if (from >= vertices.size() || to >= vertices.size()) {
			throw std::out_of_range("graph out of range.");
		}
		if (from == to) {
			throw std::invalid_argument("graph mustn't have loops.");
		}

		for (Edge edge : adjacency_lists[from]) {
			if (edge.to == to) {
				edge.weight = weight;
				return;
			}
		}

		adjacency_lists[from].emplace_back(to, weight);
	}

	friend std::ostream& operator<<(std::ostream& os, Graph const &graph) {
		{
			if (graph.empty()) {
				os << "The graph is empty." << endl;
			}

			for (size_t i = 0; i < graph.size(); i++) {
				os << i << ": ";
				for (auto edge : graph.adjacency_lists[i]) {
					os << edge.to << "(" << edge.weight << ")  ";
				}
				os << endl;
			}

			return os;
		}
	}

	//������� ���� �������
	double getTotalCost(size_t n) const {
		if (n >= vertices.size()) {
			throw std::out_of_range("graph out of range.");
		}
		return vertices[n]->total_cost;
	}

	vector<size_t> getShortestPath(size_t to) const {
		if (to >= vertices.size()) {
			throw std::out_of_range("graph out of range.");
		}

		vector<size_t> path;
		path.push_back(to);
		while (vertices[to]->parent) {
			to = vertices[to]->parent->id;
			path.push_back(to);
		}
	}

	//�������� ��������-�����
	bool bellmanFordAlgorithm(size_t init_vertex) {
		if (init_vertex >= vertices.size()) {
			throw std::out_of_range("graph out of range.");
		}

		//������� ������� ���� ������
		initSignleSource(init_vertex);

		//�� ����� �������� ���������� �� �����
		for (size_t i = 0; i < vertices.size() - 1; i++) {
			for (size_t j = 0; j < vertices.size(); j++) {
				relaxAdjacent(j);
			}
		}

		//����������, �� � ���� � ��'����� �����
		for (size_t i = 0; i < vertices.size(); i++) {
			for (Edge edge : adjacency_lists[i]) {
				if (relax(i, edge.to, edge.weight)) return false;
			}
		}
		return true;
	}

	//�������� ��������
	void dijkstraAlgorithm(size_t init_vertex) {
		if (init_vertex >= vertices.size()) {
			throw std::out_of_range("graph out of range.");
		}

		//������� ������� ���� ������
		initSignleSource(init_vertex);

		//������ ������� � ������
		FibonacciHeap<Node> heap;
		for (Node* v : vertices) {
			heap.insert(*v);
		}

		Node current;
		while (!heap.empty())
		{
			//���� ������ ���������, �������� ������� � ��������� �������� �� ��������� �������
			current = heap.extractMin();

			//���������� �� �������, ����� � �����
			Node buffer;
			for (Edge edge : adjacency_lists[current.id]) {
				buffer = *vertices[edge.to];
				if (relax(current.id, edge.to, edge.weight)) {
					//���� ���������� ��������, ������� �������� ������� � �����
					heap.decreaseKey(buffer, *vertices[edge.to]);
				}
			}
		}
	}

	std::pair<vector<vector<double>>, vector<vector<size_t>>> johnsonAlgorithm() {
		//��������� ���������� ��� ��� �� ������
		vector<vector<double>> weights(vertices.size());
		vector<vector<size_t>> ways(vertices.size());
		for (vector<double> &v : weights) {
			v.resize(vertices.size());
		}
		for (vector<size_t> &v : ways) {
			v.resize(vertices.size());
		}

		//��������� ����� ���������� ����
		Graph<T> expanded_graph(this);
		expanded_graph.addVertex(T());
		size_t index = vertices.size();
		for (size_t i = 0; i < index; i++) {
			expanded_graph.addEdge(index, i, 0);
		}

		//���� ���� ��'����� �����, ����������
		if (expanded_graph.bellmanFordAlgorithm(index)) {

			//�����'������� ��������, ��������� ���������� ��������-�����
			for (size_t i = 0; i < index; i++) {
				vertices[i]->total_cost = expanded_graph.vertices[i]->total_cost;
			}

			//���������� ������ ���� ����'���� ����
			for (size_t i = 0; i <= index; i++) {
				for (Edge &edge : expanded_graph.adjacency_lists[i]) {
					edge.weight += expanded_graph.vertices[i]->total_cost - expanded_graph.vertices[edge.to]->total_cost;
				}
			}

			//��� ����� ������� ������ ���������� ����� ���������� ��������, ������������ �������� ���� ����� �������
			for (size_t i = 0; i < index; i++) {
				expanded_graph.dijkstraAlgorithm(i);
				for (size_t j = 0; j < index; j++) {
					weights[i][j] = expanded_graph.vertices[j]->total_cost + vertices[j]->total_cost - vertices[i]->total_cost;
					Node* parent = expanded_graph.vertices[j]->parent;
					ways[i][j] = parent ? parent->id : j;
				}
			}
		}
		//������ ��������� ������ ������
		else {
			weights.clear();
			ways.clear();
		}

		return std::make_pair(weights, ways);
	}
};

template <typename T>
const double Graph<T>::MAX_COST = 1e12;