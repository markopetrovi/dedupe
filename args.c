/* Copyright (C) Marko Petrovic 2023
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or 
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program. 
* If not, see <https://www.gnu.org/licenses/>.
*/

// In this file are the functions for parsing options from arguments. 
// To add new option, add check function declaration. 
// It should get single argument, the string it should parse at that moment.
// If it handles argument correctly it should set option_correct to true.
// Call it from argparse() with argv[argoffset] at defined place. 
// Optionally add variable for storing option result that will be used in other modules.
// If your option uses more than one argument, increment argoffset

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "helper.h"
#include "man.h"

int argoffset;
static bool option_correct;
// Add option variables here with their default values:
bool direct = true;
__u64 chunk_size = 4*1024;	// In bytes - 4KB

// Add checks for new options after this line
static bool mode = false;
static void direct_cmp_o(char arg[])
{
	if(strcmp("--direct-cmp", arg) == 0 || strcmp("-d", arg) == 0)
	{
		if(!mode)
		{
			option_correct = true;
			direct = true;
			mode = true;
		}
	}
}
static void cross_cmp_o(char arg[])
{
	if(strcmp("--cross-cmp", arg) == 0 || strcmp("-c", arg) == 0)
	{
		if(!mode)
		{
			option_correct = true;
			direct = false;
			mode = true;
		}
	}
}
static void chunk_size_o(char arg[])
{
	int len = strlen("--chunk-size=");
	char* num = &arg[len];
	if(strncmp("--chunk-size=", arg, len) == 0)
	{
		__u64 br = atoi(num);
		if(br == 0)
			return;
		for(int i = 0; num[i] != 0; i++)
		{
			if(num[i] == 'K')
			{
				br = br * 1024;
				break;	
			}
			if(num[i] == 'M')
			{
				br = br * 1024 * 1024;
				break;
			}
		}
		if(br > 16*1024*1024)
			return;
		chunk_size = br;
		option_correct = true;
	}
}
void argparse(int argc, char* argv[])
{
	// Check if help was requested
	if(argc >= 2)
	if(strcmp("--help", argv[1]) == 0)
		help(0);
	// Check if there are too few arguments
	if(argc < 3)
	{
		printf("Bad arguments! Usage: dedupe [option] <file1> <file2>\n");
		printf_sync("Enter dedupe --help for help page.\n");
		syscall(SYS_exit_group, 1);
	}
	// Parse options and set argoffset at the offset of argument for first filename
	for(argoffset = 1; argoffset < argc - 2; argoffset++)
	{
		option_correct = false;
		// Call checks for new options here:
		cross_cmp_o(argv[argoffset]);
		direct_cmp_o(argv[argoffset]);
		chunk_size_o(argv[argoffset]);
		
		if(!option_correct)
		{
			printf("Bad option! Look at the help page:\n\n");
			help(2);
		}
	}
}

