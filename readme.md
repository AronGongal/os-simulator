# Operating System Simulator

By Aron Gongal


## Description
This project simulates a simple processor and operating system. It takes a list of programs written in a simple instruction set and executes them concurrently. It then prints the total amount of holes created and the values of three memory locations.

The cpu scheduler implements round robin to schedule processes, and the simple memory manager implements contiguous memory allocation and dynamic partitioning.


## Installation
Compile the source code with `make`. The GCC compiler is required.


## Usage
Run `main.o`, optionally with the filename of the program list as an argument (
i.e. `./main.o filename`). The default filename will be `program_list_valid.txt`. 

Also included are `program_list_invalid_access.txt`, which attempts to allocate too much memory, and `program_list_invalid_allocation.txt`, which has a program that will attempt to write outside of its available memory space.


## License
Licensed under the MIT License.
