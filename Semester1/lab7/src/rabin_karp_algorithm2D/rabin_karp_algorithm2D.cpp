#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <random>
using std::cout;
using std::vector;

char get_random_char() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(33, 255);
	return char(dis(gen));
}

vector<vector<char>> get_random_matrix(unsigned int size) {
	vector<vector<char>> res(size);
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			res[i].push_back(get_random_char());
		}
	}
	return res;
}

vector<vector<char>> copy_matrix(vector<vector<char>> const &matrix, unsigned int x0, unsigned int y0, unsigned int size) {
	try {
		if (x0 + size > matrix.size() || y0 + size > matrix.size()) throw std::invalid_argument("wrong copy parameters");
	}
	catch (std::exception &exp) {
		cout << "ERROR: " << exp.what();
	}

	vector<vector<char>> res(size);
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			res[i].push_back(matrix[y0 + i][x0 + j]);
		}
	}

	return res;
}

int matrix_to_num(vector<vector<char>> const &matrix, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int mod) {
	int result = 0;
	for (unsigned int y = y1; y <= y2; y++) {
		for (unsigned int x = x1; x <= x2; x++) {
			result = (result * 10 + (int)matrix[y][x] % 10) % mod;
		}
	 }
	return result;
}

bool compare(vector<vector<char>> const &first, unsigned int x1, unsigned int y1,
	vector<vector<char>> const & second, unsigned int x2, unsigned int y2, unsigned int breadth, unsigned int height) {
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < breadth; j++) {
			if (first[y1 + i][x1 + j] != second[y2 + i][x2 + j]) return false;
		}
	}
	return true;
}

std::pair<unsigned int, unsigned int> find_sample(vector<vector<char>> const &matrix, vector<vector<char>> const &sample, const unsigned int MOD) {
	unsigned int sample_size = sample.size(), matrix_size = matrix.size();
	int sample_num = matrix_to_num(sample, 0, 0, sample_size - 1, sample_size - 1, MOD);
	int matrix_num = matrix_to_num(matrix, 0, 0, sample_size - 1, sample_size - 1, MOD);
	int temp = matrix_num;
	unsigned int limit = matrix_size - sample_size + 1;
	int coef_upper = (unsigned int)(pow(10, sample_size * (sample_size - 1))) % MOD;
	int coef_lower = (unsigned int)(pow(10, sample_size)) % MOD;
	int coef_right0 = (unsigned int)(pow(10, sample_size * sample_size - 1)) % MOD;
	int* coef_right = new int[sample_size - 1];
 
	for (int i = sample_size - 2; i >= 0; i--) {
		coef_right[i] = (int)(pow(10, sample_size * (i + 1))) % MOD;
	}

	for (unsigned int x = 0; x < limit; x++) {
		if (x != 0) {
			//move the pattern right, change the corresponding number 
			x--;
			matrix_num = ((temp - (matrix[0][x] % 10) * coef_right0) * 10) % MOD;
			for (unsigned int i = 0; i < sample_size - 1; i++) {
				matrix_num = (matrix_num - coef_right[i] * ((matrix[i + 1][x] % 10) - (matrix[i][x + sample_size] % 10))) % MOD;
			}
			matrix_num = (matrix_num + (matrix[sample_size - 1][x + sample_size] % 10)) % MOD;
			temp = matrix_num;
			x++;
		}
		for (unsigned int y = 0; y < limit; y++) {
			if (sample_num == matrix_num) {
				if (compare(matrix, x, y, sample, 0, 0, sample_size, sample_size)) {
					//compare pattern and sample directly
					return std::pair<unsigned int, unsigned int>(x, y);
				}
			}
			if (y != limit - 1) {
				//move the pattern down, change the corresponding number
				int upper_num = matrix_to_num(matrix, x, y, x + sample_size - 1, y, MOD);
				int lower_num = matrix_to_num(matrix, x, y + sample_size, x + sample_size - 1, y + sample_size, MOD);
				matrix_num = ((matrix_num - upper_num * coef_upper) * coef_lower + lower_num) % MOD;
			}
		}
	}

	return std::pair<unsigned int, unsigned int>(UINT_MAX, UINT_MAX);
}

int main()
{
	const unsigned int OFFSET_X = 60, OFFSET_Y = 20, MATRIX_SIZE = 100, SAMPLE_SIZE = 20, MOD = 10000;
	auto matrix = get_random_matrix(MATRIX_SIZE);
	auto sample = copy_matrix(matrix, OFFSET_X, OFFSET_Y, SAMPLE_SIZE);
	auto res = find_sample(matrix, sample, MOD);
	cout << "Set offset: " << OFFSET_X << ", " << OFFSET_Y << '\n';
	cout << "Calculated offset: " << res.first << ", " << res.second;
	return 0;
}