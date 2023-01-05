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

// TODO: Add support for cross compare deduplication - compare every chunk from source to every chunk from destination
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/fs.h>
#include "helper.h"
#include "args.c"
#include "work.h"

extern int argoffset;
// Declare option variables for the use of main program here:
extern bool direct;
extern __u64 chunk_size;

int main(int argc, char* argv[], char* envp[])
{
	argparse(argc, argv);
	int srcfd = open_check(argv[argoffset], O_RDONLY);
	struct file_dedupe_range *fdr = malloc(struct_size);
	
	memset(fdr, 0, sizeof(struct_size));
	fdr->dest_count = 1;
	fdr->src_length = chunk_size;
	fdr->info->dest_fd = open_check(argv[argoffset+1], O_WRONLY);
	
	unsigned long long int count;
	if(direct)
		count = do_direct(fdr, srcfd);
	else
		count = do_cross(fdr, srcfd);
	
	if(count >= 1024)
	{
		count = count / 1024;
		if(count > 1024)
			printf("Deduplicated %llu megabytes!\n", count/1024);
		else
			printf("Deduplicated %llu kilobytes!\n", count);
	}
	else
		printf("Deduplicated %llu bytes!\n", count);
	
	close(srcfd);
	close(fdr->info->dest_fd);
	return 0;
}
