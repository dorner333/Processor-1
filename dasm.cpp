#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int const OPEN_ERR = 1;
int const CMD_ERROR = 2;

enum {end = '0', PUSH, PUSHR, POP, POPR, ADD, SUB, DIV, MUL, FSQRT, IN, PRINT, OUT} CMDS;
enum {RAX = '0', RBX, RCX, RDX} REGS;

int read(char** cmd, char** bin_f);
int dasm(char* cmd, int SIZE);

int main(int argc, char* argv[]){

	char* cmd = NULL;

	int SIZE = read(&cmd, argv + 1);
	if(SIZE == OPEN_ERR){
		printf("Couldn't open file\n");
		return OPEN_ERR;
	}

	dasm(cmd, SIZE);

	return 0;
}

int read(char** cmd, char** bin_f){

	FILE* file1 = fopen(*bin_f, "r+");

	if(file1 == NULL){
		return OPEN_ERR;
	}

	fseek(file1, 0, SEEK_END);
	int SIZE = 3*ftell(file1);
	rewind(file1);
	
	*cmd = (char*)calloc(SIZE, sizeof(char));
	fread(*cmd, sizeof(char), SIZE, file1);

	for(int i = 0; i < SIZE; i++){
		if((*cmd)[i] == ' ' || (*cmd)[i] == '\n'){
			(*cmd)[i] = '\0';
		}
	}

	fclose(file1);

	return SIZE;
}

int dasm(char* cmd, int SIZE){

	FILE* file2 = fopen("dcmd.asm", "w");

	char* char_cmd = (char*)calloc(SIZE, sizeof(char));

	int mode = -1, j = 0;

	if(cmd[0] == PUSH){
		sscanf("push", "%s", char_cmd + j);
		j += strlen(char_cmd + j);
		char_cmd[j++] = ' ';		
		mode = 1;
	} else if(cmd[0] == PUSHR){
		sscanf("pushr", "%s", char_cmd + j);
		j += strlen(char_cmd + j);
		char_cmd[j++] = ' ';
		mode = 2;
	} else {
		printf("Wrong command\n");
		return CMD_ERROR;
	}

	for(int i = 1; i < SIZE; i++){
		
		if(cmd[i] != '\0'){
			continue;
		}

		switch(mode){
		case(0):

			switch(cmd[i + 1]){
			case(PUSH):
				sscanf("push", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = ' ';
				mode = 1;
				break;
			case(PUSHR):
				sscanf("pushr", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = ' ';
				mode = 2;
				break;
			case(POP):
				sscanf("pop", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(POPR):
				sscanf("popr", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = ' ';
				mode = 2;
				break;
			case(ADD):				
				sscanf("add", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(SUB):			
				sscanf("sub", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(DIV):
				sscanf("div", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(MUL):
				sscanf("mul", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(IN):
				sscanf("in", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(PRINT):
				sscanf("print", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			case(OUT):
				sscanf("out", "%s", char_cmd + j);
				j += strlen(char_cmd + j);
				char_cmd[j++] = '\n';
				break;
			}
			break;

		case(1):	
			mode = 0;
			sscanf(cmd + i + 1, "%s", char_cmd + j);		
			j += strlen(char_cmd + j);
			char_cmd[j++] = '\n';
			break;
		case(2):

			switch(cmd[i + 1]){
			case(RAX):
				sscanf("rax", "%s", char_cmd + j);
				break;
			case(RBX):
				sscanf("rbx", "%s", char_cmd + j);
				break;
			case(RCX):
				sscanf("rcx", "%s", char_cmd + j);
				break;
			case(RDX):
				sscanf("rdx", "%s", char_cmd + j);
				break;
			}

			mode = 0;
			j += strlen(char_cmd + j);
			char_cmd[j++] = '\n';
			break;	
		}
	}

	fwrite(char_cmd, sizeof(char), j, file2);

	fclose(file2);
	free(cmd);
	free(char_cmd);

	return 0;
}