#include "pch.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
using std::cout;
using std::vector;

int get_random_int(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

template <typename T>
class max_heap {
	vector<T> arr;
	unsigned int d;

public:
	max_heap(unsigned int dimension) {
		assert(dimension > 1);
		d = dimension;
	}

	//complexity of build_max_heap (this constructor) is O(n)
	max_heap(unsigned int dimension, vector<T> const &sample) : arr(sample) {
		assert(dimension > 1);
		d = dimension;
		int i = (arr.size() - 2) / d;
		for (; i >= 0; i--) {
			max_heapify(i);
		}
	}

	unsigned int parent(unsigned int i) const {
		return i / d;
	}

	unsigned int child(unsigned int i, unsigned int num_of_child) const {
		assert(num_of_child < d);
		num_of_child++;
		return d * i + num_of_child;
	}

	unsigned int get_size() const {
		return arr.size();
	}

	unsigned int get_dimension() const {
		return d;
	}

	T get(unsigned int i) const {
		assert(i < arr.size());
		return arr[i];
	}
	
	//complexity of max_heapify is O(log base d of n)
	void max_heapify(unsigned int i) {
		assert(i < arr.size());
		unsigned int child_index, largest = i;
		for (unsigned int j = 0; j < d; j++) {
			child_index = child(i, j);
			if (child_index < arr.size()) {
				if (arr[child_index] > arr[largest]) largest = child_index;
			}
			else break;
		}
		if (largest != i) {
			T temp = arr[i];
			arr[i] = arr[largest];
			arr[largest] = temp;
			max_heapify(largest);
		}
	}

	//complexity of extract_max is O(log base d of n)
	T extract_max() {
		assert(!arr.empty());
		T max = arr[0];
		arr[0] = arr[arr.size() - 1];
		arr.pop_back();
		max_heapify(0);
		return max;
	}

	//complexity of increase_key is O(log base d of n)
	void increase_key(unsigned int i, T key) {
		assert(i < arr.size());
		if (key < arr[i]) return;
		arr[i] = key;
		while (i > 0 && arr[parent(i)] < arr[i]) {
			T temp = arr[i];
			arr[i] = arr[parent(i)];
			arr[parent(i)] = temp;
			i = parent(i);
		}
	}

	//complexity of insert is O(log base d of n)
	void insert(T key) {
		arr.push_back(key);
		increase_key(arr.size() - 1, key);
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, max_heap<T> piramide) {
	unsigned int d = piramide.get_dimension();
	for (unsigned int i = 0; i < piramide.get_size(); i++) cout << piramide.get(i) << ' ';
	return os;
}

int main()
{
	vector<int> initial;
	for (int i = 0; i < 20; i++) {
		initial.push_back(get_random_int(0, 20));
	}
	cout << "Vector: ";
	for (int el : initial) cout << el << ' ';
	cout << "\n\nPiramide: ";
	max_heap<int> piramide(4, initial);
	cout << piramide << "\n\n";
	cout << "Inserted 0, 200, -12: ";
	piramide.insert(0);
	piramide.insert(200);
	piramide.insert(-12);
	cout << piramide << "\n\n";
	cout << "Extracted max: " << piramide.extract_max() << "\n\n";
	cout << piramide << "\n\n";
	cout << "Increased the last key to 500: ";
	piramide.increase_key(piramide.get_size() - 1, 500);
	cout << piramide << '\n';
	return 0;
}