#include <stdio.h>
#include <string.h>
#include "common.h"


int* translate(char *instruction);

void load_prog(char *fname, int addr, int size)
{
	FILE *fp;
	fp = fopen(fname, "r");
	
	char line[100];
	new_process(addr, size);
	while(fgets(line, 100, fp) != NULL)
	{
		if (strncmp(line, "//", 2) != 0)
		{
			int args[2];
			memcpy(args, translate(line), 2 * sizeof(int));
			main_write(addr, args);
			addr++;
		}
	}
	
	fclose(fp);
}

void load_programs(char fname[])
{
	FILE *fp;
	fp = fopen(fname, "r");
	int pid = 0;
	
	char line[100];
	while(fgets(line, 100, fp) != NULL)
	{
		int size;
		char file_name[100];
		sscanf(line, "%i %s", &size, file_name);
		
		if (allocate(pid, size) == 1)
		{
			load_prog(file_name, get_base_address(pid), size);
			pid++;
		}
	}
	
	fclose(fp);
}

int streq(char *str1, char *str2)
{
	return (strcmp(str1, str2) == 0) ? 1 : 0;
}

int* translate(char *instruction)
{
	char cmd[50];
	int arg;
	
	sscanf(instruction, "%s %i", cmd, &arg);
	
	int translation[2];
	
	translation[0] = streq(cmd, "exit") ? 0
	: streq(cmd, "load_const") ? 1
	: streq(cmd, "move_from_mbr") ? 2
	: streq(cmd, "move_from_mar") ? 3
	: streq(cmd, "move_to_mbr") ? 4
	: streq(cmd, "move_to_mar") ? 5
	: streq(cmd, "load_at_addr") ? 6
	: streq(cmd, "write_at_addr") ? 7
	: streq(cmd, "add") ? 8
	: streq(cmd, "multiply") ? 9
	: streq(cmd, "and") ? 10
	: streq(cmd, "or") ? 11
	: streq(cmd, "ifgo") ? 12 : 13;
	
	
	if (streq(cmd, "load_const") || streq(cmd, "ifgo"))
	{
		translation[1] = arg;
	}
	else translation[1] = 0;
	
	int* ptr = translation;
	
	return ptr;
}
