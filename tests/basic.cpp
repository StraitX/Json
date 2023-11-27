#include <json/json.hpp>
#include <core/hash_table.hpp>
#include <core/list.hpp>
#include <core/string.hpp>
#include <core/print.hpp>


template<typename T>
void EnsureConsistency(const T &object) {
	T clone;
	try {

		Println("[Info] Running ensure consistenfy for type '%'", typeid(T).name());
		Println("[Info] ToJson");
		Json json = object;
		Println("[Info] FromJson: \n%", json.dump(4));
		clone = T(json);
	}catch (const std::exception& exception) {
		return Println("[Fail] Ensure consistency failed because of exception '%': %", typeid(exception).name(), exception.what());
	}

	if (object != clone) {
		Println("[Fail] Ensure consistency failed for type '%'", typeid(T).name());
	}
}

int main() {
	
	int num = 1;
	
	String str = "Hello From Here";
	std::string std_str = str;

	List<int> list = {1, 2, 3, 4};
	std::vector<int> std_list = {1, 2, 3, 4};

	HashTable<std::string, int> table_str = {
		{"1", 2},
		{"2", 4},
		{"3", 9},
	};

	std::unordered_map<std::string, int> std_table_str = table_str;

	HashTable<int, int> table_int = {
		{1, 2},
		{2, 4},
		{3, 9},
	};

	std::unordered_map<int, int> std_table_int = table_int;

	EnsureConsistency(num);

	EnsureConsistency(str);
	EnsureConsistency(std_str);

	EnsureConsistency(list);
	EnsureConsistency(std_list);

	EnsureConsistency(table_str);
	EnsureConsistency(std_table_str);

	EnsureConsistency(table_int);
	EnsureConsistency(std_table_int);
}