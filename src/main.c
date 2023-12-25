#include <stdio.h>
#include <stdlib.h>
#include "file_input.h"
#include "interpreter.h"

int main(int argc, char* argv[]){
	if (argc < 2){
		fprintf(stderr, "You must pass the path to a brainfuck program.\n");
		return 1;
	} else {
		char* program_path = argv[1];
		char* program = load_program(program_path);
		if (program==NULL){
			return 1;
		}
		return start_interpreter(program);
	}
}