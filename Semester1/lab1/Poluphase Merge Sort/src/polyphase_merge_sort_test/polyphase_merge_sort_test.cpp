#include "pch.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <string>
#include <cassert>
#include <random>
#include <chrono>
using std::cout;
using std::fstream;
using std::ios;
using std::vector;
using std::string;

int get_random_int(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

void generate_bin(fstream &init_file, const int SIZE) {
	assert(SIZE > 0);
	int num;
	for (int i = 0; i < SIZE; i++) {
		num = get_random_int(0, INT32_MAX);
		init_file.write(reinterpret_cast<char*>(&num), sizeof(int));
	}
	init_file.clear();
	init_file.seekp(0);
}

void swap(int* left, int* right) {
	int temp = *left;
	*left = *right;
	*right = temp;
}

int partition(vector <int> &nums, int &i, int &j) {
	int pivot = nums[get_random_int(i, j)];
	while (i <= j) {
		while (nums[i] < pivot) i++;
		while (pivot < nums[j]) j--;
		if (i <= j) {
			swap(&nums[i], &nums[j]);
			i++;
			j--;
		}
	}
	return pivot;
}

void quick_sort(vector <int> &nums, int left, int right) {
	int i = left, j = right;
	partition(nums, i, j);
	if (j > left) quick_sort(nums, left, j);
	if (i < right) quick_sort(nums, i, right);
}

int count_nums(fstream &init_file) {
	char ch;
	int counter = 0;
	while (init_file) {
		init_file.get(ch);
		if (ch == ' ') counter++;
	}
	init_file.clear();
	init_file.seekg(0);
	return counter + 1;
}

int find_max(vector<int> const &arr) {
	int size = arr.size(), index = -1;
	int max = INT_MIN;
	for (int i = 0; i < size; i++) {
		if (arr[i] > max) {
			max = arr[i];
			index = i;
		}
	}
	return index;
}

int calc_sum(vector<int> const &arr) {
	if (arr.empty()) return -1;
	int size = arr.size();
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += arr[i];
	}
	return sum;
}

vector<int> calculate_runs(int limit, int num_of_files, bool demo = false) {
	assert(limit > 1 && num_of_files > 2);
	int num_of_runs = 1;
	vector<int> distribution;
	distribution.assign(num_of_files - 1, 1);
	distribution.push_back(0);
	if (demo) {
		for (int i = 1; i < num_of_files; i++) cout << "0 ";
		cout << "1\n";
		for (int i = 1; i < num_of_files; i++) cout << "1 ";
		cout << "0\n";
	}
	while (num_of_runs < limit) {
		int i = find_max(distribution);
		int max = distribution[i];
		for (int j = 0; j < num_of_files; j++) {
			distribution[j] += max;
		}
		distribution[i] = 0;
		if (demo) {
			for (int num : distribution) cout << num << " ";
			cout << '\n';
		}
		num_of_runs = calc_sum(distribution);
	}
	return distribution;
}

void distribute_nums(fstream &init_file, fstream files[], vector<int> const &distribution, const int MAX_NUMS, const int NUM_OF_BAD_RUNS, int separator) {
	int len = distribution.size(), long_runs_counter = NUM_OF_BAD_RUNS;
	vector<int> run(MAX_NUMS + 1);

	for (int i = 0; i < len; i++) {
		for (int j = 0; j < distribution[i]; j++) {
			for (int k = 0; k < MAX_NUMS; k++) {
				init_file.read(reinterpret_cast<char*>(&run[k]), sizeof(int));
			}
			if (long_runs_counter) {
				init_file.read(reinterpret_cast<char*>(&run[MAX_NUMS]), sizeof(int));
				quick_sort(run, 0, MAX_NUMS);
				for (int k = 0; k <= MAX_NUMS; k++) {
					files[i].write(reinterpret_cast<char*>(&run[k]), sizeof(int));
				}
				long_runs_counter--;
			}
			else {
				quick_sort(run, 0, MAX_NUMS - 1);
				for (int k = 0; k < MAX_NUMS; k++) {
					files[i].write(reinterpret_cast<char*>(&run[k]), sizeof(int));
				}
			}
			if (j != distribution[i] - 1) {
				files[i].write(reinterpret_cast<char*>(&separator), sizeof(int));
			}
		}
	}
	for (int i = 0; i < len; i++) {
		files[i].clear();
		files[i].seekp(0);
	}
}

int find_min(vector<int> const &nums, bool available[]) {
	int min = INT_MAX, min_ind = -1;
	for (unsigned int i = 0; i < nums.size(); i++) {
		if (available[i]) {
			if (nums[i] < min) {
				min = nums[i];
				min_ind = i;
			}
		}
	}
	return min_ind;
}

void pull_number(fstream files[], vector<int> &runs, bool available[], bool &file_processed, int to_read, int separator) {
	int num;

	if (available[to_read]) {
		if (files[to_read].peek() != EOF) {
			files[to_read].read(reinterpret_cast<char*>(&num), sizeof(int));
			if (num == separator) available[to_read] = false;
			else runs[to_read] = num;
		}
		else {
			available[to_read] = false;
			file_processed = true;
		}
	}
}

void merge_files(fstream files[], const int NUM_OF_FILES, const int MAX_NUMS, int separator) {
	vector<int> runs(NUM_OF_FILES);
	int num_of_empty, to_add;
	bool* available = new bool[NUM_OF_FILES], file_processed, next_step;

	while (files[0].peek() != EOF || files[1].peek() != EOF) {
		for (int i = 0; i < NUM_OF_FILES; i++) available[i] = true;
		for (int i = 0; i < NUM_OF_FILES; i++) {
			if (files[i].peek() == EOF) {
				num_of_empty = i;
				available[i] = false;
				files[i].clear();
				files[i].seekp(0);
				break;
			}
		}
		to_add = -1;
		file_processed = false;

		while (true) {
			if (to_add == -1) {
				for (int i = 0; i < NUM_OF_FILES; i++) {
					if (available[i]) pull_number(files, runs, available, file_processed, i, separator);
				}
			}
			next_step = true;
			for (int i = 0; i < NUM_OF_FILES; i++) {
				if (available[i]) {
					next_step = false;
					break;
				}
			}
			if (next_step) {
				if (file_processed) break;
				else {
					for (int i = 0; i < NUM_OF_FILES; i++) available[i] = true;
					available[num_of_empty] = false;
					files[num_of_empty].write(reinterpret_cast<char*>(&separator), sizeof(int));
					to_add = -1;
					continue;
				}
			}
			if (to_add != -1) {
				if (available[to_add]) pull_number(files, runs, available, file_processed, to_add, separator);
			}
			to_add = find_min(runs, available);
			if (to_add != -1) files[num_of_empty].write(reinterpret_cast<char*>(&runs[to_add]), sizeof(int));
		}

		files[num_of_empty].clear();
		files[num_of_empty].seekg(0);
		for (int i = 0; i < NUM_OF_FILES; i++) {
			if (files[i].peek() == EOF) {
				files[i].close();
				files[i].open(std::to_string(i + 1) + ".bin", ios::binary | ios::out | ios::in | ios::trunc);
				if (!(files[i].is_open())) std::cout << "Cannot open bin file\n";
			}
		}
	}
}

fstream* bin_to_txt(fstream& bin) {
	fstream* output = new fstream("output.txt", ios::out | ios::trunc);
	int num;
	while (bin.read(reinterpret_cast<char*>(&num), sizeof(int))) *output << num << " ";
	bin.clear();
	bin.seekg(0);
	output->clear();
	output->seekg(0);
	return output;
}

int polyphase_merge_sort(int max_nums, int num_of_files, int separator, fstream &init_file, bool demo = true) {
	int size, limit, num_of_bad_runs;
	vector<int> distribution;
	fstream* files = new fstream[num_of_files];
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	for (int i = 0; i < num_of_files; i++) {
		files[i].open(std::to_string(i + 1) + ".bin", ios::binary | ios::out | ios::in | ios::trunc);
		if (!(files[i].is_open())) std::cout << "Cannot open bin file\n";
	}

	init_file.clear();
	init_file.seekp(0, ios::end);
	size = (int)init_file.tellp() / sizeof(int);
	init_file.clear();
	init_file.seekp(0);
	limit = size / max_nums;
	if (demo) cout << "DISTRIBUTION\n";
	distribution = calculate_runs(limit, num_of_files, demo);
	int num_of_runs = calc_sum(distribution);
	max_nums = size / num_of_runs;
	num_of_bad_runs = size % num_of_runs;
	distribute_nums(init_file, files, distribution, max_nums, num_of_bad_runs, separator);
	merge_files(files, num_of_files, max_nums, separator);
	end = std::chrono::system_clock::now();
	int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	if (demo) cout << "\nDURATION: " << duration << "ms\n";
	bin_to_txt(files[num_of_files - 1]);
	return duration;
}

void benchmark() {
	fstream init_file("initial_file.bin", ios::binary | ios::in | ios::out | ios::trunc);
	if (!(init_file.is_open())) std::cout << "Cannot open bin file\n";
	fstream benchmark("benchmark.txt", ios::out | ios::trunc);
	if (!(benchmark.is_open())) std::cout << "Cannot open txt file\n";
	int separator = INT_MIN;

	int num_of_files[4];
	int initial_size[3];
	int ram_limit[2];
	for (int i = 0; i < 4; i++) num_of_files[i] = i + 4;
	initial_size[0] = 50000;
	initial_size[1] = 100000;
	initial_size[2] = 1000000;
	ram_limit[0] = 1000;
	ram_limit[1] = 10000;

	for (int i = 0; i < 4; i++) {
		benchmark << num_of_files[i] << " FILES\n";
		cout << num_of_files[i] << " FILES\n";
		for (int j = 0; j < 3; j++) {
			benchmark << ' ' << initial_size[j] << " NUMBERS\n";
			cout << ' ' << initial_size[j] << " NUMBERS\n";
			generate_bin(init_file, initial_size[j]);
			for (int k = 0; k < 2; k++) {
				int time = polyphase_merge_sort(ram_limit[k], num_of_files[i], separator, init_file, false);
				benchmark << "  " << ram_limit[k] << " nums in chunk: " << time << "ms\n";
				cout << "  " << ram_limit[k] << " nums in chunk: " << time << "ms\n";
				init_file.clear();
				init_file.seekg(0);
			}
			init_file.close();
			init_file.open("initial_file.bin", ios::binary | ios::in | ios::out | ios::trunc);
		}
	}
}

int main()
{
	/*int max_nums = 10000, num_of_files = 5, separator = INT_MIN;

	fstream init_file("initial_file.bin", ios::binary | ios::in | ios::out | ios::trunc);
	if (!(init_file.is_open())) std::cout << "Cannot open bin file\n";

	generate_bin(init_file, 1000000);
	polyphase_merge_sort(max_nums, num_of_files, separator, init_file);*/
	benchmark();
	return 0;
}