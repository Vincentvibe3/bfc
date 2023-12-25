#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

const char VALID_BF_TOKENS[8] = {'>', '<', '+', '-', '.', ',', '[', ']'};

HashMap* setup_bf_tokens(){
	HashMap* tokenMap = create_hashmap();
	for (int i=0;i<8;i++){
		int addResult = hashmap_add(tokenMap, VALID_BF_TOKENS[i], 0);
		if (!addResult){
			free_map(tokenMap);
			return NULL;
		}
	}
	return tokenMap;
}

char* load_program(char* path){
	HashMap* tokenMap = setup_bf_tokens();
	FILE *fptr = fopen(path, "r");
	if (fptr == NULL){
		fprintf(stderr, "Could not open file %s\n", path);
		return NULL;
	}
	int read_char = 0;
	int buffer_size = 1024;
	char* buffer = malloc(buffer_size*sizeof(char));
	if (buffer == NULL){
		perror("Could not allocate buffer to read file\n");
		return NULL;
	}
	while (!feof(fptr)) {
		char c_input = fgetc(fptr);
		if (read_char == buffer_size){
			buffer_size*=2;
			buffer = realloc(buffer, buffer_size);
			if (buffer == NULL){
				perror("Could not allocate buffer to read file\n");
				return NULL;
			}
		}
		if (c_input<0){
			continue;
		}
		HashMapGetResult queryResult = hashmap_get(tokenMap, c_input);
		if (queryResult.found){
			buffer[read_char] = c_input;
			read_char++;
		}
	}
	fclose(fptr);
	return buffer;
}