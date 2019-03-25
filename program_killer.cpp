#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char const *argv[]){
	char killer_cmd[] = "kill -9 ";
	strcat(killer_cmd, argv[1]);
	system(killer_cmd);
	exit (0);
}