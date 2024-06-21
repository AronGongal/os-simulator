#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"


void add_hole(int base, int size);
void merge_holes();
int find_hole(int size);
int find_empty_row();

int holes = 0;
int a_table[256][3]; //pid, base, size


struct hole
{
	int base;
	int size;
	
	struct hole *next;
};

struct hole *h_head = NULL;

void smm_init()
{
	// add intial hole for entire main memory space
	h_head = (struct hole*)malloc(sizeof(struct hole));
	h_head->base = 0;
	h_head->size = 1024;
	h_head->next = NULL;
	holes++;
}

int allocate(int pid, int size)
{
	int base = find_hole(size);
	
	if (base == -1)
	{
		printf("Could not allocate memory for process %i.\n", pid);
		return 0;
	}
	
	int index = find_empty_row();
	
	a_table[index][0] = pid;
	a_table[index][1] = base;
	a_table[index][2] = size;
	
	
	return 1;
}

void deallocate(int pid)
{
	for(int i = 0; i < 256; i++)
	{
		if (a_table[i][0] == pid)
		{
			add_hole(a_table[i][1], a_table[i][2]);
			a_table[i][2] = 0;
			return;
		}
	}
}

void add_hole(int base, int size)
{
	struct hole *entry = malloc(sizeof(struct hole));
	entry->base = base;
	entry->size = size;
	entry->next = NULL;
	
	if (h_head == NULL)
	{
		h_head = entry;
		holes++;
		return;
	}
	
	struct hole *h = h_head;
	
	while (h != NULL)
	{
		if (base < h->base)
		{
			entry->next = h;
			if (h_head == h)
			{
				h_head = entry;
			}
			holes++;
			break;
		}
		if (h->next == NULL)
		{
			h->next = entry;
			holes++;
			break;
		}
		h = h->next;
	}
	
	merge_holes();
}

void remove_hole(int base)
{
	if (h_head == NULL) return;
	
	if (h_head->next == NULL)
	{
		if (h_head->base == base)
		{
			free(h_head);
			h_head = NULL;
		}
		return;
	}
	
	struct hole *h = h_head;
	
	while (h->next->base != base)
	{
		h = h->next;
		
		if (h->next == NULL) return;
	}
	
	struct hole *removed_hole = h->next;
	h->next = h->next->next;
	free(removed_hole);
}

void merge_holes()
{
	if (h_head == NULL) return;
	
	struct hole *h = h_head;
	while(h->next != NULL)
	{
		int end = h->base + h->size - 1;
		// if the current hole overlaps with the next
		if (end >= h->next->base)
		{
			int next_end = h->next->base + h->next->size - 1;
			h->size = next_end - h->base + 1;
			remove_hole(h->next->base);
		}
		h = h->next;
	}
}

int find_hole(int size)
{
	struct hole *h = h_head;
	
	while (h != NULL)
	{
		if (h->size >= size)
		{
			int old_base = h->base;
			if (size == h->size)
			{
				remove_hole(old_base);
			}
			else
			{
				h->base = old_base + size;
				h->size -= size;
			}
			return old_base;
		}
		h = h->next;
	}
	
	return -1;
}

int get_base_address(int pid)
{
	for(int i = 0; i < 256; i++)
	{
		if (a_table[i][0] == pid)
		{
			return a_table[i][1];
		}
	}
	
	return -1;
}

int find_empty_row()
{
	for(int i = 0; i < 256; i++)
	{
		if (a_table[i][2] == 0)
		{
			return i;
		}
	}
	
	return -1;
}

int is_allowed_address(int pid, int addr)
{
	int base;
	int size;
	
	for(int i = 0; i < 256; i++)
	{
		if (a_table[i][0] == pid)
		{
			base = a_table[i][1];
			size = a_table[i][2];
			break;
		}
	}

	if (addr >= base && addr < base + size)
	{
		return 1;
	}
	printf("Address %i outside of bounds of process %i.\n", addr, pid);
	return 0;
}

int get_hole_count()
{
	return holes;
}
