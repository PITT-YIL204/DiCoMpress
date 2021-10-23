#include <stdlib.h>
#include <stdio.h>

char command[4096] = {0};

//for demonstration, we decode the last dcm only
int main(int argc, char** argv) {

	system("mkdir ..\\build");

	int i;
	for(i = 1; i < argc; i++) {
		sprintf(command, "cp %s ../build/%s",argv[i], argv[i]);
		system(command);
	}

	return 0;
}