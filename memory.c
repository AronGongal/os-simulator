#include <stddef.h>
#include "common.h"

int mem[1024][2];

int is_valid(int pid, int addr)
{
	if (is_allowed_address(pid, addr))
	{
		return 1;
	}
	
	return 0;
}

int* main_read(int addr)
{
	int* ptr = mem[addr];
	return ptr;
}

void main_write(int addr, int* data)
{
	mem[addr][0] = data[0];
	mem[addr][1] = data[1];
}

int* mem_read(int pid, int addr)
{
	if (is_valid(pid, addr))
	{
		int* ptr = mem[addr];
		return ptr;
	}
	
	return NULL;
}

int mem_write(int pid, int addr, int* data)
{
	if (is_valid(pid, addr))
	{
		mem[addr][0] = data[0];
		mem[addr][1] = data[1];
		return 1;
	}
	
	return 0;
}
