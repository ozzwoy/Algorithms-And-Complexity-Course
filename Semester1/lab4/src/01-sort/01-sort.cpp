#include "pch.h"
#include <iostream>
#include <random>
using std::cout;
using std::endl;
using std::vector;
using std::pair;

bool get_rand_bool() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	return bool(dis(gen));
}

vector<pair<bool, unsigned int>> get_rand_seq(unsigned int size) {
	unsigned int nuls = 0;
	pair<bool, unsigned int> temp;
	vector<std::pair<bool, unsigned int>> result;

	for (unsigned int i = 0; i < size; i++) {
		temp.first = get_rand_bool();
		if (temp.first) temp.second = i - nuls;
		else {
			temp.second = nuls;
			nuls++;
		}
		result.push_back(temp);
	}

	return result;
}

std::ostream& operator<< (std::ostream &os, pair<bool, unsigned int> const &elem) {
	os << '(' << elem.first << ',' << elem.second << ")";
	return os;
}

std::ostream& operator<< (std::ostream &os, vector<pair<bool, unsigned int>> const &seq) {
	os << "{ ";
	for (auto elem : seq) os << elem << ' ';
	os << '}';
	return os;
}

void linear_stable_sort(vector<pair<bool, unsigned int>> &seq) {
	vector<pair<bool, unsigned int>> temp(seq.size());
	unsigned int j = 0;
	for (unsigned int i = 0; i < seq.size(); i++) {
		if (seq[i].first == 0) {
			temp[j].first = 0;
			temp[j].second = seq[i].second;
			j++;
		}
	}
	for (unsigned int i = 0; i < seq.size(); i++) {
		if (seq[i].first == 1) {
			temp[j].first = 1;
			temp[j].second = seq[i].second;
			j++;
		}
	}
	seq = temp;
}

void linear_local_sort(vector<pair<bool, unsigned int>> &seq) {
	unsigned int i = 0, j = seq.size() - 1;
	while (i < j) {
		while (seq[i].first == 0) i++;
		while (seq[j].first == 1) j--;
		if (i < j) std::swap(seq[i], seq[j]);
	}
}

void insertion_sort(vector<pair<bool, unsigned int>> &seq, unsigned int left, unsigned int right) {
	for (unsigned int i = left; i <= right; i++) {
		for (unsigned int j = i; j > left; j--) {
			if (seq[j].first < seq[j - 1].first) std::swap(seq[j], seq[j - 1]);
			else break;
		}
	}
}

void stable_local_sort(vector<pair<bool, unsigned int>> &seq) {
	insertion_sort(seq, 0, seq.size() - 1);
}

int main()
{
	cout << "LINEAR STABLE SORT\n";
	auto seq = get_rand_seq(20);
	cout << seq << '\n';
	linear_stable_sort(seq);
	cout << seq << "\n\n";
	cout << "LINEAR LOCAL SORT\n";
	seq = get_rand_seq(20);
	cout << seq << '\n';
	linear_local_sort(seq);
	cout << seq << "\n\n";
	cout << "STABLE LOCAL SORT\n";
	seq = get_rand_seq(20);
	cout << seq << '\n';
	stable_local_sort(seq);
	cout << seq << endl;
	return 0;
}