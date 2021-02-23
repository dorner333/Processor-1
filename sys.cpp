#include <stdlib.h>

int main(){

	system("gcc asm.cpp");
	system("./a.out cmd.asm");
	system("gcc proc.cpp");
	system("./a.out bin.out");
	system("gcc dasm.cpp");
	system("./a.out bin.out");
	
	return 0;
}