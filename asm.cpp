#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int OPEN_ERR = 1;

enum {end = '0', push, add, sub, DIV, mul, fsqrt, in, hlt, out} CMDS;

int open(char** symbols);
void read(char* symbols, int SIZE);

int main(){

	char* symbols = NULL;
	
	int SIZE = open(&symbols);
	
	if(SIZE == OPEN_ERR){
		printf("OPEN_ERR\n");
		return OPEN_ERR;
	}

	read(symbols, SIZE);

	return 0;
}

int open(char** symbols){

	FILE* file1 = fopen("TEST_ASM.txt", "r+");

	if(file1 == NULL){
		return OPEN_ERR;
	}

	fseek(file1, 0, SEEK_END);
	int SIZE = ftell(file1);
	rewind(file1);
	
	*symbols = (char*)calloc(SIZE, sizeof(char));
	fread(*symbols, sizeof(char), SIZE, file1);

	for(int i = 0; i < SIZE; i++){
		if((*symbols)[i] == ' ' || (*symbols)[i] == '\n'){
			(*symbols)[i] = '\0';
		}
	}

	fclose(file1);

	return SIZE;
}

void read(char* symbols, int SIZE){

	FILE* file2 = fopen("TEST.txt", "w");

	char* code = (char*)calloc(SIZE, sizeof(char));

	int j = 0, pnt = 0;

	for(int i = 0; i < SIZE; i++){		
		if(symbols[i] == '\0'){
			if(!strcmp(symbols + pnt, "push")){
				code[j++] = push;
				code[j++] = '\n';
			} else if(!strcmp(symbols + pnt, "add")){
				code[j++] = add;
				code[j++] = '\n';
			} else if(!strcmp(symbols + pnt, "sub")){
				code[j++] = sub;
				code[j++] = '\n';
			} else if(!strcmp(symbols + pnt, "div")){
				code[j++] = DIV;
				code[j++] = '\n';
			} else if(!strcmp(symbols + pnt, "mul")){
				code[j++] = mul;
				code[j++] = '\n';
			} else if(!strcmp(symbols + pnt, "fsqrt")){
				code[j++] = fsqrt;
				code[j++] = '\n';
			} else if(!strcmp(symbols + pnt, "out")){
				code[j++] = out;
				code[j++] = '\n';
			} else {
				//printf("%s\n", symbols + pnt);
				//sscanf(symbols + pnt, "%d", &x);
				sprintf(code + j, "%s", symbols + pnt);
				j++;
				code[j++] = '\n';
			}
			pnt = i + 1;
		}
	}

	fwrite(code, sizeof(char), j, file2);

	for(int i = 0; i < SIZE; i++){
		printf("%c", code[i]);
	}

	fclose(file2);
	free(symbols);
	free(code);
}