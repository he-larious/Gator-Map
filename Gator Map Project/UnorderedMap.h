#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using std::string;
using std::pair;

// My custom linked list class from COP3503 project 1 (modified)
template <typename T>
class LinkedList {
public:
	struct Node {
		Node* next = nullptr;
		Node* prev = nullptr;
		T key;
		T value;
	};

	// Accessors
	Node* Find(const T key);
	Node* Head();
	const Node* Head() const;
	Node* Tail();
	const Node* Tail() const;

	// Insertion
	void AddHead(const T key, const T value);
	void AddTail(const T key, const T value);

	// Removal
	bool Remove(T key);

	// Operators
	LinkedList<T>& operator=(const LinkedList<T>& rhs);

	// Constructors and Destructor
	LinkedList();
	LinkedList(const LinkedList<T>& list);
	~LinkedList();

private:
	Node* head = nullptr;
	Node* tail = nullptr;
	unsigned int nodeCount;
};

template <typename T>
void LinkedList<T>::AddHead(const T key, const T value) {
	Node* newNode = new Node;
	newNode->key = key;
	newNode->value = value;

	// If there are no nodes in the list...
	if (!head) {
		head = newNode;
		tail = newNode;
	}
	// If there is at least one node in the list...
	else {
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
	nodeCount++;    // Number of nodes increases by one after adding a new head.
}

template <typename T>
void LinkedList<T>::AddTail(const T key, const T value) {
	Node* newNode = new Node;
	newNode->key = key;
	newNode->value = value;
	if (!head) {
		head = newNode;
		tail = newNode;
	}
	else {
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
	nodeCount++;
}

template <typename T>
LinkedList<T>::LinkedList() {
	nodeCount = 0;
	head = nullptr;
	tail = nullptr;
}

template <typename T>
LinkedList<T>::~LinkedList() {
	Node* currentNode = head;
	Node* temp = currentNode;

	// Traverse the list and delete each node one by one.
	while (currentNode) {
		temp = currentNode->next;
		delete currentNode;
		currentNode = temp;
	}
	/* Error with copy assignment operator, fixed by reassigning head
	 * and tail pointers to null after the list was deleted.
	 */
	head = nullptr;
	tail = nullptr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
	return head;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const {
	return head;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
	return tail;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const {
	return tail;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T key) {
	Node* currentNode = head;
	while (currentNode) {
		if (currentNode->key == key) {
			return currentNode;
		}
		currentNode = currentNode->next;
	}
	return nullptr;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
	// Delete old list.
	Node* currentNode = head;
	Node* temp = currentNode;
	while (currentNode) {
		temp = currentNode->next;
		delete currentNode;
		currentNode = temp;
	}
	head = nullptr;
	tail = nullptr;
	Node* copy = list.head;
	while (copy) {
		// Allocate space for new list and copy data.
		AddTail(copy->key, copy->value);
		copy = copy->next;
	}
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
	Node* currentNode = head;
	Node* temp = currentNode;
	while (currentNode) {
		temp = currentNode->next;
		delete currentNode;
		currentNode = temp;
	}
	head = nullptr;
	tail = nullptr;
	Node* copy = rhs.head;
	while (copy) {
		AddTail(copy->key, copy->value);
		copy = copy->next;
	}
	return *this;
}

template <typename T>
bool LinkedList<T>::Remove(T key) {
	Node* currentNode = head;

	// Edge case
	if (nodeCount == 1) {
		delete head;
		head = nullptr;
		tail = nullptr;
		nodeCount--;
		return true;
	}

	while (currentNode && currentNode->next && currentNode->prev) {
		// If data at current node matches passed in data...
		if (currentNode->key == key) {
			// Store the current node into a temporary new node.
			Node* remove = currentNode;

			// Set the next and previous pointers accordingly.
			currentNode->prev->next = currentNode->next;
			currentNode->next->prev = currentNode->prev;

			// Iterate to next element in the list.
			currentNode = currentNode->next;

			// Delete the current node.
			delete remove;

			// Decrement node count.
			nodeCount--;
			return true;
		}
		else {
			currentNode = currentNode->next;
		}
	}
	return false;
}

unsigned int hashFunction(char const* key, int table_size) {
	unsigned int hashCode = 0;
	unsigned int temp;
	for (int i = 0; key[i] != '\0'; i++) {
		unsigned int ascii = int(key[i]);
		if (i % 2 == 0) {
			temp = (hashCode << 7) ^ ascii ^ (hashCode >> 3);
		}
		else {
			temp = (hashCode << 11) ^ ascii ^ (hashCode >> 5);
			temp = ~temp;
		}
		hashCode = hashCode ^ temp;
	}
	hashCode = hashCode & 0x7FFFFFFF;
	return (hashCode % table_size);
}

class UnorderedMap {
private:
	// Use separate chaining approach with resizable array of linked lists
	LinkedList<string>* map = nullptr;
	unsigned int buckets;
	double maxLoad;
	unsigned int elements;

public:
	class Iterator;
	UnorderedMap(unsigned int bucketCount, double loadFactor);
	~UnorderedMap();
	Iterator begin() const;
	Iterator end() const;
	string& operator[] (string const& key);
	void rehash();
	void remove(string const& key);
	unsigned int size();
	double loadFactor();

	class Iterator {
	private:
		const LinkedList<string>::Node* nodePtr = nullptr;
		const UnorderedMap* mapPtr = nullptr;

	public:
		Iterator(const LinkedList<string>::Node* p1, const UnorderedMap* p2);
		Iterator& operator=(Iterator const& rhs);
		Iterator& operator++();
		bool operator!=(Iterator const& rhs);
		bool operator==(Iterator const& rhs);
		pair<string, string> operator*() const;
		friend class UnorderedMap;
	};
};

UnorderedMap::UnorderedMap(unsigned int bucketCount, double loadFactor) {
	buckets = bucketCount;
	maxLoad = loadFactor;
	elements = 0;
	map = new LinkedList<string>[buckets];
}

UnorderedMap::~UnorderedMap() {
	delete[] map;
}

UnorderedMap::Iterator UnorderedMap::begin() const {
	for (unsigned int i = 0; i < buckets; i++) {
		// Check for first non empty linked list in array
		if (map[i].Head()) {
			return Iterator(map[i].Head(), this);
		}
	}
	// If all linked lists in array are empty, return same as end
	return end();
}

UnorderedMap::Iterator UnorderedMap::end() const {
	return Iterator(nullptr, this);
}

string& UnorderedMap::operator[] (string const& key) {
	const char* convertedKey = key.c_str();
	unsigned int index = hashFunction(convertedKey, buckets);

	// If key doesn't exist, construct the value and place in map
	if (!map[index].Find(key)) {
		map[index].AddHead(key, "");
		elements++;
		rehash();
		// Update index after rehash since bucket count changes
		index = hashFunction(convertedKey, buckets);
	}

	return map[index].Find(key)->value;
}

void UnorderedMap::rehash() {
	if (loadFactor() >= maxLoad) {
		buckets *= 2;
		LinkedList<string>* temp = new LinkedList<string>[buckets];
		for (unsigned int i = 0; i < buckets / 2; i++) {
			LinkedList<string>::Node* curr = map[i].Head();
			while (curr) {
				const char* convertedkey = curr->key.c_str();
				unsigned int index = hashFunction(convertedkey, buckets);
				temp[index].AddHead(curr->key, curr->value);
				curr = curr->next;
			}
		}
		delete[] map;
		map = temp;
	}
}

void UnorderedMap::remove(string const& key) {
	const char* convertedKey = key.c_str();
	unsigned int index = hashFunction(convertedKey, buckets);
	if (map[index].Remove(key)) {
		elements--;
	}
}

unsigned int UnorderedMap::size() {
	return elements;
}

double UnorderedMap::loadFactor() {
	return ((double)elements / buckets);
}

UnorderedMap::Iterator::Iterator(const LinkedList<string>::Node* p1, const UnorderedMap* p2) {
	nodePtr = p1;
	mapPtr = p2;
}

UnorderedMap::Iterator& UnorderedMap::Iterator::operator=(Iterator const& rhs) {
	nodePtr = rhs.nodePtr;
	return *this;
}

UnorderedMap::Iterator& UnorderedMap::Iterator::operator++() {
	const char* convertedkey = nodePtr->key.c_str();
	unsigned int nextIndex = hashFunction(convertedkey, mapPtr->buckets) + 1;
	for (unsigned int i = nextIndex; i < mapPtr->buckets; i++) {
		if (nodePtr && nodePtr->next) {
			nodePtr = nodePtr->next;
			return *this;
		}
		else if (nodePtr && !nodePtr->next) {
			if (mapPtr->map[i].Head()) {
				nodePtr = mapPtr->map[i].Head();
				return *this;
			}
		}
	}
	nodePtr = nullptr;
	return *this;
}

bool UnorderedMap::Iterator::operator!=(Iterator const& rhs) {
	return (nodePtr != rhs.nodePtr);
}

bool UnorderedMap::Iterator::operator==(Iterator const& rhs) {
	return (nodePtr == rhs.nodePtr);
}

pair<string, string> UnorderedMap::Iterator::operator*() const {
	return make_pair(nodePtr->key, nodePtr->value);
}