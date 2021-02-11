#include <stdio.h>
#include <stdlib.h>
#include "stack.cpp"
		//	 0	   1	2	 3 	  4    5     6                   7     8
enum {end = 0, PUSH, ADD, SUB, DIV, MUL, FSQRT, IN, /* hlt,*/ PRINT, OUT} CMDS;

const int OPEN_ERR = 1;
const int CMD_ERROR = 2;

int file(int** cmd);
int proc(int* cmd, int SIZE);

int main(){

	int* cmd;

	int SIZE = file(&cmd);

	if(SIZE == OPEN_ERR){

		printf("Couldn't open file\n");
		return OPEN_ERR;
	}

	proc(cmd, SIZE);

	return 0;
}

int file(int** cmd){

	FILE* comands = fopen("TEST.txt", "r");

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

	int mode = -1;

	if(cmd[0] != PUSH){
		printf("Bad input data\n");
		return CMD_ERROR;
	} else {
		mode = 0;   							//mode == 1 ==> waiting for a digit, else waiting for a command
	}

	int tmp1 = 0, tmp2 = 0;

	for(int i = 1; i < stack.capacity && cmd[i] != OUT; i++){
		
		if(mode == 1){

			switch(cmd[i]){
				case(PUSH):
					mode = 0;
					break;
				case(ADD):				
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, tmp2 + tmp1);
					break;
				case(SUB):			
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, tmp2 - tmp1);
					break;
				case(DIV):
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, (tmp2)/(tmp1));
					break;
				case(MUL):
					tmp1 = pop(&stack), tmp2 = pop(&stack);
					push(&stack, (tmp2) * (tmp1));
					break;
				case(IN):
					scanf("%d", &tmp1);
					push(&stack, tmp1);
					break;
				case(PRINT):
					char top = pop(&stack);
					push(&stack, top);
					printf("TOP = %d\n", top);
					break;
				}

		} else {

			push(&stack, cmd[i]);		
			mode = 1;
		}
	}

	destructor(&stack);
	free(cmd);

	return 0;
}