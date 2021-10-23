#include <stdlib.h>
#include <stdio.h>

char command[4096] = {0};

//for demonstration, we decode the last dcm only
int main(int argc, char** argv) {

	if (argc != 2) {
        fprintf(stderr, "Usage: decode <encoded serial(e.g. 17)>\n");
        return 1;
    }

	system("mkdir decoded");
	sprintf(command, "decmat.exe image.vpy %s.mat",argv[1]);
	system(command);
	sprintf(command, "py dcmrc.py %s encoded/%s",argv[1], argv[1]);
	// printf(command);
	system(command);
	system("rm *.mat");

	return 0;
}