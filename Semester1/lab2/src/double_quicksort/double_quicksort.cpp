#include "pch.h"
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
using std::vector;

int get_random_int(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

void swap(int* left, int* right) {
	int temp = *left;
	*left = *right;
	*right = temp;
}

void partition(vector<int> &objects, int pivot, int &i, int &j) {
	while (i <= j) {
		while (objects[i] < pivot) i++;
		while (pivot < objects[j]) j--;
		if (i <= j) {
			swap(&objects[i], &objects[j]);
			i++;
			j--;
		}
	}
}

void quick_sort(vector<int> &nails, vector<int> &nuts, int left, int right) {
	int i = left, j = right;
	int pivot = nuts[left], pivot_index;
	for (pivot_index = left; pivot_index <= right; pivot_index++) {
		if (pivot == nails[pivot_index]) break;
	}
	swap(&nails[left], &nails[pivot_index]);
	partition(nails, pivot, i, j);
	for (pivot_index = left; pivot_index <= right; pivot_index++) {
		if (pivot == nails[pivot_index]) break;
	}
	pivot = nails[pivot_index];
	i = left, j = right;
	partition(nuts, pivot, i, j);
	if (j > left) quick_sort(nails, nuts, left, j);
	if (i < right) quick_sort(nails, nuts, i, right);
}

vector<int> generate_sequence(unsigned int size) {
	vector<int> seq(size);
	for (unsigned int i = 0; i < size; i++) seq[i] = get_random_int(0, INT_MAX);
	return seq;
}

vector<int> mix_sequence(vector<int> const &sample) {
	assert(!sample.empty());
	vector<int> result(sample.size());
	int offset = sample.size() / 2;
	for (unsigned int i = 0; i < sample.size(); i++) result[i] = sample[(i + offset) % sample.size()];
	return result;
}

int main()
{
	const int SIZE = 20;
	vector<int> nails = generate_sequence(SIZE);
	vector<int> nuts = mix_sequence(nails);

	quick_sort(nails, nuts, 0, SIZE - 1);

	for (auto nail : nails) std::cout << nail << ' ';
	std::cout << '\n';
	for (auto nut : nuts) std::cout << nut << ' ';
	std::cout << '\n';

	return 0;
}
