#include <iostream>
#include <string>
#include "lru_cache_map.hpp"

using namespace std;

int main() {
	lru_cache_map<int, string> cache(3);

	cache.insert(1, "Tst");
	cache.insert(2, "Hello Hello");
	cache.insert(5, "Hello");

	cache.mention(1);

	cache.insert(56, "World!");

	cout << cache.getLeastUsed().second << endl;
	cout << cache.getMostUsed().second << endl;

	cache.resize(2);

	cout << cache.getLeastUsed().second << endl;
	cout << cache.getMostUsed().second << endl;

	return 0;
}