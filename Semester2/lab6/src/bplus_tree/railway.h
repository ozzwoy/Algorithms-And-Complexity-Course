#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

//Клас, що визначає залізничну станцію. Містить єдине поле: ім'я станції.
class station {
private:
	string name;
	
public:
	station() : name("default name") {}

	station(string const &_name) {
		if (_name.empty()) {
			name = "default name";
		}
		else {
			name = _name;
		}
	}

	string get_name() const {
		return name;
	}

	void set_name(string const &_name) {
		if (!_name.empty()) {
			name = _name;
		}
	}

	bool operator==(station const &to_compare) const {
		return this->name == to_compare.get_name();
	}

	bool operator<(station const &to_compare) const {
		return this->name < to_compare.get_name();
	}

	bool operator>(station const &to_compare) const {
		return this->name > to_compare.get_name();
	}
};

//Клас, що визначає залізничну дорогу. Містить впорядкований список станцій. Список мусить мати як мінімум дві станції.
class road {
private:
	string name;
	vector<station> stations;

public:
	road() {
		stations.push_back(station{});
		stations.push_back(station{});
		name = stations[0].get_name() + "--" + stations[1].get_name();
	}

	road(vector<station> const &_stations) {
		if (_stations.empty()) {
			stations.push_back(station{});
			stations.push_back(station{});
		}
		else if (_stations.size() == 1) {
			stations.push_back(station{});
		}
		else {
			stations = _stations;
		}
		name = stations[0].get_name() + "--" + stations.back().get_name();
	}

	string get_name() const {
		return name;
	}

	void set_name(string const &_name) {
		if (!_name.empty()) {
			name = _name;
		}
	}

	size_t num_of_stations() const {
		return stations.size();
	}

	station get_station(size_t index) const {
		return stations[index];
	}

	//Вставити станцію в список перед деякою станцією
	bool insert_station(station const &new_station, station const &next_station) {
		if (!new_station.get_name().empty()) {
			if (next_station.get_name().empty()) {
				stations.push_back(new_station);
				return true;
			}
			else {
				auto iter = std::find(stations.begin(), stations.end(), next_station);
				if (iter != stations.end()) {
					stations.insert(iter, new_station);
					return true;
				}
			}
		}
		return false;
	}

	//Видалити зазначену станцію зі списку
	bool remove_station(station const &to_remove) {
		if (!to_remove.get_name().empty()) {
			auto iter = std::find(stations.begin(), stations.end(), to_remove);
			if (iter != stations.end()) {
				stations.erase(iter);
				return true;
			}
		}
		return false;
	}

	bool operator==(road const &to_compare) const {
		return this->name == to_compare.get_name();
	}

	bool operator<(road const &to_compare) const {
		return this->name < to_compare.get_name();
	}

	bool operator>(road const &to_compare) const {
		return this->name > to_compare.get_name();
	}
};

//Клас, що визначає залізницю. Містить список залізничних доріг.
class railway {
private:
	string name;
	vector<road> roads;

public:
	railway() {
		name = "default name";
	}

	railway(string const &_name) {
		if (_name.empty()) {
			name = "default name";
		}
		else {
			name = _name;
		}
	}

	railway(vector<road> const &_roads, string const &_name) : roads(_roads) {
		if (_name.empty()) {
			name = "default name";
		}
		else {
			name = _name;
		}
	}

	string get_name() const {
		return name;
	}

	void set_name(string const &_name) {
		if (!_name.empty()) {
			name = _name;
		}
	}
	
	size_t num_of_roads() const {
		return roads.size();
	}

	road get_road(size_t index) const {
		return roads[index];
	}

	//Додати дорогу до списку
	bool add_road(road const &road) {
		if (road.num_of_stations() != 0 && road.num_of_stations() != 1) {
			roads.push_back(road);
		}
		return false;
	}

	//Видалити зазначену дорогу зі списку
	bool remove_road(road const &to_remove) {
		if (to_remove.num_of_stations() != 0 && to_remove.num_of_stations() != 1) {
			auto iter = std::find(roads.begin(), roads.end(), to_remove);
			if (iter != roads.end()) {
				roads.erase(iter);
				return true;
			}
		}
		return false;
	}

	bool operator==(railway const &to_compare) const {
		return this->name == to_compare.get_name();
	}

	bool operator<(railway const &to_compare) const {
		return this->name < to_compare.get_name();
	}

	bool operator>(railway const &to_compare) const {
		return this->name > to_compare.get_name();
	}
};

std::ostream& operator<<(std::ostream &os, railway const &this_railway) {
	os << this_railway.get_name();
	return os;
}