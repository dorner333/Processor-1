#include <stdio.h>
#include <stdlib.h>
#include "stack.cpp"

enum {END = 0, PUSH, PUSHR, POP, POPR, ADD, SUB, DIV, MUL, FSQRT, IN, PRINT, OUT} CMDS;

const int OPEN_ERR = 1;
const int CMD_ERROR = 2;

int file(int** cmd, char** file);
int proc(int* cmd, int SIZE);

int main(int argc, char* argv[]){

	int* cmd = NULL;

	int SIZE = file(&cmd, argv + 1);

	if(SIZE == OPEN_ERR){

		printf("Couldn't open file\n");
		return OPEN_ERR;
	}

	proc(cmd, SIZE);

	return 0;
}

int file(int** cmd, char** file){

	FILE* comands = fopen(*file, "r");

	if(comands == NULL){
		fclose(comands);
		return OPEN_ERR;
	}

	fseek(comands, 0, SEEK_END);
	int SIZE = ftell(comands);
	rewind(comands);

	char* char_cmd = (char*)calloc(SIZE, sizeof(char));
	*cmd = (int*)calloc(SIZE, sizeof(int));

	fread(char_cmd, sizeof(char), SIZE, comands);

	int j = 0, pnt = 0; 

	for(int i = 0; i < SIZE; i++){
		if(char_cmd[i] == '\n'){
			char_cmd[i] = '\0';
			(*cmd)[j++] = atoi(char_cmd + pnt);
			pnt = i + 1;
		}
	}

	fclose(comands);
	free(char_cmd);

	return SIZE;
}

int proc(int* cmd, int SIZE){
	
	Stack stack;
	constructor(&stack, SIZE);

	int registers[4] = { };

	int mode = -1;

	if(cmd[0] != PUSH && cmd[0] != PUSHR){
		printf("Bad input data\n");
		return CMD_ERROR;
	} else {
		mode = 0;
	}

	int tmp = 0;

	for(int i = 1; i < stack.capacity && cmd[i] != OUT; i++){
		
		switch(mode){
		case(0):	
			
			switch(cmd[i]){
			case(PUSH):
				mode = 1;
				break;
			case(PUSHR):
				mode = 2;
				break;
			case(POP):
				tmp = pop(&stack);
				break;
			case(POPR):
				mode = 3;
			case(ADD):				
				push(&stack, pop(&stack) + pop(&stack));
				break;
			case(SUB):			
				push(&stack, pop(&stack) - pop(&stack));
				break;
			case(DIV):
				push(&stack, pop(&stack) / pop(&stack));
				break;
			case(MUL):
				push(&stack, pop(&stack) * pop(&stack));
				break;
			case(IN):
				scanf("%d", &tmp);
				push(&stack, tmp);
				break;
			case(PRINT):
				tmp = pop(&stack);
				push(&stack, tmp);
				printf("TOP = %d\n", tmp);
				break;
			}

			break;

		case(1):
			push(&stack, cmd[i]);		
			mode = 0;
			break;
		case(2):
			push(&stack, registers[cmd[i]]);		
			mode = 0;
			break;
		case(3):
			registers[cmd[i]] = pop(&stack);
			break;
		}
	}

	stack_dump(&stack);

	destructor(&stack);
	free(cmd);

	return 0;
}