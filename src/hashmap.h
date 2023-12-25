#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMapCell {
	int key;
	int value;
} HashMapCell;

typedef struct HashMap {
	HashMapCell** backing_array;
	int maxSize;
	int size;
} HashMap;

typedef struct HashMapGetResult {
	int value;
	int found;
} HashMapGetResult;

HashMap* create_hashmap();

int hash(int key, int size);

int hashmap_add(HashMap* hashmap, int key, int value);

HashMapGetResult hashmap_get(HashMap* hashmap, int key);

void free_map(HashMap* hashmap);

float compute_load_factor(HashMap* hashmap);

int resize(HashMap* hashmap);

#endif /* HASHMAP_H */