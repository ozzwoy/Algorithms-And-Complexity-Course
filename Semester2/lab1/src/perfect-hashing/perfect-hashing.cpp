#include "pch.h"
#include "hash.h"
#include "railway.h"
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

int main()
{
	vector<station> stations;
	stations.push_back(station{ "Bucha" });
	stations.push_back(station{ "Boyarka" });
	stations.push_back(station{ "Vorzel" });
	stations.push_back(station{ "Nemishayeve" });
	stations.push_back(station{ "Hostomel" });
	stations.push_back(station{ "Irpin" });
	stations.push_back(station{ "Klavdiyeve" });
	stations.push_back(station{ "Kyiv" });
	stations.push_back(station{ "Odesa" });
	stations.push_back(station{ "Kharkiv" });
	stations.push_back(station{ "Zhytomyr" });
	stations.push_back(station{ "Lviv" });

	cout << "INPUT          OUTPUT\n\n";
	perfect_hash_table<station> hash(stations, compute_hash);
	for (size_t i = 0; i < stations.size(); i++) {
		cout << stations[i].get_name() << ".........." << hash.get(compute_hash(stations[i])).get_name() << '\n';
	}
}