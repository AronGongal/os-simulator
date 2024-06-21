struct registers 
{
	int base;
	int pc;
	int ir0;
	int ir1;
	int ac;
	int mar;
	int mbr;
};

int* main_read(int addr);
void main_write(int addr, int* data);
int* mem_read(int pid, int addr);
int mem_write(int pid, int addr, int* data);

int clock_cycle();
struct registers context_switch(struct registers new_process);

void load_programs(char fname[]);

void initialize();
int schedule(int process_status);
void new_process(int base, int size);
int get_pid();

int allocate(int pid, int size);
void deallocate(int pid);
void add_hole(int base, int size);
int get_base_address(int pid);
int is_allowed_address(int pid, int addr);
int get_hole_count();
void smm_init();
