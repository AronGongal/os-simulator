#include <stdio.h>
#include "common.h"


int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		printf("%s", "Usage: ./main.o OR ./main.o filename\n");
		return 0;
	}
	
	smm_init(); // initialize the Simple Memory Manager
	load_programs((argc == 2) ? argv[1] : "program_list_valid.txt");
	initialize(); // load register values of first process
	
	
	int keep_running = 1;
	while (keep_running == 1)
	{
		int status = clock_cycle();
		keep_running = schedule(status);
	}
	
	printf("Total Holes Created: %i\n", get_hole_count());
	
	int locations[3] = {30, 150, 230};
	for (int i = 0; i < 3; i++)
	{
		int* word = main_read(locations[i]);
		printf("Data at Address %i: %i %i\n", locations[i], word[0], word[1]);
	}
	
	return 0;
}
