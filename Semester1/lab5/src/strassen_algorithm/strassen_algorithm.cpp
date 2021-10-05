#include "pch.h"
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <chrono>
using std::cout;
using std::endl;
using std::vector;

int get_random_int(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

unsigned int find_max(unsigned int left, unsigned int right) {
	if (left < right) return right;
	return left;
}

unsigned int find_min(unsigned int left, unsigned int right) {
	if (left > right) return right;
	return left;
}

template <typename T>
class matrix {
	vector<vector<T>> matr;
public:
	matrix() {}
	matrix(unsigned int rows, unsigned int columns) : matr(rows) {
		vector<T> temp(columns);
		for (unsigned int i = 0; i < rows; i++) matr[i] = temp;
	}
	matrix(unsigned int size) : matr(size) {
		vector<T> temp(size);
		for (unsigned int i = 0; i < size; i++) matr[i] = temp;
	}
	matrix(matrix<T> const &sample) {
		matr = sample.matr;
	}
	bool empty() const {
		return matr.empty();
	}
	unsigned int rows() const {
		return matr.size();
	}
	unsigned int columns() const {
		if (matr.empty()) return 0;
		return matr[0].size();
	}
	void reserve(unsigned int rows, unsigned int columns) {
		if (!matr.empty()) return;
		vector<T> row(columns);
		for (unsigned int i = 0; i < rows; i++) matr.push_back(row);
	}
	T get(unsigned int i, unsigned int j) const {
		return matr[i][j];
	}
	void set(unsigned int i, unsigned int j, T value) {
		matr[i][j] = value;
	}
	void push_back_row() {
		if (matr.empty()) return;
		matr.push_back(vector<T>(this->columns()));
	}
	void push_back_column() {
		if (matr.empty()) return;
		T def = T();
		for (unsigned int i = 0; i < matr.size(); i++) {
			matr[i].push_back(def);
		}
	}
	void pop_back_row() {
		matr.pop_back();
	}
	void pop_back_column() {
		for (unsigned int i = 0; i < matr.size(); i++) matr[i].pop_back();
	}
	void clear() {
		matr.clear();
	}
	matrix<T> const &operator=(matrix<T> const &right) {
		matr = right.matr;
		return right;
	}
	matrix<T> operator+(matrix<T> const &right) const {
		matrix<T> result;
		if (this->rows() == right.rows() && this->columns() == right.columns()) {
			result.reserve(right.rows(), right.columns());
			for (unsigned int i = 0; i < right.rows(); i++) {
				for (unsigned int j = 0; j < right.columns(); j++) {
					result.set(i, j, matr[i][j] + right.get(i, j));
				}
			}
		}
		return result;
	}
	matrix<T> operator-(matrix<T> const &right) {
		matrix<T> result;
		if (this->rows() == right.rows() && this->columns() == right.columns()) {
			result.reserve(right.rows(), right.columns());
			for (unsigned int i = 0; i < right.rows(); i++) {
				for (unsigned int j = 0; j < right.columns(); j++) {
					result.set(i, j, matr[i][j] - right.get(i, j));
				}
			}
		}
		return result;
	}
	matrix<T> operator*(matrix<T> const &right) const {
		if (matr.empty() || right.empty() || this->columns() != right.rows() || this->rows() != right.columns()) return matrix<T>();
		unsigned int rows = this->rows(), cols = this->columns();
		matrix<T> result(rows, right.columns());
		T sum = T();

		for (unsigned int k = 0; k < rows; k++) {
			for (unsigned int i = 0; i < rows; i++) {
				for (unsigned int j = 0; j < cols; j++) {
					sum = sum + this->get(k, j) * right.get(j, i);
				}
				result.set(k, i, sum);
				sum = T();
			}
		}

		return result;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream &os, matrix<T> matr) {
	unsigned int rows = matr.rows(), cols = matr.columns();
	os << '{' << endl;
	if (!matr.empty()) {
		for (unsigned int i = 0; i < rows; i++) {
			os << "[ ";
			for (unsigned int j = 0; j < cols; j++) {
				os << matr.get(i, j) << ' ';
			}
			os << ']' << endl;
		}
	}
	os << '}' << std::endl;
	return os;
}

template <typename T>
matrix<T> strassen_algorithm(matrix<T> const &left, matrix<T> const &right) {
	matrix<T> result(left.rows(), left.columns());

	if (left.rows() <= 32) {
		result = left * right;
	}
	else {
		unsigned int size = left.rows() / 2;
		matrix<T> a(size), b(size), c(size), d(size), e(size), f(size), g(size), h(size), p1(size), p2(size), p3(size), p4(size), p5(size), p6(size), p7(size);
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) {
				a.set(i, j, left.get(i, j));
				e.set(i, j, right.get(i, j));
			}
		}
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) {
				b.set(i, j, left.get(i, size + j));
				f.set(i, j, right.get(i, size + j));
			}
		}
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) {
				c.set(i, j, left.get(size + i, j));
				g.set(i, j, right.get(size + i, j));
			}
		}
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) {
				d.set(i, j, left.get(size + i, size + j));
				h.set(i, j, right.get(size + i, size + j));
			}
		}

		p1 = strassen_algorithm(a, f - h);
		p2 = strassen_algorithm(a + b, h);
		p3 = strassen_algorithm(c + d, e);
		p4 = strassen_algorithm(d, g - e);
		p5 = strassen_algorithm(a + d, e + h);
		p6 = strassen_algorithm(b - d, g + h);
		p7 = strassen_algorithm(a - c, e + f);

		matrix<T> lu(p5 + p4 - p2 + p6), ru(p1 + p2), ld(p3 + p4), rd(p1 + p5 - p3 - p7);
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) result.set(i, j, lu.get(i, j));
		}
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) result.set(i, j + size, ru.get(i, j));
		}
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) result.set(i + size, j, ld.get(i, j));
		}
		for (unsigned int i = 0; i < size; i++) {
			for (unsigned int j = 0; j < size; j++) result.set(i + size, j + size, rd.get(i, j));
		}
	}

	return result;
}

template <typename T> 
matrix<T> multiply_by_strassen(matrix<T> const &left, matrix<T> const &right) {
	matrix<T> result;
	if (left.rows() != right.columns() || left.columns() != right.rows()) return result;
	matrix<T> m1 = left, m2 = right;
	unsigned int max, min, i, deg_val, delta;

	max = find_max(m1.rows(), m1.columns());
	min = find_min(m1.rows(), m1.columns());
	i = 0;
	while (max > pow(2, i)) i++;
	deg_val = (int)pow(2, i);
	delta = deg_val - left.rows();
	while (min != deg_val) {
		if (m1.rows() < deg_val) {
			m1.push_back_row();
			m2.push_back_column();
		}
		if (m2.rows() < deg_val) {
			m2.push_back_row();
			m1.push_back_column();
		}
		min++;
	}

	result = strassen_algorithm(m1, m2);
	for (unsigned int i = 0; i < delta; i++) {
		result.pop_back_row();
		result.pop_back_column();
	}
	return result;
}

matrix<int> get_random_matrix(unsigned int rows, unsigned int columns) {
	matrix<int> matr(rows, columns);
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			matr.set(i, j, get_random_int(-10, 10));
		}
	}
	return matr;
}

void benchmark() {
	vector<unsigned int> sizes{ 5, 10, 20, 50, 100, 200, 500 };
	std::ofstream out("benchmark.txt");
	std::chrono::time_point<std::chrono::system_clock> start, end;
	cout << "MATRIX\n\n";
	out << "MATRIX\n\n";

	for (unsigned int i = 0; i < sizes.size(); i++) {
		cout << sizes[i] << 'x' << sizes[i] << endl;
		out << sizes[i] << 'x' << sizes[i] << endl;
		auto m = get_random_matrix(sizes[i], sizes[i]);
		cout << "Simple multiplication: ";
		out << "Simple multiplication: ";
		start = std::chrono::system_clock::now();
		m * m;
		end = std::chrono::system_clock::now();
		cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
		out << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
		cout << "Strassen algorithm: ";
		out << "Strassen algorithm: ";
		start = std::chrono::system_clock::now();
		multiply_by_strassen(m, m);
		end = std::chrono::system_clock::now();
		cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n\n";
		out << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n\n";
	}
}

int main()
{
	matrix<int> A = get_random_matrix(15, 15), B = get_random_matrix(15, 15);
	cout << "Matrix A:" << '\n' << A << "\n\nMatrix B" << '\n' << B << "\n\n";
	cout << "Result:\n\n";
	cout << "Simple multiplication:\n" << A * B << "\n\n";
	cout << "Strassen algorithm: \n" << multiply_by_strassen(A, B);
	return 0;
}