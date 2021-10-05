#include "pch.h"
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using std::string;

vector<unsigned int> calc_z(string const &str) {
	const unsigned int N = str.length();
	vector<unsigned int> z(N);
	unsigned int seq_begin = 0, seq_end = 0, k;

	for (unsigned int i = 1; i < N; ++i)
	{
		if (i > seq_end) {
			seq_begin = seq_end = i;
			while (seq_end < N && str[seq_end - seq_begin] == str[seq_end])
				seq_end++;
			z[i] = seq_end - seq_begin;
			seq_end--;
		}
		else {
			k = i - seq_begin;
			if (z[k] < seq_end - i + 1) z[i] = z[k];
			else {
				seq_begin = i;
				while (seq_end < N && str[seq_end - seq_begin] == str[seq_end])
					seq_end++;
				z[i] = seq_end - seq_begin;
				seq_end--;
			}
		}
	}
	return z;
}

void shift_checker(string const &text, string const &varifiable) {
	if (text.length() != varifiable.length()) {
		cout << "Not shifted." << endl;
		return;
	}
	string concat = varifiable + "$" + text + text;
	const unsigned int L = concat.length();
	bool found = false;

	auto z = calc_z(concat);
	z[0] = 0;
	for (unsigned int i = 0; i < L; i++) {
		if (z[i] == varifiable.length()) {
			cout << "Shift: " << i - varifiable.length() - 1 << endl;
			found = true;
			break;
		}
	}
	if (!found) cout << "Not shifted." << endl;
}

int main()
{
	string str1("aaaaabaaa"), str2("abaaaaaaa");
	shift_checker(str1, str2);
	return 0;
}