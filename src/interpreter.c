#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "interpreter.h"

void free_stack(Node* stack){
	Node* next = stack->next;
	Node* current = stack;
	while (next){
		free(current);
		current = next;
		next = current->next;
	}
}

int scanLoops(char* program, int programLength, HashMap* jump_map){
	Node* stack = NULL;
	for (int index = 0; index < programLength; index++){
		char character = program[index];
		if (character == '['){
			Node* newNode = malloc(sizeof(Node));
			if (newNode==NULL){
				perror("Failed to allocate Node for stack while scanning loops\n");
				free_stack(stack);
				return 0;
			}
			newNode->next = stack;
			newNode->val = index;
			stack = newNode;
		} else if (character == ']'){
			Node* toRemove = stack;
			int openingBracketPos = toRemove -> val;
			// Save locations in HashMap
			int forwardResult = hashmap_add(jump_map, openingBracketPos, index);
			int backwardResult = hashmap_add(jump_map, index, openingBracketPos);
			if (!forwardResult||!backwardResult){
				perror("Failed to add to hash map while scanning loops\n");
				free_stack(stack);
				return 0;
			}
			// Pop the stack
			Node* newHead = toRemove->next;
			stack = newHead;
			free(toRemove);
		}
	}
	free(stack);
	return 1;
}

int start_interpreter(char* program){
	int dataLength = 30000;
	unsigned char* data = (char*) calloc(dataLength, sizeof(char));
	int programLength = strlen(program);
	int progress = 0;
	int dataPointer = 0;
	HashMap* jump_map =  create_hashmap();
	if (jump_map==NULL){
		perror("Failed to allocate hash map for loop jumps\n");
		return 1;
	}
	int scanResult = scanLoops(program, programLength, jump_map);
	if (!scanResult){
		free_map(jump_map);
		return 1;
	}
	while (progress<programLength) {
		char instruction = program[progress];
		int change = 1;
		switch (instruction) {
			case '>':
				if (dataPointer == dataLength - 1){
					dataPointer = 0;
				} else {
					dataPointer++;
				}
				progress++;
				break;
			case '<':
				if (dataPointer == 0){
					dataPointer = dataLength - 1;
				} else {
					dataPointer--;
				}
				progress++;
				break;
			case '+':
				if (data[dataPointer] == 255){
					data[dataPointer] = 0;
				} else {
					data[dataPointer]++;
				}
				progress++;
				break;
			case '-':
				if (data[dataPointer] == 0){
					data[dataPointer] = 255;
				} else {
					data[dataPointer]--;
				}
				progress++;
				break;
			case '.':
				printf("%c", data[dataPointer]);
				progress++;
				break;
			case ',':
				char input;
				scanf("%c", &input);
				data[dataPointer] = input;
				progress++;
				break;
			case '[':
				if (data[dataPointer]==0){
					HashMapGetResult jumpResult = hashmap_get(jump_map, progress);
					if (jumpResult.found){
						progress = jumpResult.value + 1;
					} else {
						perror("An error occured in loop execution");
						return 1;
					}
				} else {
					progress++;
				}
				break;
			case ']':
				if (data[dataPointer]!=0){
					HashMapGetResult jumpResult = hashmap_get(jump_map, progress);
					if (jumpResult.found){
						progress = jumpResult.value + 1;
					}else {
						perror("An error occured in loop execution");
						return 1;
					}
				} else {
					progress++;
				}
				break;
			
			default:
				progress++;
				break;
		}
	}
	printf("\n");
	free(data);
	free_map(jump_map);
	return 0;
}