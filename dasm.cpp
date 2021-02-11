#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int const OPEN_ERR = 1;
int const CMD_ERROR = 2;

enum {end = '0', PUSH, ADD, SUB, DIV, MUL, FSQRT, IN, /* hlt,*/ PRINT, OUT} CMDS;

int read(char** cmd);
int DASM(char* cmd, int SIZE);

int main(){

	char* cmd = NULL;

	int SIZE = read(&cmd);
	if(SIZE == OPEN_ERR){
		printf("Couldn't open file\n");
		return OPEN_ERR;
	}

	DASM(cmd, SIZE);

	return 0;
}

int read(char** cmd){

	FILE* file1 = fopen("TEST.txt", "r+");

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

int DASM(char* cmd, int SIZE){

	FILE* file2 = fopen("DASM.txt", "w");

	char* char_cmd = (char*)calloc(SIZE, sizeof(char));

	int mode = -1, j = 0;

	if(cmd[0] != PUSH){
		printf("Bad input data\n");
		return CMD_ERROR;
	} else {
		sscanf("push", "%s", char_cmd);
		j += strlen(char_cmd + j);
		char_cmd[j++] = ' ';
		mode = 0;
	}

	for(int i = 1; i < SIZE; i++){
		
		if(cmd[i] != '\0'){
			continue;
		}

		if(mode == 1){
			switch(cmd[i + 1]){
				case(PUSH):
					sscanf("push", "%s", char_cmd + j);
					j += strlen(char_cmd + j);
					char_cmd[j++] = ' ';
					mode = 0;
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

		} else {		
			mode = 1;
			sscanf(cmd + i + 1, "%s", char_cmd + j);
			j += strlen(char_cmd + j);
			char_cmd[j++] = '\n';
		}
	}

	fwrite(char_cmd, sizeof(char), j, file2);

	fclose(file2);
	free(cmd);
	free(char_cmd);
	return 0;
}