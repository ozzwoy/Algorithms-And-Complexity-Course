#include "pch.h"
#include <iostream>
#include <cassert>
#include <random>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

int get_random_int(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

char get_random_char(char min, char max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

string get_random_bin_seq(int length) {
	string text;
	for (int i = 0; i < length; i++) {
		text += get_random_char('0', '1');
	}
	return text;
}

string get_random_text(int length) {
	string text;
	for (int i = 0; i < length; i++) {
		text += get_random_char('a', 'z');
	}
	return text;
}

size_t naive_algorithm(string const &text, string const &pattern) {
	assert(!text.empty() && !pattern.empty() && text.size() > pattern.size());
	size_t end = text.size() - pattern.size() + 1;
	bool found = true;
	for (size_t i = 0; i < end; i++) {
		if (text[i] == pattern[0]) {
			for (size_t j = 1; j < pattern.size(); j++) {
				if (text[i + j] != pattern[j]) {
					found = false;
					break;
				}
			}
			if (found) return i;
			else found = true;
		}
	}
	return SIZE_MAX;
}

unsigned int string_to_num(string const &text, size_t left, size_t right, const unsigned int MOD) {
	assert(left < right && left < text.size() || right < text.size());
	unsigned int num = 0;
	for (size_t i = left; i <= right; i++) {
		num = (num * 10 + text[i] % 10) % MOD;
	}
	return num;
}

size_t rabin_karp_algorithm(string const &text, string const &pattern) {
	assert(!text.empty() && !pattern.empty() && text.size() > pattern.size());
	const unsigned int MOD = 5000;
	unsigned int pattern_num = string_to_num(pattern, 0, pattern.size() - 1, MOD), num = string_to_num(text, 0, pattern.size() - 1, MOD), h = 1;
	size_t end = text.size() - pattern.size() + 1;

	for (size_t i = 0; i < pattern.size() - 1; i++) h = (h * 10) % MOD;

	for (size_t i = 0; i < end; i++) {
		if (num == pattern_num) {
			bool found = true;
			for (size_t j = 0; j < pattern.size(); j++) {
				if (text[i + j] != pattern[j]) {
					found = false;
					break;
				}
			}
			if (found) return i;
		}
		if (i != end - 1) {
			num = ((num - (text[i] % 10) * h) * 10 + text[i + pattern.size()] % 10) % MOD;
		}
	}

	return SIZE_MAX;
}

vector<size_t> prefix_function(string const &text) {
	vector<size_t> values(text.size());
	size_t k = 0;
	values[0] = 0;
	for (size_t i = 1; i < text.size(); i++) {
		while (k > 0 && text[k] != text[i]) k = values[k - 1];
		if (text[k] == text[i]) k++;
		values[i] = k;
	}
	return values;
}

size_t knuth_morris_pratt_algorithm(string const &text, string const &pattern) {
	assert(!text.empty() && !pattern.empty() && text.size() > pattern.size());
	auto prefix_values = prefix_function(pattern);
	size_t matched = 0, end = text.size() - pattern.size() + 1;

	for (size_t i = 0; i < end; i++) {
		while (matched > 0 && pattern[matched] != text[i]) matched = prefix_values[matched - 1];
		if (pattern[matched] == text[i]) matched++;
		if (matched == pattern.size()) return i - pattern.size() + 1;
	}
	return SIZE_MAX;
}

size_t horspool_algorithm(string const &text, string const &pattern) {
	assert(!text.empty() && !pattern.empty() && text.size() > pattern.size());
	size_t offsets[255];
	for (int i = 0; i < 255; i++) {
		offsets[i] = pattern.size();
	}
	for (size_t i = 0; i < pattern.size() - 1; i++) {
		offsets[(int)pattern[i]] = pattern.size() - i - 1;
	}

	size_t i = pattern.size() - 1;
	for (size_t j = pattern.size() - 1; i < text.size() && j > 0; j--, i--) {
		if (text[i] != pattern[j]) {
			i += offsets[(int)text[i]] + 1;
			j = pattern.size();
		}
	}
	if (i < text.size()) {
		return i;
	}
	else return SIZE_MAX;
}

int boyer_moore_algorithm(string const &text, string const &pattern) {
	assert(!text.empty() && !pattern.empty() && text.size() > pattern.size());

	size_t stop_table[255];
	for (int i = 0; i < 255; i++) {
		stop_table[i] = pattern.size();
	}
	for (unsigned int i = 0; i < pattern.size() - 1; i++) {
		stop_table[(int)pattern[i]] = pattern.size() - i - 1;
	}

	vector<size_t> suffix_table(pattern.size());
	string reversed_pattern(pattern.rbegin(), pattern.rend());
	vector<size_t> prefix_table = prefix_function(pattern), reversed_prefix_table = prefix_function(reversed_pattern);

	for (size_t i = 0; i < pattern.size(); i++) {
		suffix_table[i] = pattern.size() - prefix_table.back();
	}
	for (size_t i = 1; i < pattern.size(); i++) {
		size_t j = reversed_prefix_table[i];
		suffix_table[j] = std::min(suffix_table[j], i - reversed_prefix_table[i] + 1);
	}

	size_t end = text.size() - pattern.size(), shift = 0, pos;
	while (shift <= end) {
		pos = pattern.size() - 1;

		while (pattern[pos] == text[pos + shift]) {
			if (pos == 0) return shift;
			pos--;
		}

		/*if (pos == pattern.size() - 1) {
			shift += stop_table[(int)text[pos + shift]];
		}
		else {
			shift += suffix_table[pattern.size() - pos - 1];
		}*/
		shift += suffix_table[pattern.size() - pos - 1];
	}

	return SIZE_MAX;
}

int main()
{
	string text = get_random_text(10000), pattern;
	for (unsigned int i = 8800; i < 8900; i++) pattern += text[i];
	cout << text << endl << endl << pattern << endl << endl;

	cout << "REAL OFFSET: 8800" << endl << endl;

	cout << "NAIVE ALGORITHM: " << naive_algorithm(text, pattern) << endl;
	cout << "RABIN-KARP ALGORITHM: " << rabin_karp_algorithm(text, pattern) << endl;
	cout << "KNUTH-MORRIS-PRATT ALGORITHM: " << knuth_morris_pratt_algorithm(text, pattern) << endl;
	cout << "HORSPOOL ALGORITHM: " << horspool_algorithm(text, pattern) << endl;
	cout << "BOYER-MOORE ALGORITHM: " << boyer_moore_algorithm(text, pattern) << endl;
}