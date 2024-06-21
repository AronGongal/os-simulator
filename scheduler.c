#include <stddef.h>
#include <stdlib.h>
#include "common.h"


struct pcb 
{
	int id;
	int size;
	
	struct registers regs;
};

struct node
{
	struct pcb data;
	struct node *next;
	struct node *prev;
};

struct node *head = NULL;
struct node *tail = NULL;

struct node *create(struct pcb data)
{
	struct node *entry = malloc(sizeof(struct node));
	entry->data = data;
	entry->next = NULL;
	entry->prev = NULL;
	
	return entry;
}

void push(struct node *entry)
{
	if (head == NULL)
	{
		head = entry;
	}
	else
	{
		tail->next = entry;
		entry->prev = tail;
	}
	
	tail = entry;
}

struct node *pop()
{
	if (head == NULL)
	{
		return NULL;
	}
	
	struct node *popped = head;
	if (head == tail)
	{
		head = NULL;
		tail = NULL;
	}
	else
	{
		head = head->next;
		head->prev = NULL;
	}
	popped->next = NULL;
	
	return popped;
}

struct pcb p_table[1024];
int table_index = 0;
int time = 10;

void next_process() 
{
	struct node *process = pop();
	push(process);
}

void new_process(int base, int size) 
{
	struct registers regs;
	regs.base = base;
	regs.pc = 0;
	struct pcb block = {table_index, size, regs};
	
	p_table[table_index] = block;
	table_index++;
	
	struct node *process = create(block);
	push(process);
}

void initialize()
{
	context_switch(head->data.regs);
}

int schedule(int process_status) 
{
	time--;
	if (process_status == 0) // terminated
	{
		deallocate(head->data.id);
		struct node *removed_node = pop();
		free(removed_node);
		
		if (head == NULL)
		{
			return 0;
		}
		
		context_switch(head->data.regs);
		time = 10;
	}
	else if (time <= 0) // swapped out
	{
		int id = head->data.id;
		next_process();
		if (head->data.id != id)
			tail->data.regs = context_switch(head->data.regs);
		
		time = 10;
	}
	
	return 1;
}

int get_pid()
{
	return head->data.id;
}
