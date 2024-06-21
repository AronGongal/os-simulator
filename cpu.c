#include <stddef.h>
#include "common.h"

struct registers regs;

struct registers context_switch(struct registers new_process)
{
	struct registers old_process = regs;
	regs = new_process;
	
	return old_process;
}

void fetch_instruction(int addr)
{
	int *instr = mem_read(get_pid(), addr);
	
	if (instr == NULL)
	{
		regs.ir0 = 0;
		return;
	}
	regs.ir0 = *instr;
	regs.ir1 = *(instr + 1);
}

void execute_instruction()
{
	switch(regs.ir0)
	{
		case 1: // load_const
			regs.ac = regs.ir1;
			break;
		case 2: // move_from_mbr
			regs.ac = regs.mbr;
			break;
		case 3: // move_from_mar
			regs.ac = regs.mar;
			break;
		case 4: // move_to_mbr
			regs.mbr = regs.ac;
			break;
		case 5: // move_to_mar
			regs.mar = regs.ac;
			break;
		case 6: // load_at_addr
		{
			int* buffer = mem_read(get_pid(), regs.mar);
			if (buffer != NULL)
			{
				regs.mbr = *buffer;
			}
			else regs.ir0 = 0;
			break;
		}
		case 7: // write_at_addr
		{
			int word[2] = {regs.mbr, 0};
			if (mem_write(get_pid(), regs.mar, word) == 0)
			{
				regs.ir0 = 0;
			}
			break;
		}
		case 8: // add
			regs.ac += regs.mbr;
			break;
		case 9: // multiply
			regs.ac = regs.ac * regs.mbr;
			break;
		case 10: // and
			regs.ac = regs.ac && regs.mbr;
			break;
		case 11: // or
			regs.ac = regs.ac || regs.mbr;
			break;
		case 12: // ifgo
			if (regs.ac != 0)
				regs.pc = regs.ir1 - 1;
			break;
		default: // exit, sleep
			break;
	}
}

int mem_address(int l_addr)
{
	return regs.base + l_addr;
}

int clock_cycle()
{
	fetch_instruction(mem_address(regs.pc));
	execute_instruction();
	regs.pc += 1;
	
	if (regs.ir0 == 0)
		return 0;
	else return 1;
}
