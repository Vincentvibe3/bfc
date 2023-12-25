#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "hashmap.h"

typedef struct Node {
	struct Node* next;
	int val;
} Node;

void free_stack(Node* stack);

int scanLoops(char* program, int programLength, HashMap* jump_map);
int start_interpreter(char* program);

#endif /* INTERPRETER_H */