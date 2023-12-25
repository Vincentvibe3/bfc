#include <stdlib.h>
#include "hashmap.h"

int hash(int key, int size) {
	if (key < 0){
		return -key % size;
	}
	return key % size;
}

HashMap* create_hashmap(){
	HashMap* new_hashmap = malloc(sizeof(HashMap));
	if (new_hashmap==NULL){
		return NULL;
	}
	HashMapCell** new_backing_array = (HashMapCell**) calloc(20, sizeof(HashMapCell*));
	if (new_backing_array==NULL){
		free(new_hashmap);
		return NULL;
	}
	new_hashmap->backing_array = new_backing_array;
	new_hashmap->maxSize = 20;
	new_hashmap->size = 0;
	return new_hashmap;
}

int hashmap_add(HashMap* hashmap, int key, int value){
	if (compute_load_factor(hashmap)>0.5){
		int resizeSuccess = resize(hashmap);
		if (!resizeSuccess){
			return 0;
		}
	}
	int hash_index = hash(key, hashmap->maxSize);
	HashMapCell** backing_array = hashmap->backing_array;
	while (backing_array[hash_index] != 0 && backing_array[hash_index]->key != key){
		if (hash_index == (hashmap->maxSize)-1){
			hash_index = 0;
		} else {
			hash_index++;
		}
	}
	if (backing_array[hash_index] != 0 && backing_array[hash_index]->key != key){
		backing_array[hash_index]->value = value;
	} else {
		HashMapCell* cell = malloc(sizeof(HashMapCell));
		if (cell == NULL){
			return 0;
		}
		cell->key = key;
		cell->value = value;
		backing_array[hash_index] = cell;
		(*hashmap).size++;
	}
	return 1;
}

HashMapGetResult hashmap_get(HashMap* hashmap, int key){
	int hash_index = hash(key, hashmap->maxSize);
	if (hash_index<0){
		return (HashMapGetResult){-1,0};
	}
	HashMapCell** backing_array = hashmap->backing_array;
	int wrappedAround = 0;
	while (backing_array[hash_index]==NULL || backing_array[hash_index]->key != key){
		if (hash_index == (hashmap->maxSize)-1){
			if (wrappedAround){
				return (HashMapGetResult){-1,0};
			}
			hash_index = 0;
			wrappedAround = 1;
		} else {
			hash_index++;
		}
	}
	return (HashMapGetResult){backing_array[hash_index]->value, 1};
}

float compute_load_factor(HashMap* hashmap){
	return hashmap->size / hashmap->maxSize;
}

void free_map(HashMap* hashmap){
	HashMapCell** backing_array = hashmap->backing_array;
	for (int i = 0; i> hashmap->maxSize; i++){
		free(backing_array[i]);
	}
	free(backing_array);
	free(hashmap);
}

int resize(HashMap* hashmap){
	int newSize = hashmap->maxSize*2;
	HashMapCell** new_backing_array = (HashMapCell**) calloc(newSize, sizeof(HashMapCell*));
	if (new_backing_array == NULL){
		return 0;
	}
	for (int i=0; i<hashmap->maxSize; i++){
		HashMapCell* cell = (hashmap->backing_array)[i];
		if (cell!=0){
			int key = (*cell).key;
			int newhash = hash(key, newSize);
			while (new_backing_array[newhash] != 0){
				if (newhash == newSize-1){
					newhash = 0;
				} else {
					newhash++;
				}
			}
			new_backing_array[newhash] = cell;
		}
	}
	free(hashmap->backing_array);
	hashmap->backing_array = new_backing_array;
	hashmap->maxSize = newSize;
	return 1;
}
