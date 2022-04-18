#include "OrderedMap.h"
#include "UnorderedMap.h"
#include "Random.h"
#include <iostream>
#include <ctime>
#include <chrono>
using std::cout;
using std::endl;
using std::to_string;
using namespace std::chrono;

void orderedInsert(int n);
void unorderedInsert(int n);
void orderedSearch(int n);
void unorderedSearch(int n);
void orderedTraverse(int n);
void unorderedTraverse(int n);
void unorderedRemove(int n);

int main() {
	// Testing ordered map insertions
	orderedInsert(1000);
	orderedInsert(10000);
	orderedInsert(100000);

	// Testing unordered map insertions
	unorderedInsert(1000);
	unorderedInsert(10000);
	unorderedInsert(100000);

	// Testing ordered map searches
	orderedSearch(1000);
	orderedSearch(10000);
	orderedSearch(100000);

	// Testing unordered map searches
	unorderedSearch(1000);
	unorderedSearch(10000);
	unorderedSearch(100000);

	// Testing ordered map traversal
	orderedTraverse(1000);
	orderedTraverse(10000);
	orderedTraverse(100000);

	// Testing unordered map traversal
	unorderedTraverse(1000);
	unorderedTraverse(10000);
	unorderedTraverse(100000);

	// Not part of the 3 operation tests - used to see if unordered map remove was O(1) for average case.
	unorderedRemove(1000);
	unorderedRemove(10000);
	unorderedRemove(100000);

	return 0;
}

void orderedInsert(int n) {
	OrderedMap map;

	// Start clock
	auto t1 = high_resolution_clock::now();

	// Insert n randomly generated keys
	for (int i = 0; i < n; i++) {
		map.insert(to_string(Random::RandomInt(0, 99999999)), "test");
	}

	// Stop clock
	auto t2 = high_resolution_clock::now();

	// Convert to seconds
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	// Print findings
	cout << "Time for " << n << " inserts in ordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}

void unorderedInsert(int n) {
	UnorderedMap map(100, 0.80);

	auto t1 = high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		map[to_string(Random::RandomInt(0, 99999999))] = "test";
	}
	auto t2 = high_resolution_clock::now();
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	cout << "Time for " << n << " inserts in unordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}

void orderedSearch(int n) {
	OrderedMap map;

	// Insert n randomly generated keys
	for (int i = 0; i < n; i++) {
		map.insert(to_string(Random::RandomInt(0, 99999999)), "test");
	}

	// Start clock
	auto t1 = high_resolution_clock::now();

	// Randomly generate n keys and search for the key in the map
	for (int i = 0; i < n; i++) {
		map.search(to_string(Random::RandomInt(0, 99999999)));
	}

	// Stop clock
	auto t2 = high_resolution_clock::now();

	// Convert to seconds
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	cout << "Time for " << n << " searches in ordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}

void unorderedSearch(int n) {
	UnorderedMap map(100, 0.80);

	for (int i = 0; i < n; i++) {
		map[to_string(Random::RandomInt(0, 99999999))] = "test";
	}

	auto t1 = high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		map[to_string(Random::RandomInt(0, 99999999))];
	}
	auto t2 = high_resolution_clock::now();
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	cout << "Time for " << n << " searches in unordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}

void orderedTraverse(int n) {
	OrderedMap map;

	for (int i = 0; i < n; i++) {
		map.insert(to_string(Random::RandomInt(0, 99999999)), "test");
	}

	auto t1 = high_resolution_clock::now();
	map.traverse();
	auto t2 = high_resolution_clock::now();
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	cout << "Time for " << n << " operations traversal in ordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}

void unorderedTraverse(int n) {
	UnorderedMap map(100, 0.80);

	for (int i = 0; i < n; i++) {
		map[to_string(Random::RandomInt(0, 99999999))] = "test";
	}

	auto t1 = high_resolution_clock::now();
	for (auto iter = map.begin(); iter != map.end(); ++iter) {
		cout << (*iter).first << " " << (*iter).second << endl;
	}
	auto t2 = high_resolution_clock::now();
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	cout << "Time for " << n << " operations traversal in unordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}

void unorderedRemove(int n) {
	UnorderedMap map(100, 0.80);

	for (int i = 0; i < n; i++) {
		map[to_string(n)] = "test";
	}

	auto t1 = high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		map.remove(to_string(n));
	}
	auto t2 = high_resolution_clock::now();
	auto exeTime = duration_cast<duration<double>>(t2 - t1);

	cout << "Time for " << n << " removes in unordered map: " << exeTime.count() << " seconds" << endl;
	cout << "Size of map: " << map.size() << endl;
}